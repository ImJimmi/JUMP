#include "jump_SpectrumAnalyserEngine.h"

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    void SpectrumAnalyserEngine::initialise()
    {
        setProperty(PropertyIDs::windowingMethodId, juce::VariantConverter<WindowingMethod>::toVar(WindowingMethod::hann));
        setProperty(PropertyIDs::fftOrderId,        0);
        setProperty(PropertyIDs::frequencyRangeId,  "[20.0, 20000.0]");
        setProperty(PropertyIDs::decibelRangeId,    "[-100.0, 0.0]");
        setProperty(PropertyIDs::holdTimeId,        100.f);
        setProperty(PropertyIDs::maxHoldTimeId,     10000.f);
        setProperty(PropertyIDs::decayTimeId,       500.f);
        setProperty(PropertyIDs::numPointsId,       256);
    }

    //==================================================================================================================
    SpectrumAnalyserEngine::SpectrumAnalyserEngine()
    {
        initialise();
    }

    SpectrumAnalyserEngine::SpectrumAnalyserEngine(const juce::Identifier& uniqueID, StatefulObject* parentState)
        :   AudioComponentEngine{ uniqueID, parentState }
    {
        initialise();
    }

    //==================================================================================================================
    void SpectrumAnalyserEngine::addSamples(const std::vector<float>& samples)
    {
        for (auto& sample : samples)
            buffer.write(sample);
    }

    //==================================================================================================================
    void SpectrumAnalyserEngine::setSampleRate(double newSampleRate)
    {
        jassert(newSampleRate > 0.0);

        setProperty(SharedPropertyIDs::sampleRateId, newSampleRate);
    }

    void SpectrumAnalyserEngine::setWindowingMethod(WindowingMethod newWindowingMethod)
    {
        setProperty(PropertyIDs::windowingMethodId, juce::VariantConverter<WindowingMethod>::toVar(newWindowingMethod));
    }

    void SpectrumAnalyserEngine::setFFTOrder(int newFFTOrder)
    {
        jassert(newFFTOrder >= 0);

        setProperty(PropertyIDs::fftOrderId, newFFTOrder);
    }

    void SpectrumAnalyserEngine::setFrequencyRange(const juce::NormalisableRange<float>& newFrequencyRange)
    {
        jassert(newFrequencyRange.start > 0.f);
        jassert(newFrequencyRange.end <= nyquistFrequency);

        const auto value = juce::var{ juce::Array<juce::var>{ newFrequencyRange.start, newFrequencyRange.end } };
        setProperty(PropertyIDs::frequencyRangeId, juce::JSON::toString(value, true));
    }

    void SpectrumAnalyserEngine::setDecibelRange(const juce::NormalisableRange<float>& newDecibelRange)
    {
        const auto value = juce::var{ juce::Array<juce::var>{ newDecibelRange.start, newDecibelRange.end } };
        setProperty(PropertyIDs::decibelRangeId, juce::JSON::toString(value, true));
    }

    void SpectrumAnalyserEngine::setHoldTime(float newHoldTimeMs)
    {
        jassert(newHoldTimeMs >= 0.f);
        jassert(newHoldTimeMs <= maxHoldTime);

        setProperty(PropertyIDs::holdTimeId, newHoldTimeMs);
    }

    void SpectrumAnalyserEngine::setMaxHoldTime(float newMaxHoldTimeMs)
    {
        jassert(newMaxHoldTimeMs >= 0.f);
        jassert(holdTime <= newMaxHoldTimeMs);

        setProperty(PropertyIDs::maxHoldTimeId, newMaxHoldTimeMs);
    }

    void SpectrumAnalyserEngine::setDecayTime(float newDecayTimeMs)
    {
        jassert(newDecayTimeMs >= 0.f);

        setProperty(PropertyIDs::decayTimeId, newDecayTimeMs);
    }

    void SpectrumAnalyserEngine::setNumPoints(int newNumPoints)
    {
        jassert(newNumPoints >= 0);

        setProperty(PropertyIDs::numPointsId, newNumPoints);
    }

    //==================================================================================================================
    SpectrumAnalyserEngine::AnalyserPointInfo::AnalyserPointInfo(int fftBinIndex, float frequency,
                                                                 const juce::NormalisableRange<float>& freqRange)
        :   binIndex{ fftBinIndex },
            normalisedX{ Math::inverseLogSpace(freqRange.start, freqRange.end, frequency) }
    {
    }

    //==================================================================================================================
    float getDecibelLevelFromFFTData(const std::vector<float>& fftData, int binIndex, int fftSize,
                                     const juce::NormalisableRange<float>& decibelRange)
    {
        const auto gain = fftData[static_cast<std::size_t>(binIndex)] / (fftSize * 2.f);
        return juce::Decibels::gainToDecibels(gain, decibelRange.start);
    }

    void SpectrumAnalyserEngine::AnalyserPointInfo::update(const std::vector<float>& fftData, int fftSize,
                                                           const juce::NormalisableRange<float>& dBRange,
                                                           juce::uint32 now,
                                                           float holdTimeMS, float maxHoldTimeMS, float decayTimeMS)
    {
        auto newDB = getDecibelLevelFromFFTData(fftData, binIndex, fftSize, dBRange);

        if (newDB < dB)
            newDB = applyEnvelopeToDecibelLevel(levelOfLatestPeak, timeOfLatestPeak, now, holdTimeMS, maxHoldTimeMS, decayTimeMS, dBRange);
        else
        {
            timeOfLatestPeak = juce::Time::getMillisecondCounter();
            levelOfLatestPeak = newDB;
        }

        dB = newDB;
    }

    juce::Point<float> SpectrumAnalyserEngine::AnalyserPointInfo::normalise(const juce::NormalisableRange<float>& dBRange)
    {
        return { normalisedX, 1.f - dBRange.convertTo0to1(dBRange.snapToLegalValue(dB)) };
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

    void SpectrumAnalyserEngine::update(juce::uint32 now)
    {
        if (fft.get() == nullptr)
            return;

        const auto fftData = getFFTData(buffer, *fft, *windowingFunction);

        if (fftData.size() == 0)
            return;

        auto prevBin = -1;

        std::vector<juce::Point<float>> points;
        points.reserve(static_cast<std::size_t>(numPoints));

        for (auto& pointInfo : pointsInfo)
        {
            pointInfo.update(fftData, fft->getSize(), decibelRange, now, holdTime, maxHoldTime, decayTime);

            if (prevBin != pointInfo.binIndex)
                points.push_back(pointInfo.normalise(decibelRange));

            prevBin = pointInfo.binIndex;
        }

        renderers.call(&SpectrumAnalyserRendererBase::newSpectrumAnalyserPointsAvailable, *this, points);
    }

    void SpectrumAnalyserEngine::propertyChanged(const juce::Identifier& name, const juce::var& newValue)
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
    void SpectrumAnalyserEngine::updateBinRange()
    {
        if (fft.get() == nullptr)
            return;

        const auto numBinsUpToNyquist = fft->getSize() / 2;

        binRange.setStart(juce::roundToInt(numBinsUpToNyquist * frequencyRange.start / nyquistFrequency));
        binRange.setEnd  (juce::roundToInt(numBinsUpToNyquist * frequencyRange.end   / nyquistFrequency));

        if (binRange.getEnd() == 0)
            return;

        pointsInfo.clear();
        pointsInfo.reserve(static_cast<std::size_t>(numPoints));

        for (auto& frequency : Math::logRange(frequencyRange.start, frequencyRange.end, numPoints))
        {
            const auto binIndex = juce::roundToInt(juce::jmap(frequency,
                                                                frequencyRange.start, frequencyRange.end,
                                                                static_cast<float>(binRange.getStart()), static_cast<float>(binRange.getEnd())));

            pointsInfo.push_back({ binIndex, frequency, frequencyRange });
        }
    }

    //==================================================================================================================
    void SpectrumAnalyserEngine::setFFTOrderInternal(int newFFTOrder)
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

    void SpectrumAnalyserEngine::setSampleRateInternal(double newSampleRate)
    {
        nyquistFrequency = static_cast<float>(newSampleRate / 2.0);
        updateBinRange();
    }

    void SpectrumAnalyserEngine::setFrequencyRangeInternal(const juce::NormalisableRange<float>& newFrequencyRange)
    {
        frequencyRange = newFrequencyRange;
        updateBinRange();
    }
}   // namespace jump::SpectrumAnalyser
