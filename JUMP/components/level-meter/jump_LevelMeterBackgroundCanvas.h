#pragma once

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    class LevelMeterBackgroundCanvas    :   public Canvas,
                                            private juce::ValueTree::Listener
    {
    public:
        //==============================================================================================================
        struct LookAndFeelMethods
        {
            virtual ~LookAndFeelMethods() = default;

            virtual void drawLevelMeterBackground(juce::Graphics& g,
                                                  const LevelMeterBackgroundCanvas& component) const noexcept = 0;
            virtual float getLevelMeterGridlineInterval(const LevelMeterBackgroundCanvas& component) const noexcept = 0;
            virtual void drawLevelMeterGridline(juce::Graphics& g, const LevelMeterBackgroundCanvas& component,
                                                float normalisedLevel, float decibelLevel,
                                                Orientation orientation) const noexcept = 0;
        };

        //==============================================================================================================
        explicit LevelMeterBackgroundCanvas(const LevelMeterEngine& correspondingEngine)
            :   engine{ correspondingEngine }
        {
            lookAndFeel.attachTo(this);
        }

        ~LevelMeterBackgroundCanvas() override = default;

        //==============================================================================================================
        void setOrientation(Orientation newOrientation)
        {
            orientation = newOrientation;
            repaint();
        }

        Orientation getOrientation() const noexcept { return orientation; }

    private:
        //==============================================================================================================
        void paint(juce::Graphics& g) override
        {
            lookAndFeel->drawLevelMeterBackground(g, *this);

            const auto& decibelRange = engine.getDecibelRange();

            const auto gridlineInterval = lookAndFeel->getLevelMeterGridlineInterval(*this);

            for (auto dB = decibelRange.end; dB >= decibelRange.start; dB -= gridlineInterval)
            {
                const auto normalisedLevel = decibelRange.convertTo0to1(dB);
                lookAndFeel->drawLevelMeterGridline(g, *this, normalisedLevel, dB, orientation);
            }
        }

        //==============================================================================================================
        const LevelMeterEngine& engine;

        LookAndFeelAccessor<LookAndFeelMethods> lookAndFeel;

        Orientation orientation{ Orientation::vertical };

        //==============================================================================================================
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LevelMeterBackgroundCanvas)
    };
}   // namespace jump
