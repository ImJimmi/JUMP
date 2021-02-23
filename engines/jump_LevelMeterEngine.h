#pragma once

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    class LevelMeterEngine  :   public AudioComponentEngine<float>
    {
    public:
        //==============================================================================================================
        /** Defines the properties used by the state getters and setters.
        
            This avoids the need for hard-coded strings and makes it easier to
            see what properties are available in the state node.
        */
        struct PropertyIDs
        {
            PropertyIDs() = delete;

            static const inline juce::Identifier rmsAttackTimeId    { "rmsAttackTime" };
            static const inline juce::Identifier rmsReleaseTimeId   { "rmsReleaseTime" };
            static const inline juce::Identifier peakHoldTimeId     { "peakHoldTime" };
            static const inline juce::Identifier peakMaxHoldTimeId  { "peakMaxHoldTime" };
            static const inline juce::Identifier peakReleaseTimeId  { "peakReleaseTime" };
            static const inline juce::Identifier decibelRangeStartId{ "decibelRange.start" };
            static const inline juce::Identifier decibelRangeEndId  { "decibelRange.end" };
        };

        //==============================================================================================================
        LevelMeterEngine()
        {
            rmsFilter .setLevelCalculationType(juce::dsp::BallisticsFilterLevelCalculationType::RMS);
            rmsFilter.setAttackTime (rmsAttack);
            rmsFilter.setReleaseTime(rmsRelease);
        }

        //==============================================================================================================
        void addSamples(const std::vector<float>& samples)
        {
            buffer.insert(std::end(buffer), std::begin(samples), std::end(samples));
        }

        //==============================================================================================================
        /** Specifies the current sample rate being used.

            This is important in order for the frequency of the FFT bins to be accurately calculated.

            @param newSampleRate    The new sample rate being used.
        */
        void setSampleRate(double newSampleRate)
        {
            juce::dsp::ProcessSpec processSpec;
            processSpec.sampleRate  = newSampleRate;
            processSpec.numChannels = 1;

            rmsFilter.prepare(processSpec);
        }

        void setRMSAttackTime(float newAttackTimeMS)
        {
            rmsFilter.setAttackTime(newAttackTimeMS);
            rmsAttack = newAttackTimeMS;
        }

        void setRMSReleaseTime(float newReleaseTimeMS)
        {
            rmsFilter.setReleaseTime(newReleaseTimeMS);
            rmsRelease = newReleaseTimeMS;
        }

        void setPeakHoldTime(float newHoldTime)
        {
            peakHoldTime = newHoldTime;
        }

        void setPeakMaxHoldTime(float newMaxHoldTime)
        {
            peakMaxHoldTime = newMaxHoldTime;
        }

        void setPeakReleaseTime(float newReleaseTime)
        {
            peakRelease = newReleaseTime;
        }

        juce::ValueTree getStateInformation(const juce::String& nodeName) const override
        {
            return {
                nodeName,
                {
                    { PropertyIDs::rmsAttackTimeId,     rmsAttack },
                    { PropertyIDs::rmsReleaseTimeId,    rmsRelease },

                    { PropertyIDs::peakHoldTimeId,      peakHoldTime },
                    { PropertyIDs::peakMaxHoldTimeId,   peakMaxHoldTime },
                    { PropertyIDs::peakReleaseTimeId,   peakRelease },

                    { PropertyIDs::decibelRangeStartId, decibelRange.start },
                    { PropertyIDs::decibelRangeEndId,   decibelRange.end }
                }
            };
        }

        void setStateInformation(const juce::ValueTree& node) override
        {
            setRMSAttackTime (static_cast<float>(node[PropertyIDs::rmsAttackTimeId]));
            setRMSReleaseTime(static_cast<float>(node[PropertyIDs::rmsReleaseTimeId]));

            setPeakHoldTime   (static_cast<float>(node[PropertyIDs::peakHoldTimeId]));
            setPeakMaxHoldTime(static_cast<float>(node[PropertyIDs::peakMaxHoldTimeId]));
            setPeakReleaseTime(static_cast<float>(node[PropertyIDs::peakReleaseTimeId]));

            decibelRange.start = static_cast<float>(node[PropertyIDs::decibelRangeStartId]);
            decibelRange.end   = static_cast<float>(node[PropertyIDs::decibelRangeEndId]);
        }

        //==============================================================================================================
        std::function<void(float peakLevel, float rmsLevel)> onNewLevelsCalculated = nullptr;

    private:
        //==============================================================================================================
        void update() override
        {
            juce::var peak;
            juce::var rms;

            const auto now = juce::Time::getMillisecondCounter();

            for (auto& value : buffer)
            {
                rms = rmsFilter.processSample(0, value);

                auto peakDB = juce::Decibels::gainToDecibels(value, decibelRange.start);

                if (peakDB < previousPeakDB)
                {
                    const auto ellapsedTime = static_cast<float>(now - timeOfPeakMax);
                    auto multiplier{ 1.f };

                    if (!(ellapsedTime < peakHoldTime || peakHoldTime == peakMaxHoldTime))
                        multiplier = juce::jlimit(0.f, 1.f, 1.f - (ellapsedTime - peakHoldTime) / peakRelease);

                    peakDB = ((peakMaxDB - decibelRange.start) * multiplier) + decibelRange.start;
                }
                else
                {
                    peakMaxDB = peakDB;
                    timeOfPeakMax = now;
                }

                peak = peakDB;
                previousPeakDB = peakDB;
            }

            buffer.clear();

            if (peak != juce::var() && rms != juce::var())
            {
                rms = juce::Decibels::gainToDecibels(static_cast<float>(rms), decibelRange.start);

                if (onNewLevelsCalculated != nullptr)
                    onNewLevelsCalculated(1.f - decibelRange.convertTo0to1(peak), 1.f - decibelRange.convertTo0to1(rms));
            }
        }

        //==============================================================================================================
        std::vector<float> buffer;
        juce::dsp::BallisticsFilter<float> rmsFilter;
        float rmsAttack{ 150.f };
        float rmsRelease{ 350.f };

        float previousPeakDB{ 0.f };
        juce::uint32 timeOfPeakMax{ 0 };
        float peakMaxDB{ 0.f };
        float peakHoldTime{ 400.f };
        float peakMaxHoldTime{ 10.f };
        float peakRelease{ 1500.f };

        juce::NormalisableRange<float> decibelRange{ -100.f, 0.f };

        //==============================================================================================================
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LevelMeterEngine)
    };
}   // namespace jump
