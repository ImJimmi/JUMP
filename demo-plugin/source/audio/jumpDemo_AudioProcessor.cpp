#include "jumpDemo_AudioProcessor.h"

//======================================================================================================================
namespace jumpDemo
{
    //==================================================================================================================
    AudioProcessor::AudioProcessor(const jump::PluginProcessor& /*pluginInstance*/)
    {
    }

    //==================================================================================================================
    void AudioProcessor::prepare(const juce::dsp::ProcessSpec& /*spec*/)
    {
    }

    void AudioProcessor::process(const juce::dsp::ProcessContextReplacing<float>& /*context*/)
    {
    }

    void AudioProcessor::reset()
    {
    }
}   // namespace jumpDemo
