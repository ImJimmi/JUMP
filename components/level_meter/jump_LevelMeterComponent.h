#pragma once

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    class LevelMeterComponent   :   public juce::Component,
                                    private StatefulObject
    {
    public:
        //==============================================================================================================
        enum class LabelsPosition
        {
            Left,
            Right,
            Above,
            Below,
            Centred
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

            virtual int getLevelMetersGap(const LevelMeterComponent& component) const noexcept = 0;
        };

        //==============================================================================================================
        LevelMeterComponent(const std::vector<LevelMeter::Engine*>& enginesToUse,
                            juce::Identifier type = "NonStatefulLevelMeterComponent",
                            StatefulObject* parentState = nullptr);

        //==============================================================================================================
        void setLabelsPosition(LabelsPosition newPosition);
        void setShowLabels(bool shouldShowLabels);
        void setDecibelLevelsForLabels(const std::vector<float>& newLabelLevels);
        void setOrientation(LevelMeter::Orientation newOrientation);

    private:
        //==============================================================================================================
        void resized() override;
        void propertyChanged(const juce::Identifier& name, const juce::var& newValue) override;

        //==============================================================================================================
        void initialiseState();

        //==============================================================================================================
        void setLabelsPositionInternal(LabelsPosition newLabelsPosition);
        void setOrientationInternal(LevelMeter::Orientation newOrientation);

        //==============================================================================================================
        const LevelMeter::Engine& mainEngine;
        juce::OwnedArray<LevelMeter::DefaultRenderer> meters;
        LevelMeter::LabelsComponent labels;

        LabelsPosition labelsPosition{ LabelsPosition::Left };
        LevelMeter::Orientation orientation{ LevelMeter::Orientation::Vertical };

        LookAndFeelAccessor<LookAndFeelMethods> lookAndFeel{ *this };

        static inline const juce::Identifier labelsLevelPropertyID{ "level" };

        //==============================================================================================================
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LevelMeterComponent)
    };
}   // namespace jump
