#include "jump_SpectrumAnalyserEngine.h"

//======================================================================================================================
namespace jump::SpectrumAnalyser
{
    //==================================================================================================================
    Engine::StateInitialiser::StateInitialiser(Engine& engine)
    {
        engine.setProperty(PropertyIDs::windowingMethodId, juce::VariantConverter<WindowingMethod>::toVar(WindowingMethod::hann));
        engine.setProperty(PropertyIDs::fftOrderId,        0);
        engine.setProperty(PropertyIDs::frequencyRangeId,  "[20.0, 20000.0]");
        engine.setProperty(PropertyIDs::decibelRangeId,    "[-100.0, 0.0]");
        engine.setProperty(PropertyIDs::holdTimeId,        100.f);
        engine.setProperty(PropertyIDs::maxHoldTimeId,     10000.f);
        engine.setProperty(PropertyIDs::decayTimeId,       500.f);
        engine.setProperty(PropertyIDs::numPointsId,       256);
    }

    //==================================================================================================================
    Engine::Engine(const juce::Identifier& uniqueID, StatefulObject* parentState)
        :   AudioComponentEngine{ uniqueID, parentState }
    {
    }

    //==================================================================================================================
    void Engine::addSamples(const std::vector<float>& samples)
    {
        for (auto& sample : samples)
            buffer.write(sample);
    }

    //==================================================================================================================
    void Engine::setSampleRate(double newSampleRate)
    {
        jassert(newSampleRate > 0.0);

        setProperty(SharedPropertyIDs::sampleRateId, newSampleRate);
    }

    void Engine::setWindowingMethod(WindowingMethod newWindowingMethod)
    {
        setProperty(PropertyIDs::windowingMethodId, juce::VariantConverter<WindowingMethod>::toVar(newWindowingMethod));
    }

    void Engine::setFFTOrder(int newFFTOrder)
    {
        jassert(newFFTOrder >= 0);

        setProperty(PropertyIDs::fftOrderId, newFFTOrder);
    }

    void Engine::setFrequencyRange(const juce::NormalisableRange<float>& newFrequencyRange)
    {
        jassert(newFrequencyRange.start > 0.f);
        jassert(newFrequencyRange.end <= nyquistFrequency);

        const auto value = juce::var{ juce::Array<juce::var>{ newFrequencyRange.start, newFrequencyRange.end } };
        setProperty(PropertyIDs::frequencyRangeId, juce::JSON::toString(value, true));
    }

    void Engine::setDecibelRange(const juce::NormalisableRange<float>& newDecibelRange)
    {
        const auto value = juce::var{ juce::Array<juce::var>{ newDecibelRange.start, newDecibelRange.end } };
        setProperty(PropertyIDs::decibelRangeId, juce::JSON::toString(value, true));
    }

    void Engine::setHoldTime(float newHoldTimeMs)
    {
        jassert(newHoldTimeMs >= 0.f);
        jassert(newHoldTimeMs <= maxHoldTime);

        setProperty(PropertyIDs::holdTimeId, newHoldTimeMs);
    }

    void Engine::setMaxHoldTime(float newMaxHoldTimeMs)
    {
        jassert(newMaxHoldTimeMs >= 0.f);
        jassert(holdTime <= newMaxHoldTimeMs);

        setProperty(PropertyIDs::maxHoldTimeId, newMaxHoldTimeMs);
    }

    void Engine::setDecayTime(float newDecayTimeMs)
    {
        jassert(newDecayTimeMs >= 0.f);

        setProperty(PropertyIDs::decayTimeId, newDecayTimeMs);
    }

    void Engine::setNumPoints(int newNumPoints)
    {
        jassert(newNumPoints >= 0);

        setProperty(PropertyIDs::numPointsId, newNumPoints);
    }

    //==================================================================================================================
    Engine::AnalyserPointInfo::AnalyserPointInfo(int fftBinIndex, float frequency,
                                                 const juce::NormalisableRange<float>& freqRange)
        :   binIndex{ fftBinIndex },
            normalisedX{ Math::inverseLogSpace(freqRange.start, freqRange.end, frequency) }
    {
    }

    //==================================================================================================================
    float getDecibelLevelFromFFTData(const std::vector<float>& fftData, int binIndex, int fftSize,
                                     const juce::NormalisableRange<float>& decibelRange)
    {
        const auto gain = fftData[binIndex] / (fftSize * 2.f);
        return juce::Decibels::gainToDecibels(gain, decibelRange.start);
    }

    void Engine::AnalyserPointInfo::update(const std::vector<float>& fftData, int fftSize,
                                           const juce::NormalisableRange<float>& decibelRange, juce::uint32 now,
                                           float holdTime, float maxHoldTime, float decayTime)
    {
        auto newDB = getDecibelLevelFromFFTData(fftData, binIndex, fftSize, decibelRange);

        if (newDB < dB)
            newDB = applyEnvelopeToDecibelLevel(levelOfLatestPeak, timeOfLatestPeak, now, holdTime, maxHoldTime, decayTime, decibelRange);
        else
        {
            timeOfLatestPeak = juce::Time::getMillisecondCounter();
            levelOfLatestPeak = newDB;
        }

        dB = newDB;
    }

    juce::Point<float> Engine::AnalyserPointInfo::normalise(const juce::NormalisableRange<float>& decibelRange)
    {
        return { normalisedX, 1.f - decibelRange.convertTo0to1(decibelRange.snapToLegalValue(dB)) };
    }

    //==================================================================================================================
    std::vector<float> getFFTData(CircularBuffer<float>& buffer, juce::dsp::FFT& fft, juce::dsp::WindowingFunction<float>& windowingFunction)
    {
        auto fftData = buffer.read();
        fftData.resize(static_cast<std::size_t>(fft.getSize()) * 2, 0.f);

        windowingFunction.multiplyWithWindowingTable(fftData.data(), fftData.size());
        fft.performFrequencyOnlyForwardTransform(fftData.data());

        return fftData;
    }

    void Engine::update(juce::uint32 now)
    {
        if (fft.get() == nullptr)
            return;

        const auto fftData = getFFTData(buffer, *fft, *windowingFunction);

        if (fftData.size() == 0)
            return;

        auto prevBin = -1;

        std::vector<juce::Point<float>> points;
        points.reserve(numPoints);

        for (auto& pointInfo : pointsInfo)
        {
            pointInfo.update(fftData, fft->getSize(), decibelRange, now, holdTime, maxHoldTime, decayTime);

            if (prevBin != pointInfo.binIndex)
                points.push_back(pointInfo.normalise(decibelRange));

            prevBin = pointInfo.binIndex;
        }

        renderers.call(&Renderer::newSpectrumAnalyserPointsAvailable, this, points);
    }

    void Engine::propertyChanged(const juce::Identifier& name, const juce::var& newValue)
    {
        if (name == PropertyIDs::fftOrderId)
            setFFTOrderInternal(newValue);
        else if (name == SharedPropertyIDs::sampleRateId)
            setSampleRateInternal(newValue);
        else if (name == PropertyIDs::frequencyRangeId)
        {
            auto value = juce::JSON::fromString(newValue.toString());

            jassert(value.isArray());
            jassert(value.size() == 2);

            setFrequencyRangeInternal(juce::NormalisableRange<float>{ value[0], value[1] });
        }
        else if (name == PropertyIDs::decibelRangeId)
        {
            auto value = juce::JSON::fromString(newValue.toString());

            jassert(value.isArray());
            jassert(value.size() == 2);

            decibelRange = juce::NormalisableRange<float>{ value[0], value[1] };
        }
        else if (name == PropertyIDs::holdTimeId)
            holdTime = newValue;
        else if (name == PropertyIDs::maxHoldTimeId)
            maxHoldTime = newValue;
        else if (name == PropertyIDs::decayTimeId)
            decayTime = newValue;
        else if (name == PropertyIDs::numPointsId)
            numPoints = newValue;
        else if (name == PropertyIDs::windowingMethodId)
            windowingMethod = static_cast<juce::dsp::WindowingFunction<float>::WindowingMethod>(static_cast<int>(newValue));
        else
        {
            // Unhandled property ID.
            jassertfalse;
        }
    }

    //==================================================================================================================
    void Engine::updateBinRange()
    {
        if (fft.get() == nullptr)
            return;

        const auto numBinsUpToNyquist = fft->getSize() / 2;

        binRange.setStart(juce::roundToInt(numBinsUpToNyquist * frequencyRange.start / nyquistFrequency));
        binRange.setEnd  (juce::roundToInt(numBinsUpToNyquist * frequencyRange.end   / nyquistFrequency));

        if (binRange.getEnd() == 0)
            return;

        pointsInfo.clear();
        pointsInfo.reserve(numPoints);

        for (auto& frequency : Math::logRange(frequencyRange.start, frequencyRange.end, numPoints))
        {
            const auto binIndex = juce::roundToInt(juce::jmap(frequency,
                                                                frequencyRange.start, frequencyRange.end,
                                                                static_cast<float>(binRange.getStart()), static_cast<float>(binRange.getEnd())));

            pointsInfo.push_back({ binIndex, frequency, frequencyRange });
        }
    }

    //==================================================================================================================
    void Engine::setFFTOrderInternal(int newFFTOrder)
    {
        fft.reset(new juce::dsp::FFT{ newFFTOrder });
        windowingFunction.reset(new juce::dsp::WindowingFunction<float>{ static_cast<std::size_t>(1) << newFFTOrder, windowingMethod });
        buffer.resize(1 << newFFTOrder);

        if (newFFTOrder > 0)
        {
            // You need to call setSampleRate first before calling setFFTOrder.
            jassert(nyquistFrequency > 0.f);

            updateBinRange();
        }
    }

    void Engine::setSampleRateInternal(double newSampleRate)
    {
        nyquistFrequency = static_cast<float>(newSampleRate / 2.0);
        updateBinRange();
    }

    void Engine::setFrequencyRangeInternal(const juce::NormalisableRange<float>& newFrequencyRange)
    {
        frequencyRange = newFrequencyRange;
        updateBinRange();
    }
}   // namespace jump::SpectrumAnalyser
