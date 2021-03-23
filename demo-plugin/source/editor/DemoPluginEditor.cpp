#include "DemoPluginEditor.h"

//======================================================================================================================
DemoPluginEditor::DemoPluginEditor (DemoPluginInstance& p)
    :   jump::PluginEditor{ p },
        pluginInstance{ p }
{
    setLookAndFeel(&jumpLookAndFeel);

    addAndMakeVisible(levelMetersDemo);
    levelMetersDemo.setSampleRate(p.getSampleRate());

    setSize(720, 360);
    startTimerHz(60);
}

DemoPluginEditor::~DemoPluginEditor()
{
    setLookAndFeel(nullptr);
}

//======================================================================================================================
void DemoPluginEditor::paint (juce::Graphics& g)
{
    g.fillAll(findColour(juce::ResizableWindow::backgroundColourId));
}

void DemoPluginEditor::resized()
{
    levelMetersDemo.setBounds(getLocalBounds());
}

void DemoPluginEditor::timerCallback()
{
    const auto inputSamples = pluginInstance.getLatestInputSamples();
    levelMetersDemo.addInputSamples(inputSamples);

    const auto outputSamples = pluginInstance.getLatestOutputSamples();
    levelMetersDemo.addOutputSamples(outputSamples);
}
