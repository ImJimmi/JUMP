#pragma once

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    class SpectrumAnalyser  :   public Container,
                                public SpectrumAnalyserRendererBase
    {
    public:
        //==============================================================================================================
        struct LookAndFeelMethods
        {
            virtual ~LookAndFeelMethods() = default;

            virtual void drawBackground(juce::Graphics& g, const SpectrumAnalyser& analyser) const noexcept = 0;
            virtual void drawSpectrumAnalyser(juce::Graphics& g, const SpectrumAnalyser& analyser,
                                              const std::vector<juce::Point<float>>& points) const noexcept = 0;
        };

        //==============================================================================================================
        explicit SpectrumAnalyser(const SpectrumAnalyserEngine& engineToUse)
            :   engine{ engineToUse }
        {
            lookAndFeel.attachTo(this);

            addAndMakeVisible(background);
            background.setDrawFunction([this](juce::Graphics& g) {
                lookAndFeel->drawBackground(g, *this);
            });

            addAndMakeVisible(analyser);
            analyser.setDrawFunction([this](juce::Graphics& g) {
                lookAndFeel->drawSpectrumAnalyser(g, *this, analyserPoints);
            });

            engineToUse.addRenderer(this);
        }

        ~SpectrumAnalyser() override
        {
            engine.removeRenderer(this);
        }

        //==============================================================================================================
        const SpectrumAnalyserEngine& getEngine() const noexcept
        {
            return engine;
        }

    private:
        //==============================================================================================================
        void resized() override
        {
            const auto bounds = getLocalBounds();

            background.setBounds(bounds);
            analyser.setBounds(bounds);
        }

        void newSpectrumAnalyserPointsAvailable(const SpectrumAnalyserEngine&,
                                                const std::vector<juce::Point<float>>& points) override
        {
            analyserPoints = points;
            analyser.repaint();
        }

        //==============================================================================================================
        const SpectrumAnalyserEngine& engine;

        Canvas background;
        Canvas analyser;

        std::vector<juce::Point<float>> analyserPoints;

        LookAndFeelAccessor<LookAndFeelMethods> lookAndFeel;
    };
}   // namespace jump
