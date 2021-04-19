#include "jumpDemo_SpectrumAnalysers.h"

//======================================================================================================================
namespace jumpDemo
{
    //==================================================================================================================
    juce::Font getTitleFont()
    {
        return { "Helvetica", 24.f, 0 };
    }

    SpectrumAnalysers::SpectrumAnalysers()
    {
        addAndMakeVisible(title);
        title.setText("Spectrum Analysers", juce::dontSendNotification);
        title.setJustificationType(juce::Justification::centred);
        title.setFont(getTitleFont());

        addAndMakeVisible(analyser);
    }

    //==================================================================================================================
    void SpectrumAnalysers::setSampleRate(double sampleRate)
    {
        inputEngine.setSampleRate(sampleRate);
        outputEngine.setSampleRate(sampleRate);

        inputEngine.setFFTOrder(13);
        outputEngine.setFFTOrder(13);
    }

    void SpectrumAnalysers::addInputSamples(const std::vector<float>& samples)
    {
        inputEngine.addSamples(samples);
    }

    void SpectrumAnalysers::addOutputSamples(const std::vector<float>& samples)
    {
        outputEngine.addSamples(samples);
    }

    //==================================================================================================================
    void SpectrumAnalysers::resized()
    {
        juce::Grid grid;

        grid.templateRows = {
            juce::Grid::Px{ static_cast<int>(std::ceil(title.getFont().getHeight())) },
            juce::Grid::Fr{ 1 }
        };
        grid.templateColumns = { juce::Grid::Fr{ 1 } };

        const auto gap = 10;
        grid.setGap(juce::Grid::Px{ gap });

        grid.items = {
            juce::GridItem{ title },
            juce::GridItem{ analyser }
        };

        grid.performLayout(getLocalBounds().reduced(gap));
    }
}   // namespace jumpDemo
