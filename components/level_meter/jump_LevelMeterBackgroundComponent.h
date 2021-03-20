#pragma once

//======================================================================================================================
namespace jump::LevelMeter
{
    //==================================================================================================================
    class BackgroundComponent   :   public juce::Component,
                                    private juce::ValueTree::Listener
    {
    public:
        //==============================================================================================================
        struct LookAndFeelMethods
        {
            virtual ~LookAndFeelMethods() = default;

            virtual void drawLevelMeterBackground(juce::Graphics& g, const BackgroundComponent& component) const = 0;
            virtual float getLevelMeterGridlineInterval(const BackgroundComponent& component) const = 0;
            virtual void drawLevelMeterGridline(juce::Graphics& g, const BackgroundComponent& component,
                                                float normalisedLevel, float decibelLevel,
                                                LevelMeter::Orientation orientation) const = 0;
        };

        //==============================================================================================================
        explicit BackgroundComponent(const Engine& correspondingEngine)
            :   engine{ correspondingEngine }
        {
        }

        ~BackgroundComponent()
        {
        }

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
        const Engine& engine;

        LookAndFeelAccessor<LookAndFeelMethods> lookAndFeel{ *this };

        Orientation orientation{ Orientation::Vertical };
    };
}   // namespace jump
