#pragma once

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    class LookAndFeel   :   public juce::LookAndFeel_V4,
                            public LevelMeterBackgroundCanvas::LookAndFeelMethods,
                            public LevelMeterLabelsComponent::LookAndFeelMethods,
                            public LevelMeterRenderer::LookAndFeelMethods,
                            public MultiMeter::LookAndFeelMethods
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

        struct ColourPalette
        {
            static inline const juce::Colour blueGrey050{ 0xFFECEFF1 };
            static inline const juce::Colour blueGrey100{ 0xFFCFD8DC };
            static inline const juce::Colour blueGrey200{ 0xFFB0BEC5 };
            static inline const juce::Colour blueGrey300{ 0xFF90A4AE };
            static inline const juce::Colour blueGrey400{ 0xFF78909C };
            static inline const juce::Colour blueGrey500{ 0xFF607D8B };
            static inline const juce::Colour blueGrey600{ 0xFF546E7A };
            static inline const juce::Colour blueGrey700{ 0xFF455A64 };
            static inline const juce::Colour blueGrey800{ 0xFF37474F };
            static inline const juce::Colour blueGrey850{ 0xFF2E3C43 };
            static inline const juce::Colour blueGrey900{ 0xFF263238 };

            static inline const juce::Colour lightGreen500{ 0xFF8BC34A };
            static inline const juce::Colour yellow500    { 0xFFFFEB3B };
            static inline const juce::Colour red500       { 0xFFF44336 };
        };

        //==============================================================================================================
        LookAndFeel();

    private:
        //==============================================================================================================
        //================================//
        // MultiMeter::LookAndFeelMethods //
        //================================//
        int getLevelMetersGap(const MultiMeter& component) const noexcept override;

        //================================================//
        // LevelMeterBackgroundCanvas::LookAndFeelMethods //
        //================================================//
        void drawLevelMeterBackground(juce::Graphics& g,
                                      const LevelMeterBackgroundCanvas& component) const noexcept override;
        float getLevelMeterGridlineInterval(const LevelMeterBackgroundCanvas& component) const noexcept override;
        void drawLevelMeterGridline(juce::Graphics& g, const LevelMeterBackgroundCanvas& component,
                                    float normalisedLevel, float decibelLevel,
                                    Orientation orientation) const noexcept override;

        //========================================//
        // LevelMeterRenderer::LookAndFeelMethods //
        //========================================//
        juce::Path createLevelMeterPath(const LevelMeterRenderer& renderer, Orientation orientation,
                                        float peakLevel, float rmsLevel) const noexcept override;
        void drawJumpLevelMeter(juce::Graphics& g, const LevelMeterRenderer& renderer, Orientation orientation,
                                const juce::NormalisableRange<float>& decibelRange,
                                const juce::Path& meterPath) const noexcept override;

        //===============================================//
        // LevelMeterLabelsComponent::LookAndFeelMethods //
        //===============================================//
        juce::String getLevelMeterTextForLevel(float decibelLevel, bool isNegativeInf) const noexcept override;
        juce::Font getLevelMeterTextFont(float decibelLevel, bool isNegativeInf) const noexcept override;
        juce::Colour getLevelMeterTextColour(float decibelLevel, bool isNegativeInf) const noexcept override;
    };
}   // namespace jump
