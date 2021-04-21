#pragma once

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    class SpectrumAnalyser  :   public Container,
                                public SpectrumAnalyserRendererBase,
                                private StatefulObject
    {
    public:
        //==============================================================================================================
        struct LookAndFeelMethods
        {
            virtual ~LookAndFeelMethods() = default;

            virtual void drawBackground(juce::Graphics& g, const SpectrumAnalyser& analyser) const noexcept = 0;
            virtual void drawSpectrumAnalyser(juce::Graphics& g, const SpectrumAnalyser& analyser,
                                              const std::vector<juce::Point<float>>& points,
                                              const PaintOptions& paintOptions) const noexcept = 0;
        };

        struct PropertyIDs
        {
            static const inline juce::Identifier isBackgroundVisibleId{ "isBackgroundVisible" };
            static const inline juce::Identifier paintOptionsId       { "paintOptions" };
        };

        //==============================================================================================================
        explicit SpectrumAnalyser(const SpectrumAnalyserEngine& engineToUse,
                                  const juce::Identifier& uniqueID = "NonStatefulSpectrumAnalyser",
                                  StatefulObject* parentState = nullptr)
            :   StatefulObject{ uniqueID, parentState },
                engine{ engineToUse }
        {
            initialiseState();
            lookAndFeel.attachTo(this);

            addAndMakeVisible(background);
            background.setDrawFunction([this](juce::Graphics& g) {
                lookAndFeel->drawBackground(g, *this);
            });

            addAndMakeVisible(analyser);
            analyser.setDrawFunction([this](juce::Graphics& g) {
                lookAndFeel->drawSpectrumAnalyser(g, *this, analyserPoints, paintOptions);
            });

            engineToUse.addRenderer(this);
        }

        ~SpectrumAnalyser() override
        {
            engine.removeRenderer(this);
        }

        //==============================================================================================================
        void setBackgroundVisible(bool backgroundShouldBeVisible)
        {
            setProperty (PropertyIDs::isBackgroundVisibleId, backgroundShouldBeVisible);
        }

        void setPaintOptions(const PaintOptions& options)
        {
            setProperty (PropertyIDs::paintOptionsId, options.serialise());
        }

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

        void propertyChanged(const juce::Identifier& name, const juce::var& newValue) override
        {
            if (name == PropertyIDs::isBackgroundVisibleId)
                setBackgroundVisibleInternal(static_cast<bool>(newValue));
            else if (name == PropertyIDs::paintOptionsId)
                setPaintOptionsInternal(PaintOptions{ static_cast<juce::int64>(newValue) });
        }

        //==============================================================================================================
        void initialiseState()
        {
            setBackgroundVisible(true);
            setPaintOptions(PaintOptions{ PaintOptions::strokeAndFill });
        }

        //==============================================================================================================
        void setBackgroundVisibleInternal (bool shouldBeVisible)
        {
            background.setVisible(shouldBeVisible);
        }

        void setPaintOptionsInternal(const PaintOptions& options)
        {
            paintOptions = options;
            repaint();
        }

        //==============================================================================================================
        const SpectrumAnalyserEngine& engine;

        Canvas background;
        Canvas analyser;

        PaintOptions paintOptions{ PaintOptions::strokeAndFill };

        std::vector<juce::Point<float>> analyserPoints;

        LookAndFeelAccessor<LookAndFeelMethods> lookAndFeel;
    };
}   // namespace jump
