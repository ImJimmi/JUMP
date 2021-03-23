#pragma once

#include <JuceHeader.h>

//======================================================================================================================
class DemoPluginAudioProcessor  :   public juce::dsp::ProcessorBase
{
public:
    //==================================================================================================================
    DemoPluginAudioProcessor(const jump::PluginProcessor& pluginInstance);

    //==================================================================================================================
    void prepare(const juce::dsp::ProcessSpec& spec) override;
    void process(const juce::dsp::ProcessContextReplacing<float>& context) override;
    void reset() override;

private:
    //==================================================================================================================
};
