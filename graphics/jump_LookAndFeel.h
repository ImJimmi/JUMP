#pragma once

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    class LookAndFeel   :   public juce::LookAndFeel_V4,
                            public LevelMeterComponent::LookAndFeelMethods,
                            public LevelMeter::BackgroundComponent::LookAndFeelMethods,
                            public LevelMeter::DefaultRenderer::LookAndFeelMethods,
                            public LevelMeter::LabelsComponent::LookAndFeelMethods
    {
    public:
        //==============================================================================================================
        enum ColourIds
        {
            widgetBackgroundColourId = 0x2000000,
            widgetBorderColourId,

            levelMeterSafeColourId,
            levelMeterWarningColourId,
            levelMeterDangerColourId,
            levelMeterLabelNormalTextColourId,
            levelMeterLabelHighlightedTextColourId
        };

        //==============================================================================================================
        LookAndFeel();

    private:
        //==============================================================================================================
        //=========================================//
        // LevelMeterComponent::LookAndFeelMethods //
        //=========================================//
        int getLevelMetersGap(const LevelMeterComponent& component) const noexcept override;

        //=====================================================//
        // LevelMeter::BackgroundComponent::LookAndFeelMethods //
        //=====================================================//
        void drawLevelMeterBackground(juce::Graphics& g,
                                      const LevelMeter::BackgroundComponent& component) const override;
        float getLevelMeterGridlineInterval(const LevelMeter::BackgroundComponent& component) const override;
        void drawLevelMeterGridline(juce::Graphics& g, const LevelMeter::BackgroundComponent& component,
                                    float normalisedLevel, float decibelLevel,
                                    LevelMeter::Orientation orientation) const override;

        //=================================================//
        // LevelMeter::DefaultRenderer::LookAndFeelMethods //
        //=================================================//
        juce::Path createLevelMeterPath(const juce::Component& component, LevelMeter::Orientation orientation,
                                        float peakLevel, float rmsLevel) override;
        void drawLevelMeter(juce::Graphics& g, const juce::Component& component, LevelMeter::Orientation orientation,
                            const juce::NormalisableRange<float>& decibelRange, const juce::Path& meterPath) override;

        //=================================================//
        // LevelMeter::LabelsComponent::LookAndFeelMethods //
        //=================================================//
        juce::String getLevelMeterTextForLevel(float decibelLevel, bool isNegativeInf) const override;
        juce::Font getLevelMeterTextFont(float decibelLevel, bool isNegativeInf) const override;
        juce::Colour getLevelMeterTextColour(float decibelLevel, bool isNegativeInf) const override;
    };
}   // namespace jump