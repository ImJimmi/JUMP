#pragma once

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    class Compressor : public juce::dsp::ProcessorBase
    {
    public:
        //==============================================================================================================
        Compressor() = default;

        //==============================================================================================================
        void prepare(const juce::dsp::ProcessSpec& processSpec) override;
        void process(const juce::dsp::ProcessContextReplacing<float>& context) override;

        //==============================================================================================================
        float getSampleRate() const noexcept;
        int getNumChannels() const noexcept;

        void setThreshold(const Level<float>& newThreshold);
        const Level<float>& getThreshold() const noexcept;

        void setKnee(const Level<float>& newKnee);
        const Level<float>& getKnee() const noexcept;

        void setRatio(float newRatio);
        float getRatio() const noexcept;

        void setAttack(float newAttackTimeMs);
        float getAttack() const noexcept;

        void setRelease(float releaseTimeMs);
        float getRelease() const noexcept;

        const Level<float>& getGainReduction() const noexcept;

    protected:
        //==============================================================================================================
        virtual float processSample(int channel, float sample) = 0;

        //==============================================================================================================
        void setGainReduction(const Level<float>& newGainReduction);

    private:
        //==============================================================================================================
        void reset() override;

        //==============================================================================================================
        virtual void sampleRateChanged();
        virtual void numChannelsChanged();
        virtual void thresholdChanged();
        virtual void kneeChanged();
        virtual void ratioChanged();
        virtual void attackChanged();
        virtual void releaseChanged();
        virtual void gainReductionChanged();

        //==============================================================================================================
        float sampleRate{ 0.f };
        int numChannels{ 0 };

        Level<float> threshold;
        Level<float> knee;
        float ratio{ 0.f };
        float attackMs{ 0.f };
        float releaseMs{ 0.f };
        Level<float> gainReduction{ jump::Level<float>::fromGain(1.f) };

        //==============================================================================================================
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Compressor)
    };
} // namespace jump
