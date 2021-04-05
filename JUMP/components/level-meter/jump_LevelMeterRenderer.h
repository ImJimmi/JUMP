#pragma once

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    class LevelMeterRenderer    :   public Canvas,
                                    public LevelMeterRendererBase
    {
    public:
        //==============================================================================================================
        struct LookAndFeelMethods
        {
            virtual ~LookAndFeelMethods() = default;

            virtual juce::Path createLevelMeterPath(const LevelMeterRenderer& renderer, Orientation orientation,
                                                    float peakLevel, float rmsLevel) const noexcept = 0;
            virtual void drawJumpLevelMeter(juce::Graphics& g, const LevelMeterRenderer& renderer, Orientation orientation,
                                            const juce::NormalisableRange<float>& decibelRange,
                                            const juce::Path& meterPath) const noexcept = 0;
        };

        //==============================================================================================================
        explicit LevelMeterRenderer(const LevelMeterEngine& engineToUse)
            :   engine{ engineToUse }
        {
            engineToUse.addRenderer(this);
            lookAndFeel.attachTo(this);
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
            lookAndFeel->drawJumpLevelMeter(g, *this, orientation, engine.getDecibelRange(), meterPath);
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

        LookAndFeelAccessor<LookAndFeelMethods> lookAndFeel;

        juce::Path meterPath;
        Orientation orientation{ Orientation::vertical };

        //==============================================================================================================
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LevelMeterRenderer)
    };
}   // namespace jump
