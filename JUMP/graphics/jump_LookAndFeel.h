#pragma once

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    namespace lookAndFeelImplementations
    {
        class LevelMeterLookAndFeel :   public LevelMeter::LookAndFeelMethods,
                                        public LevelMeterLabelsComponent::LookAndFeelMethods,
                                        public MultiMeter::LookAndFeelMethods
        {
            // LevelMeter
            void drawBackground(juce::Graphics& g, const LevelMeter& meter) const noexcept override;
            void drawLevelMeter(juce::Graphics& g, const LevelMeter& renderer,
                                float peakLevelNormalised, float rmsLevelNormalised) const noexcept override;

            // LevelMeterLabelsComponent
            std::unique_ptr<juce::Label> createLabelForLevel(const LevelMeterLabelsComponent&,
                                                             float level) const noexcept override;

            // MultiMeter
            int getGapBetweenMeters(const MultiMeter& component) const noexcept override;
        };
    }

    //==================================================================================================================
    struct LookAndFeel  :   public juce::LookAndFeel_V4,
                            public lookAndFeelImplementations::LevelMeterLookAndFeel
    {
        //==============================================================================================================
        LookAndFeel();
    };
}   // namespace jump
