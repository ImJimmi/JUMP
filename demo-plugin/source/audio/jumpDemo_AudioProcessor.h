#pragma once

#include <JuceHeader.h>

//======================================================================================================================
namespace jumpDemo
{
    //==================================================================================================================
    class AudioProcessor    :   public juce::dsp::ProcessorBase
    {
    public:
        //==============================================================================================================
        AudioProcessor(const jump::PluginProcessor& pluginInstance);

        //==============================================================================================================
        void prepare(const juce::dsp::ProcessSpec& spec) override;
        void process(const juce::dsp::ProcessContextReplacing<float>& context) override;
        void reset() override;

    private:
        //==============================================================================================================
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioProcessor)
    };
}   // namespace jumpDemo
