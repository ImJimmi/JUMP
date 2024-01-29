#include "jump_Compressor.h"

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    template <typename ValueType>
    bool changeValue(ValueType& valueToChange, const ValueType& newValue)
    {
        auto hasValueChanged = !juce::approximatelyEqual(valueToChange, newValue);
        valueToChange = newValue;

        return hasValueChanged;
    }

    void Compressor::prepare(const juce::dsp::ProcessSpec& processSpec)
    {
        if (changeValue(sampleRate, static_cast<float>(processSpec.sampleRate)))
            sampleRateChanged();

        if (changeValue(numChannels, static_cast<int>(processSpec.numChannels)))
            numChannels = static_cast<int>(processSpec.numChannels);
    }

    void Compressor::process(const juce::dsp::ProcessContextReplacing<float>& context)
    {
        const auto& inBlock = context.getInputBlock();
        const auto numSamples = static_cast<int>(inBlock.getNumSamples());

        for (auto channel = 0; channel < numChannels; channel++)
        {
            const auto channelAsSizeT = static_cast<std::size_t>(channel);
            const auto* inSamples = inBlock.getChannelPointer(channelAsSizeT);
            auto* outSamples = context.getOutputBlock().getChannelPointer(channelAsSizeT);

            for (auto i = 0; i < numSamples; i++)
                outSamples[i] = processSample(channel, inSamples[i]);
        }
    }

    //==================================================================================================================
    float Compressor::getSampleRate() const noexcept
    {
        return sampleRate;
    }

    int Compressor::getNumChannels() const noexcept
    {
        return numChannels;
    }

    void Compressor::setThreshold(const Level<float>& newThreshold)
    {
        jassert(newThreshold.toDecibels() <= 0.f);

        if (changeValue(threshold, newThreshold))
            thresholdChanged();
    }

    const Level<float>& Compressor::getThreshold() const noexcept
    {
        return threshold;
    }

    void Compressor::setKnee(const Level<float>& newKnee)
    {
        jassert(newKnee.toDecibels() <= 0.f);

        if (changeValue(knee, newKnee))
            kneeChanged();
    }

    const Level<float>& Compressor::getKnee() const noexcept
    {
        return knee;
    }

    void Compressor::setRatio(float newRatio)
    {
        jassert(newRatio >= 1.f);

        if (changeValue(ratio, newRatio))
            ratioChanged();
    }

    float Compressor::getRatio() const noexcept
    {
        return ratio;
    }

    void Compressor::setAttack(float newAttackTimeMs)
    {
        jassert(newAttackTimeMs >= 0.f);

        if (changeValue(attackMs, newAttackTimeMs))
            attackChanged();
    }

    float Compressor::getAttack() const noexcept
    {
        return attackMs;
    }

    void Compressor::setRelease(float newReleaseTimeMs)
    {
        jassert(newReleaseTimeMs >= 0.f);

        if (changeValue(releaseMs, newReleaseTimeMs))
            releaseChanged();
    }

    float Compressor::getRelease() const noexcept
    {
        return releaseMs;
    }

    const Level<float>& Compressor::getGainReduction() const noexcept
    {
        return gainReduction;
    }

    //==================================================================================================================
    void Compressor::setGainReduction(const Level<float>& newGainReduction)
    {
        jassert(newGainReduction.toGain() >= 0.f);

        if (changeValue(gainReduction, newGainReduction))
            gainReductionChanged();
    }

    //==================================================================================================================
    void Compressor::reset()
    {
        sampleRate = 0.f;
        threshold = Level<float>::fromGain(0.f);
        knee = Level<float>::fromGain(0.f);
        ratio = 0.f;
        attackMs = 0.f;
        releaseMs = 0.f;
    }

    //==================================================================================================================
    void Compressor::sampleRateChanged()
    {
    }

    void Compressor::numChannelsChanged()
    {
    }

    void Compressor::thresholdChanged()
    {
    }

    void Compressor::kneeChanged()
    {
    }

    void Compressor::ratioChanged()
    {
    }

    void Compressor::attackChanged()
    {
    }

    void Compressor::releaseChanged()
    {
    }

    void Compressor::gainReductionChanged()
    {
    }
} // namespace jump
