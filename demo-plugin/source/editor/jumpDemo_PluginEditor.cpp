#include "jumpDemo_PluginEditor.h"

//======================================================================================================================
namespace jumpDemo
{
    //==================================================================================================================
    PluginEditor::PluginEditor(PluginInstance& p)
        :   jump::PluginEditor{ p },
            pluginInstance{ p }
    {
        setLookAndFeel(&jumpLookAndFeel);

        addAndMakeVisible(levelMetersDemo);
        levelMetersDemo.setSampleRate(p.getSampleRate());

        setSize(720, 360);
        startTimerHz(60);
    }

    PluginEditor::~PluginEditor()
    {
        setLookAndFeel(nullptr);
    }

    //==================================================================================================================
    void PluginEditor::paint(juce::Graphics& g)
    {
        g.fillAll(findColour(juce::ResizableWindow::backgroundColourId));
    }

    void PluginEditor::resized()
    {
        levelMetersDemo.setBounds(getLocalBounds());
    }

    void PluginEditor::timerCallback()
    {
        const auto inputSamples = pluginInstance.getLatestInputSamples();
        levelMetersDemo.addInputSamples(inputSamples);

        const auto outputSamples = pluginInstance.getLatestOutputSamples();
        levelMetersDemo.addOutputSamples(outputSamples);
    }
} // namespace jumpDemo
