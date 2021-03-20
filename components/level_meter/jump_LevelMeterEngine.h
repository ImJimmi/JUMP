#pragma once

//======================================================================================================================
namespace jump::LevelMeter
{
    //==================================================================================================================
    class Engine;

    //==================================================================================================================
    struct Renderer
    {
        //==============================================================================================================
        virtual ~Renderer() = default;

        //==============================================================================================================
        /** Derived classes must override this method in order to receive callbacks when a new set of points had been
            calculated by the given engine.
        */
        virtual void newLevelMeterLevelsAvailable(const Engine* engine, float peakLevel, float rmsLevel) = 0;
    };

    //==================================================================================================================
    /** Implements the logic required for a basic level meter with peak and RMS levels.

        Given a stream of samples, this class calculates a peak and an RMS level that can be used to draw a level meter.
        The peak and RMS levels are normalised so they need only be scaled with the desired width and height.

        The levels use a linear Decibel scale but normalised to 0-1 (e.g. if the decibel range is -100dB to 0dB and the
        peak level is -50dB, the given peak value will be 0.5).
    */
    class Engine    :   public AudioComponentEngine<Renderer>
    {
    public:
        //==================================================================================================================
        struct PropertyIDs
        {
            static const inline juce::Identifier rmsAttackTimeId  { "rmsAttackTime" };
            static const inline juce::Identifier rmsReleaseTimeId { "rmsReleaseTime" };
            static const inline juce::Identifier peakHoldTimeId   { "peakHoldTime" };
            static const inline juce::Identifier peakMaxHoldTimeId{ "peakMaxHoldTime" };
            static const inline juce::Identifier peakReleaseTimeId{ "peakReleaseTime" };
            static const inline juce::Identifier decibelRangeId   { "decibelRange" };
        };
        //==============================================================================================================
        Engine() = default;
        Engine(const juce::Identifier& uniqueID, StatefulObject* parentState);

        //==============================================================================================================
        void addSamples(const std::vector<float>& samples) override;

        //==============================================================================================================
        /** Specifies the sample rate of the samples being added to this engine.

            The default is 0Hz (so a real value must be set before adding samples).

            @param newSampleRate    The new sample rate to use.
        */
        void setSampleRate(double newSampleRate);

        /** Changes the attack time of the RMS envelope.

            The default is 150ms.

            @param newAttackTimeMS  The new attack time to use for the RMS envelope in milliseconds.
        */
        void setRMSAttackTime(float newAttackTimeMS);

        /** Changes the release time of the RMS envelope.

            The default is 350ms.

            @param newReleaseTimeMS The new release time to use for the RMS envelope in milliseconds.
        */
        void setRMSReleaseTime(float newReleaseTimeMS);

        /** Changes the hold time for the peak evnelope.

            If this is equal to or greater than the maximum hold time, the envelope will hold at the max peak forever
            (until a louder peak occurs).

            The default is 400ms.

            @param newHoldTime  The new hold time to use for the peak envelope in milliseconds.
        */
        void setPeakHoldTime(float newHoldTimeMS);

        /** Changes the maximum hold time for the peak envelope.

            If the hold time is equal to or greater than this, the envelope will hold at the max peak forever (until a
            louder peak occurs).

            The default is 10s.

            @param newMaxHoldTime   The new maximum hold time to use for the peak envelope in milliseconds.
        */
        void setPeakMaxHoldTime(float newMaxHoldTimeMS);

        /** Changes the release time of the peak envelope.

            The default is 1.5s.

            @param newReleaseTimeMS The new release time to use for the peak envelope in milliseconds.
        */
        void setPeakReleaseTime(float newReleaseTimeMS);

        /** Changes the decibel range of the meter.

            The start of the range will be treated as -inf Decibels and will be normalised to a value of 0 while the end
            of the range will be normalised to a value of 1.

            The default is -100dB to 0dB.

            @param newDecibelRange  The new decibel range to use.
        */
        void setDecibelRange(const juce::NormalisableRange<float>& newDecibelRange);

        const juce::NormalisableRange<float>& getDecibelRange() const noexcept;

    private:
        //==============================================================================================================
        struct StateInitialiser
        {
            StateInitialiser(Engine& engine);
        };

        //==============================================================================================================
        void update(juce::uint32 now) override;
        void propertyChanged(const juce::Identifier& name, const juce::var& newValue) override;

        //==============================================================================================================
        float updatePeak(float gainValue, juce::uint32 now);

        //==============================================================================================================
        void setSampleRateInternal(double newSampleRate);
        void setRMSAttackTimeInternal(float newAttackTimeMS);
        void setRMSReleaseTimeInternal(float newReleaseTimeMS);

        //==============================================================================================================
        std::vector<float> buffer;
        juce::dsp::BallisticsFilter<float> rmsFilter;
        bool rmsFilterIsPrepared{ false };
        float rmsAttack{ 0.f };
        float rmsRelease{ 0.f };

        float previousPeakDB{ 0.f };
        juce::uint32 timeOfPeakMax{ 0 };
        float peakMaxDB{ 0.f };
        float peakHoldTime{ 0.f };
        float peakMaxHoldTime{ 0.f };
        float peakRelease{ 0.f };
        juce::NormalisableRange<float> decibelRange;

        StateInitialiser stateInitialiser{ *this };

        //==============================================================================================================
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Engine)
    };
}   // namespace jump
