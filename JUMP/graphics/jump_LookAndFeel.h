#pragma once

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    namespace lookAndFeelImplementations
    {
        //==============================================================================================================
        class LevelMeterLookAndFeel
            : public LevelMeter::LookAndFeelMethods
            , public LevelMeterLabelsComponent::LookAndFeelMethods
            , public MultiMeter::LookAndFeelMethods
        {
            // LevelMeter
            void drawBackground(juce::Graphics& g, const LevelMeter& meter) const noexcept override final;
            void drawLevelMeter(juce::Graphics& g, const LevelMeter& renderer,
                                float peakLevelNormalised, float rmsLevelNormalised) const noexcept override final;

            // LevelMeterLabelsComponent
            std::unique_ptr<juce::Label> createLabelForLevel(const LevelMeterLabelsComponent&,
                                                             float level) const noexcept override final;

            // MultiMeter
            int getGapBetweenMeters(const MultiMeter& component) const noexcept override final;
        };

        //==============================================================================================================
        class SpectrumAnalyserLookAndFeel
            : public SpectrumAnalyser::LookAndFeelMethods
            , public SpectrumAnalyserLabelsComponent::LookAndFeelMethods
        {
            // SpectrumAnalyser
            void drawBackground(juce::Graphics& g, const SpectrumAnalyser& analyser) const noexcept override final;
            void drawSpectrumAnalyser(juce::Graphics& g, const SpectrumAnalyser& analyser,
                                      const std::vector<juce::Point<float>>& points,
                                      const PaintOptions& paintOptions) const noexcept override final;

            // SpectrumAnalyserLabelsComponent
            std::unique_ptr<juce::Label> createLabelForLevel(const SpectrumAnalyserLabelsComponent& component,
                                                             float level) const noexcept override final;
            std::unique_ptr<juce::Label> createLabelForFrequency(const SpectrumAnalyserLabelsComponent& component,
                                                                 float frequency) const noexcept override final;
        };

        //==============================================================================================================
        class SvgLookAndFeel : public SvgComponent::LookAndFeelMethods
        {
            // SvgComponent
            void drawSvgComponent(juce::Graphics& g, const SvgComponent& component) const override final;
        };
    } // namespace lookAndFeelImplementations

    //==================================================================================================================
    struct LookAndFeel
        : public juce::LookAndFeel_V4
        , public lookAndFeelImplementations::LevelMeterLookAndFeel
        , public lookAndFeelImplementations::SpectrumAnalyserLookAndFeel
        , public lookAndFeelImplementations::SvgLookAndFeel
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
} // namespace jump
