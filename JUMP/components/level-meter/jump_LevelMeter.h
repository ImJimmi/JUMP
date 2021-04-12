#pragma once

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    class LevelMeter    :   public Container,
                            public LevelMeterRendererBase
    {
    public:
        //==============================================================================================================
        struct LookAndFeelMethods
        {
            virtual ~LookAndFeelMethods() = default;

            virtual void drawBackground(juce::Graphics& g, const LevelMeter& meter) const noexcept = 0;
            virtual void drawLevelMeter(juce::Graphics& g, const LevelMeter& meter,
                                        float peakLevelNormalised, float rmsLevelNormalised) const noexcept = 0;
        };

        //==============================================================================================================
        explicit LevelMeter(const LevelMeterEngine& engineToUse)
            :   engine{ engineToUse }
        {
            lookAndFeel.attachTo(this);

            addAndMakeVisible(background);
            background.setDrawFunction([this](juce::Graphics& g) {
                lookAndFeel->drawBackground(g, *this);
            });

            addAndMakeVisible(meter);
            meter.setDrawFunction([this](juce::Graphics& g) {
                lookAndFeel->drawLevelMeter(g, *this, latestPeakLevel, latestRMSLevel);
            });
        }

        //==============================================================================================================
        void setOrientation(Orientation newOrientation)
        {
            orientation = newOrientation;
        }

        Orientation getOrientation() const noexcept
        {
            return orientation;
        }

        const LevelMeterEngine& getEngine() const noexcept
        {
            return engine;
        }

    private:
        //==============================================================================================================
        void resized() override
        {
            const auto bounds = getLocalBounds();

            background.setBounds(bounds);
            meter.setBounds(bounds);
        }

        void newLevelMeterLevelsAvailable(const LevelMeterEngine&, float peakLevel, float rmsLevel) override
        {
            latestPeakLevel = peakLevel;
            latestRMSLevel = rmsLevel;

            meter.repaint();
        }

        //==============================================================================================================
        const LevelMeterEngine& engine;
        Orientation orientation{ Orientation::vertical };

        Canvas background;
        Canvas meter;

        float latestPeakLevel{ 0.f };
        float latestRMSLevel{ 0.f };

        LookAndFeelAccessor<LookAndFeelMethods> lookAndFeel;

        //==============================================================================================================
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LevelMeter)
    };
}   // namespace jump
