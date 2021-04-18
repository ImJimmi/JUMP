#pragma once

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    namespace lookAndFeelImplementations
    {
        //==============================================================================================================
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

        //==============================================================================================================
        class SpectrumAnalyserLookAndFeel   :   public SpectrumAnalyser::LookAndFeelMethods
        {
            void drawBackground(juce::Graphics& g, const SpectrumAnalyser& analyser) const noexcept override;
            void drawSpectrumAnalyser(juce::Graphics& g, const SpectrumAnalyser& analyser,
                                      const std::vector<juce::Point<float>>& points) const noexcept override;
        };
    }

    //==================================================================================================================
    struct LookAndFeel  :   public juce::LookAndFeel_V4,
                            public lookAndFeelImplementations::LevelMeterLookAndFeel,
                            public lookAndFeelImplementations::SpectrumAnalyserLookAndFeel
    {
        //==============================================================================================================
        struct ColourScheme
        {
            juce::Colour windowBackground;

            juce::Colour widgetBackground;
            juce::Colour widgetBorder;
            juce::Colour textNormal;
            juce::Colour textBold;

            juce::Colour safe;
            juce::Colour warning;
            juce::Colour danger;
        };

        //==============================================================================================================
        LookAndFeel();
        explicit LookAndFeel(const ColourScheme& initialScheme);

        //==============================================================================================================
        void setColourScheme(const ColourScheme& newScheme);

        static ColourScheme getMaterialColourScheme();
    };
}   // namespace jump
