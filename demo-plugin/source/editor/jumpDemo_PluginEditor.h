#pragma once

#include "demos/jumpDemo_LevelMeters.h"

#include <JuceHeader.h>
#include <core/jumpDemo_PluginInstance.h>

//======================================================================================================================
namespace jumpDemo
{
    //==================================================================================================================
    class PluginEditor  :   public jump::PluginEditor,
                            private juce::Timer
    {
    public:
        //==============================================================================================================
        PluginEditor(PluginInstance&);
        ~PluginEditor() override;

    private:
        //==============================================================================================================
        void paint(juce::Graphics&) override;
        void resized() override;
        void timerCallback() override;

        //==============================================================================================================
        PluginInstance& pluginInstance;
        jump::LookAndFeel jumpLookAndFeel;

        LevelMeters levelMetersDemo;

        //==============================================================================================================
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginEditor)
    };
} // namespace jumpDemo
