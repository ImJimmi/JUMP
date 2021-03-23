#pragma once

#include <JuceHeader.h>
#include <core/DemoPluginInstance.h>

#include "demos/LevelMetersDemo.h"

//======================================================================================================================
class DemoPluginEditor  :   public jump::PluginEditor,
                            private juce::Timer
{
public:
    //==================================================================================================================
    DemoPluginEditor(DemoPluginInstance&);
    ~DemoPluginEditor() override;

private:
    //==================================================================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;

    //==================================================================================================================
    DemoPluginInstance& pluginInstance;
    jump::LookAndFeel jumpLookAndFeel;

    LevelMetersDemo levelMetersDemo;

    //==================================================================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DemoPluginEditor)
};
