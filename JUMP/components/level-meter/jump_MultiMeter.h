#pragma once

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    class MultiMeter    :   public Container,
                            private StatefulObject
    {
    public:
        //==============================================================================================================
        enum class LabelsPosition
        {
            left,
            right,
            above,
            below,
            centred
        };

        struct PropertyIDs
        {
            static inline const juce::Identifier labelsPositionId   { "labelsPosition" };
            static inline const juce::Identifier labelsVisibleId    { "labelsVisible" };
            static inline const juce::Identifier highlightedLevelsId{ "highlightedLevels" };
            static inline const juce::Identifier orientationId      { "orientation" };
        };

        struct LookAndFeelMethods
        {
            virtual ~LookAndFeelMethods() = default;

            virtual int getLevelMetersGap(const MultiMeter& component) const noexcept = 0;
        };

        //==============================================================================================================
        MultiMeter(const std::vector<LevelMeterEngine*>& enginesToUse,
                   juce::Identifier type = "NonStatefulLevelMeterComponent",
                   StatefulObject* parentState = nullptr);

        //==============================================================================================================
        void setLabelsPosition(LabelsPosition newPosition);
        void setShowLabels(bool shouldShowLabels);
        void setDecibelLevelsForLabels(const std::vector<float>& newLabelLevels);
        void setOrientation(Orientation newOrientation);

    private:
        //==============================================================================================================
        void resized() override;
        void propertyChanged(const juce::Identifier& name, const juce::var& newValue) override;
        void colourChanged() override;

        //==============================================================================================================
        void initialiseState();

        //==============================================================================================================
        void setLabelsPositionInternal(LabelsPosition newLabelsPosition);
        void setOrientationInternal(Orientation newOrientation);

        //==============================================================================================================
        const LevelMeterEngine& mainEngine;
        juce::OwnedArray<LevelMeter> meters;
        LevelMeterLabelsComponent labels;

        LabelsPosition labelsPosition{ LabelsPosition::left };
        Orientation orientation{ Orientation::vertical };

        LookAndFeelAccessor<LookAndFeelMethods> lookAndFeel;

        static inline const juce::Identifier labelsLevelPropertyID{ "level" };

        //==============================================================================================================
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MultiMeter)
    };
}   // namespace jump
