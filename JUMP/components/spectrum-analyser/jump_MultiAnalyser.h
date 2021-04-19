#pragma once

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    class MultiAnalyser :   public Container,
                            private StatefulObject
    {
    public:
        //==============================================================================================================
        struct PropertyIDs
        {
            static inline const juce::Identifier labelsVisibleId         { "labelsVisible" };
            static inline const juce::Identifier highlightedLevelsId     { "highlightedLevels" };
            static inline const juce::Identifier highlightedFrequenciesId{ "highlightedFrequencies" };
        };

        //==============================================================================================================
        MultiAnalyser(const std::vector<SpectrumAnalyserEngine*>& enginesToUse,
                      juce::Identifier type = "NonStatefulMultiAnalyser",
                      StatefulObject* parentState = nullptr);

        //==============================================================================================================
        void setShowLabels(bool shouldShowLabels);
        void setHighlightedLevels(const std::vector<float>& newLevels);
        void setHighlightedFrequencies(const std::vector<float>& newFrequencies);

    private:
        //==============================================================================================================
        void resized() override;
        void propertyChanged(const juce::Identifier& name, const juce::var& newValue) override;

        //==============================================================================================================
        void initialiseState();

        //==============================================================================================================
        const SpectrumAnalyserEngine& mainEngine;
        juce::OwnedArray<SpectrumAnalyser> analysers;
        SpectrumAnalyserLabelsComponent labels;

        static inline const juce::Identifier labelsLevelPropertyID{ "level" };

        //==============================================================================================================
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MultiAnalyser)
    };
}   // namespace jump
