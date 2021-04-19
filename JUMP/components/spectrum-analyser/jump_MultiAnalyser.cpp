#include "jump_MultiAnalyser.h"

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    MultiAnalyser::MultiAnalyser(const std::vector<SpectrumAnalyserEngine*>& enginesToUse,
                                 juce::Identifier type, StatefulObject* parentState)
        :   StatefulObject{ type, parentState },
            mainEngine{ *enginesToUse.front() },
            labels{ *enginesToUse.front() }
    {
        initialiseState();

        for (auto& engine : enginesToUse)
        {
            auto analyser = std::make_unique<SpectrumAnalyser>(*engine);
            addAndMakeVisible(*analyser);
            analyser->setBackgroundVisible(analysers.size() == 0);
            analysers.add(std::move(analyser));
        }

        if (analysers.size() > 1)
        {
            analysers[0]->setColour(spectrumAnalyserSafeColourId,    MaterialColourPalette::blueGrey300);
            analysers[0]->setColour(spectrumAnalyserWarningColourId, MaterialColourPalette::blueGrey200);
            analysers[0]->setColour(spectrumAnalyserDangerColourId,  MaterialColourPalette::blueGrey100);
            analysers[0]->setShouldFillGraph(false);
        }

        addAndMakeVisible(labels);
    }

    //==================================================================================================================
    void MultiAnalyser::setShowLabels(bool shouldShowLabels)
    {
        setProperty(PropertyIDs::labelsVisibleId, shouldShowLabels);
    }

    void MultiAnalyser::setHighlightedLevels(const std::vector<float>& newLevels)
    {
        setProperty(PropertyIDs::highlightedLevelsId, juce::VariantConverter<std::vector<float>>::toVar(newLevels));
    }

    void MultiAnalyser::setHighlightedFrequencies(const std::vector<float>& newFrequencies)
    {
        setProperty(PropertyIDs::highlightedFrequenciesId, juce::VariantConverter<std::vector<float>>::toVar(newFrequencies));
    }

    //==================================================================================================================
    void MultiAnalyser::resized()
    {
        auto bounds = getLocalBounds();

        labels.setBounds(bounds);

        for (auto& analyser : analysers)
            analyser->setBounds(bounds);
    }

    void MultiAnalyser::propertyChanged(const juce::Identifier& name, const juce::var& newValue)
    {
        if (name == PropertyIDs::labelsVisibleId)
            labels.setVisible(static_cast<bool>(newValue));
        else if (name == PropertyIDs::highlightedLevelsId)
            labels.setHighlightedLevels(juce::VariantConverter<std::vector<float>>::fromVar(newValue));
        else if (name == PropertyIDs::highlightedFrequenciesId)
            labels.setHighlightedFrequencies(juce::VariantConverter<std::vector<float>>::fromVar(newValue));
    }

    //==================================================================================================================
    void MultiAnalyser::initialiseState()
    {
        setProperty(PropertyIDs::labelsVisibleId, true);

        setProperty(PropertyIDs::highlightedLevelsId,
                    juce::VariantConverter<std::vector<float>>::toVar({
                        -12.f, -24.f, -36.f, -48.f, -60.f, -72.f, -84.f, -96.f
                    }));

        setProperty(PropertyIDs::highlightedFrequenciesId,
                    juce::VariantConverter<std::vector<float>>::toVar({
                        50.f, 100.f, 200.f, 500.f, 1000.f, 2000.f, 5000.f, 10000.f
                    }));
    }
}   // namespace jump
