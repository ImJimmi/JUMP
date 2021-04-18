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
        addAndMakeVisible(labels);
        labels.setHighlightedLevelsAndFrequencies({-12.f, -24.f, -36.f, -48.f, -60.f, -72.f, -84.f, -96.f},
                                                  {50.f, 100.f, 200.f, 500.f, 1000.f, 2000.f, 5000.f, 10000.f});
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

        labels.setBounds(analyser.getBounds());
    }
}   // namespace jumpDemo
