#pragma once

//======================================================================================================================
namespace jump::LevelMeter
{
    //==================================================================================================================
    class DefaultRenderer   :   public juce::Component,
                                public Renderer
    {
    public:
        //==============================================================================================================
        struct LookAndFeelMethods
        {
            virtual ~LookAndFeelMethods() = default;

            virtual juce::Path createLevelMeterPath(const juce::Component& component, Orientation orientation,
                                                    float peakLevel, float rmsLevel) = 0;
            virtual void drawLevelMeter(juce::Graphics& g, const juce::Component& component, Orientation orientation,
                                        const juce::NormalisableRange<float>& decibelRange,
                                        const juce::Path& meterPath) = 0;
        };

        //==============================================================================================================
        explicit DefaultRenderer(Engine& engineToUse)
            :   engine{ engineToUse }
        {
            engineToUse.addRenderer(this);

            addAndMakeVisible(background);
        }

        ~DefaultRenderer()
        {
            engine.removeRenderer(this);
        }

        //==============================================================================================================
        void setOrientation(Orientation newOrientation)
        {
            background.setOrientation(newOrientation);

            orientation = newOrientation;
            repaint();
        }

        Orientation getOrientation() const
        {
            return orientation;
        }

    private:
        //==============================================================================================================
        void paintOverChildren(juce::Graphics& g) override
        {
            lookAndFeel->drawLevelMeter(g, *this, orientation, engine.getDecibelRange(), meterPath);
        }

        void resized() override
        {
            background.setBounds(getLocalBounds());
        }

        void newLevelMeterLevelsAvailable(const Engine*, float peakLevel, float rmsLevel) override
        {
            meterPath.clear();
            meterPath = lookAndFeel->createLevelMeterPath(*this, orientation, peakLevel, rmsLevel);
            repaint();
        }

        //==============================================================================================================
        Engine& engine;
        BackgroundComponent background{ engine };

        LookAndFeelAccessor<LookAndFeelMethods> lookAndFeel{ *this };

        juce::Path meterPath;
        Orientation orientation{ Orientation::Vertical };
    };
}   // namespace jump
