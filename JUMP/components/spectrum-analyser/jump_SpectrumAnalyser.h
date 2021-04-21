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
                                  StatefulObject* parentState = nullptr);

        ~SpectrumAnalyser() override;

        //==============================================================================================================
        void setBackgroundVisible(bool backgroundShouldBeVisible);
        void setPaintOptions(const PaintOptions& options);

        const SpectrumAnalyserEngine& getEngine() const noexcept;

    private:
        //==============================================================================================================
        void resized() override;
        void newSpectrumAnalyserPointsAvailable(const SpectrumAnalyserEngine&,
                                                const std::vector<juce::Point<float>>& points) override;
        void propertyChanged(const juce::Identifier& name, const juce::var& newValue) override;

        //==============================================================================================================
        void initialiseState();

        //==============================================================================================================
        void setBackgroundVisibleInternal (bool shouldBeVisible);
        void setPaintOptionsInternal(const PaintOptions& options);

        //==============================================================================================================
        const SpectrumAnalyserEngine& engine;

        Canvas background;
        Canvas analyser;

        PaintOptions paintOptions{ PaintOptions::strokeAndFill };

        std::vector<juce::Point<float>> analyserPoints;

        LookAndFeelAccessor<LookAndFeelMethods> lookAndFeel;
    };
}   // namespace jump
