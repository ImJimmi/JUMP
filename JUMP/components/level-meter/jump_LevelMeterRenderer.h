#pragma once

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    class LevelMeterRenderer    :   public juce::Component,
                                    public LevelMeterRendererBase
    {
    public:
        //==============================================================================================================
        struct LookAndFeelMethods
        {
            virtual ~LookAndFeelMethods() = default;

            virtual juce::Path createLevelMeterPath(const LevelMeterRenderer& renderer, Orientation orientation,
                                                    float peakLevel, float rmsLevel) = 0;
            virtual void drawLevelMeter(juce::Graphics& g, const LevelMeterRenderer& renderer, Orientation orientation,
                                        const juce::NormalisableRange<float>& decibelRange,
                                        const juce::Path& meterPath) = 0;
        };

        //==============================================================================================================
        explicit LevelMeterRenderer(const LevelMeterEngine& engineToUse)
            :   engine{ engineToUse }
        {
            engineToUse.addRenderer(this);
        }

        ~LevelMeterRenderer()
        {
            engine.removeRenderer(this);
        }

        //==============================================================================================================
        void setOrientation(Orientation newOrientation)
        {
            orientation = newOrientation;
            repaint();
        }

        Orientation getOrientation() const
        {
            return orientation;
        }

    private:
        //==============================================================================================================
        void paint(juce::Graphics& g) override
        {
            lookAndFeel->drawLevelMeter(g, *this, orientation, engine.getDecibelRange(), meterPath);
        }

        void newLevelMeterLevelsAvailable(const LevelMeterEngine&, float peakLevel, float rmsLevel) override
        {
            if (!lookAndFeel)
                return;

            meterPath.clear();
            meterPath = lookAndFeel->createLevelMeterPath(*this, orientation, peakLevel, rmsLevel);
            repaint();
        }

        //==============================================================================================================
        const LevelMeterEngine& engine;

        LookAndFeelAccessor<LookAndFeelMethods> lookAndFeel{ *this };

        juce::Path meterPath;
        Orientation orientation{ Orientation::vertical };

        //==============================================================================================================
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LevelMeterRenderer)
    };
}   // namespace jump
