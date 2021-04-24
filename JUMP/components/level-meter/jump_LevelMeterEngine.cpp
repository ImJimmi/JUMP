#include "jump_LevelMeterEngine.h"

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    void LevelMeterEngine::initialise()
    {
        setProperty(PropertyIDs::rmsAttackTimeId,   150.f);
        setProperty(PropertyIDs::rmsReleaseTimeId,  350.f);
        setProperty(PropertyIDs::peakHoldTimeId,    400.f);
        setProperty(PropertyIDs::peakMaxHoldTimeId, 10000.f);
        setProperty(PropertyIDs::peakReleaseTimeId, 1500.f);
        setProperty(PropertyIDs::decibelRangeId,    "[-100.0, 0.0, 2.5]");
    }

    //==================================================================================================================
    LevelMeterEngine::LevelMeterEngine()
    {
        initialise();
    }

    LevelMeterEngine::LevelMeterEngine(const juce::Identifier & uniqueID, StatefulObject* parentState)
        :   AudioComponentEngine{ uniqueID, parentState }
    {
        initialise();
    }

    //==================================================================================================================
    void LevelMeterEngine::addSamples(const std::vector<float>& samples)
    {
        buffer.insert(std::end(buffer), std::begin(samples), std::end(samples));
    }

    //==================================================================================================================
    void LevelMeterEngine::setSampleRate(double newSampleRate)
    {
        jassert(newSampleRate > 0.0);

        setProperty(SharedPropertyIDs::sampleRateId, newSampleRate);
    }

    void LevelMeterEngine::setRMSAttackTime(float newAttackTimeMS)
    {
        jassert(newAttackTimeMS >= 0.f);

        setProperty(PropertyIDs::rmsAttackTimeId, newAttackTimeMS);
    }

    void LevelMeterEngine::setRMSReleaseTime(float newReleaseTimeMS)
    {
        jassert(newReleaseTimeMS >= 0.f);

        setProperty(PropertyIDs::rmsReleaseTimeId, newReleaseTimeMS);
    }

    void LevelMeterEngine::setPeakHoldTime(float newHoldTimeMS)
    {
        jassert(newHoldTimeMS >= 0.f);

        setProperty(PropertyIDs::peakHoldTimeId, newHoldTimeMS);
    }

    void LevelMeterEngine::setPeakMaxHoldTime(float newMaxHoldTimeMS)
    {
        jassert(newMaxHoldTimeMS >= 0.f);

        setProperty(PropertyIDs::peakMaxHoldTimeId, newMaxHoldTimeMS);
    }

    void LevelMeterEngine::setPeakReleaseTime(float newReleaseTimeMS)
    {
        jassert(newReleaseTimeMS >= 0.f);

        setProperty(PropertyIDs::peakReleaseTimeId, newReleaseTimeMS);
    }

    void LevelMeterEngine::setDecibelRange(const juce::NormalisableRange<float>& newDecibelRange)
    {
        const auto value = var_cast<juce::NormalisableRange<float>>(newDecibelRange);
        setProperty(PropertyIDs::decibelRangeId, value);
    }

    const juce::NormalisableRange<float>& LevelMeterEngine::getDecibelRange() const noexcept
    {
        return decibelRange;
    }

    //==================================================================================================================
    void callRenderers(juce::ListenerList<LevelMeterRendererBase>& renderers, const LevelMeterEngine* engine,
                       juce::var peak, juce::var rms, const juce::NormalisableRange<float>& decibelRange)
    {
        if (peak != juce::var() && rms != juce::var())
        {
            const auto peakValue = normaliseDecibelsTo0To1(static_cast<float>(peak), decibelRange);
            const auto rmsValue = normaliseDecibelsTo0To1(static_cast<float>(rms), decibelRange);

            renderers.call(&LevelMeterRendererBase::newLevelMeterLevelsAvailable, *engine, peakValue, rmsValue);
        }
    }

    void LevelMeterEngine::update(juce::uint32 now)
    {
        if (!rmsFilterIsPrepared || buffer.size() == 0)
            return;

        juce::var peak;
        juce::var rms;

        for (auto& value : buffer)
        {
            rms = rmsFilter.processSample(0, value);
            peak = updatePeak(value, now);
        }

        rms = juce::Decibels::gainToDecibels(static_cast<float>(rms), decibelRange.start);
        callRenderers(renderers, this, peak, rms, decibelRange);

        buffer.clear();
    }

    void LevelMeterEngine::propertyChanged(const juce::Identifier& name, const juce::var& newValue)
    {
        if (name == SharedPropertyIDs::sampleRateId)
            setSampleRateInternal(newValue);

        else if (name == PropertyIDs::rmsAttackTimeId)
            setRMSAttackTimeInternal(newValue);
        else if (name == PropertyIDs::rmsReleaseTimeId)
            setRMSReleaseTimeInternal(newValue);

        else if (name == PropertyIDs::peakReleaseTimeId)
            peakRelease = newValue;
        else if (name == PropertyIDs::peakHoldTimeId)
            peakHoldTime = newValue;
        else if (name == PropertyIDs::peakMaxHoldTimeId)
            peakMaxHoldTime = newValue;
        else if (name == PropertyIDs::decibelRangeId)
            decibelRange = var_cast<juce::NormalisableRange<float>>(newValue);
        else
        {
            // Unhandled property ID.
            jassertfalse;
        }
    }

    //==================================================================================================================
    float LevelMeterEngine::updatePeak(float gainValue, juce::uint32 now)
    {
        auto peakDB = juce::Decibels::gainToDecibels(gainValue, decibelRange.start);

        if (peakDB < previousPeakDB)
        {
            peakDB = applyEnvelopeToDecibelLevel(peakMaxDB, timeOfPeakMax, now, peakHoldTime, peakMaxHoldTime, peakRelease, decibelRange);
        }
        else
        {
            peakMaxDB = peakDB;
            timeOfPeakMax = now;
        }

        previousPeakDB = peakDB;

        return peakDB;
    }

    //==================================================================================================================
    void LevelMeterEngine::setSampleRateInternal(double newSampleRate)
    {
        if (newSampleRate <= 0.0)
            return;

        juce::dsp::ProcessSpec processSpec{};
        processSpec.sampleRate = newSampleRate;
        processSpec.numChannels = 1;

        rmsFilter.setLevelCalculationType(juce::dsp::BallisticsFilterLevelCalculationType::RMS);
        rmsFilter.prepare(processSpec);
        rmsFilterIsPrepared = true;
    }

    void LevelMeterEngine::setRMSAttackTimeInternal(float newAttackTimeMS)
    {
        rmsFilter.setAttackTime(newAttackTimeMS);
        rmsAttack = newAttackTimeMS;
    }

    void LevelMeterEngine::setRMSReleaseTimeInternal(float newReleaseTimeMS)
    {
        rmsFilter.setReleaseTime(newReleaseTimeMS);
        rmsRelease = newReleaseTimeMS;
    }
}   // namespace jump
