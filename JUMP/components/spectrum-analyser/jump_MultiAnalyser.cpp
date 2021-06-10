#include "jump_MultiAnalyser.h"

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    MultiAnalyser::MultiAnalyser(const std::vector<SpectrumAnalyserEngine*>& enginesToUse,
                                 juce::Identifier type, StatefulObject* parentState)
        : StatefulObject{ type, parentState }
        , mainEngine{ *enginesToUse.front() }
        , labels{ *enginesToUse.front(), "Labels" }
    {
        initialiseState();

        for (auto& engine : enginesToUse)
        {
            const juce::Identifier id{ "Analyser" + juce::String{ analysers.size() } };
            auto analyser = std::make_unique<SpectrumAnalyser>(*engine, id, this);
            addAndMakeVisible(*analyser);
            analyser->setBackgroundVisible(analysers.size() == 0);
            analysers.add(std::move(analyser));
        }

        if (analysers.size() > 1)
        {
            analysers[0]->setColour(spectrumAnalyserSafeColourId, MaterialColourPalette::blueGrey300);
            analysers[0]->setColour(spectrumAnalyserWarningColourId, MaterialColourPalette::blueGrey200);
            analysers[0]->setColour(spectrumAnalyserDangerColourId, MaterialColourPalette::blueGrey100);
            analysers[0]->setPaintOptions(PaintOptions{ PaintOptions::strokeFlag });
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
        labels.setHighlightedLevels(newLevels);
    }

    void MultiAnalyser::setHighlightedFrequencies(const std::vector<float>& newFrequencies)
    {
        labels.setHighlightedFrequencies(newFrequencies);
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
    }

    //==================================================================================================================
    void MultiAnalyser::initialiseState()
    {
        setProperty(PropertyIDs::labelsVisibleId, true);
    }
} // namespace jump
