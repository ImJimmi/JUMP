#pragma once

//======================================================================================================================
namespace jump::LevelMeter
{
    //==================================================================================================================
    Engine::StateInitialiser::StateInitialiser(Engine& engine)
    {
        engine.setProperty(PropertyIDs::rmsAttackTimeId,   150.f);
        engine.setProperty(PropertyIDs::rmsReleaseTimeId,  350.f);
        engine.setProperty(PropertyIDs::peakHoldTimeId,    400.f);
        engine.setProperty(PropertyIDs::peakMaxHoldTimeId, 10000.f);
        engine.setProperty(PropertyIDs::peakReleaseTimeId, 1500.f);
        engine.setProperty(PropertyIDs::decibelRangeId,    "[-100.0, 0.0]");
    }

    //==================================================================================================================
    Engine::Engine(const juce::Identifier & uniqueID, StatefulObject* parentState)
        :   AudioComponentEngine{ uniqueID, parentState }
    {
    }

    //==================================================================================================================
    void Engine::addSamples(const std::vector<float>& samples)
    {
        buffer.insert(std::end(buffer), std::begin(samples), std::end(samples));
    }

    //==================================================================================================================
    void Engine::setSampleRate(double newSampleRate)
    {
        jassert(newSampleRate > 0.0);

        setProperty(SharedPropertyIDs::sampleRateId, newSampleRate);
    }

    void Engine::setRMSAttackTime(float newAttackTimeMS)
    {
        jassert(newAttackTimeMS >= 0.f);

        setProperty(PropertyIDs::rmsAttackTimeId, newAttackTimeMS);
    }

    void Engine::setRMSReleaseTime(float newReleaseTimeMS)
    {
        jassert(newReleaseTimeMS >= 0.f);

        setProperty(PropertyIDs::rmsReleaseTimeId, newReleaseTimeMS);
    }

    void Engine::setPeakHoldTime(float newHoldTimeMS)
    {
        jassert(newHoldTimeMS >= 0.f);

        setProperty(PropertyIDs::peakHoldTimeId, newHoldTimeMS);
    }

    void Engine::setPeakMaxHoldTime(float newMaxHoldTimeMS)
    {
        jassert(newMaxHoldTimeMS >= 0.f);

        setProperty(PropertyIDs::peakMaxHoldTimeId, newMaxHoldTimeMS);
    }

    void Engine::setPeakReleaseTime(float newReleaseTimeMS)
    {
        jassert(newReleaseTimeMS >= 0.f);

        setProperty(PropertyIDs::peakReleaseTimeId, newReleaseTimeMS);
    }

    void Engine::setDecibelRange(const juce::NormalisableRange<float>& newDecibelRange)
    {
        const auto value = juce::var{ juce::Array<juce::var>{ newDecibelRange.start, newDecibelRange.end } };
        setProperty(PropertyIDs::decibelRangeId, juce::JSON::toString(value, true));
    }

    //==================================================================================================================
    float normaliseDecibelValue(float value, const juce::NormalisableRange<float>& decibelRange)
    {
        return 1.f - ((value - decibelRange.start) / (decibelRange.end - decibelRange.start));
    }

    void callRenderers(juce::ListenerList<Renderer>& renderers, const Engine* engine, juce::var peak, juce::var rms,
                       const juce::NormalisableRange<float>& decibelRange)
    {
        if (peak != juce::var() && rms != juce::var())
        {
            rms = juce::Decibels::gainToDecibels(static_cast<float>(rms), decibelRange.start);

            renderers.call(&Renderer::newLevelMeterLevelsAvailable,
                           engine, normaliseDecibelValue(peak, decibelRange), normaliseDecibelValue(rms, decibelRange));
        }
    }

    void Engine::update(juce::uint32 now)
    {
        if (!rmsFilterIsPrepared)
            return;

        juce::var peak;
        juce::var rms;

        for (auto& value : buffer)
        {
            rms = rmsFilter.processSample(0, value);
            peak = updatePeak(value, now);
        }

        callRenderers(renderers, this, peak, rms, decibelRange);

        buffer.clear();
    }

    void Engine::propertyChanged(const juce::Identifier& name, const juce::var& newValue)
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
        {
            auto value = juce::JSON::fromString(newValue.toString());

            jassert(value.isArray());
            jassert(value.size() == 2);

            decibelRange = juce::NormalisableRange<float>{ value[0], value[1] };
        }
        else
        {
            // Unhandled property ID.
            jassertfalse;
        }
    }

    //==================================================================================================================
    float Engine::updatePeak(float gainValue, juce::uint32 now)
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
    void Engine::setSampleRateInternal(double newSampleRate)
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

    void Engine::setRMSAttackTimeInternal(float newAttackTimeMS)
    {
        rmsFilter.setAttackTime(newAttackTimeMS);
        rmsAttack = newAttackTimeMS;
    }

    void Engine::setRMSReleaseTimeInternal(float newReleaseTimeMS)
    {
        rmsFilter.setReleaseTime(newReleaseTimeMS);
        rmsRelease = newReleaseTimeMS;
    }
}   // namespace jump
