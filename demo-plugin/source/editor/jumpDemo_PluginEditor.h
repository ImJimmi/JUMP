#pragma once

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
        void resized() override;
        void timerCallback() override;

        //==============================================================================================================
        PluginInstance& pluginInstance;
        jump::LookAndFeel jumpLookAndFeel;
        jump::Canvas backgroundCanvas;

        juce::ComboBox demoSelector;
        std::unique_ptr<juce::Component> activeDemo;

        //==============================================================================================================
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginEditor)
    };
} // namespace jumpDemo
