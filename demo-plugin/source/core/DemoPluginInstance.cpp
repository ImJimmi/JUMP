#include "DemoPluginInstance.h"

#include <editor/DemoPluginEditor.h>

//======================================================================================================================
juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
{
    std::vector<std::unique_ptr<juce::AudioProcessorParameterGroup>> groups;
    return { groups.begin(), groups.end() };
}

//======================================================================================================================
DemoPluginInstance::DemoPluginInstance()
    :   jump::PluginProcessor{
            BusesProperties{}.withInput ("Stereo Input",  juce::AudioChannelSet::stereo())
                             .withOutput("Stereo Output", juce::AudioChannelSet::stereo()),
            createParameterLayout(),
            audioProcessor
        }
{
}

//======================================================================================================================
void DemoPluginInstance::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiBuffer)
{
    const auto numSamples = buffer.getNumSamples();
    const auto numChannels = buffer.getNumChannels();

    inputTransferManagerLeft.write(buffer.getReadPointer(0), numSamples);

    if (numChannels > 1)
        inputTransferManagerRight.write(buffer.getReadPointer(1), numSamples);

    jump::PluginProcessor::processBlock(buffer, midiBuffer);

    outputTransferManagerLeft.write(buffer.getReadPointer(0), numSamples);

    if (numChannels > 1)
        outputTransferManagerRight.write(buffer.getReadPointer(1), numSamples);
}

bool DemoPluginInstance::isBusesLayoutSupported(const BusesLayout& /*layouts*/) const
{
    return true;
}

juce::AudioProcessorEditor* DemoPluginInstance::createEditor()
{
    return new DemoPluginEditor{ *this };
}

//======================================================================================================================
std::array<std::vector<float>, 2> DemoPluginInstance::getLatestInputSamples() const
{
    return {
        inputTransferManagerLeft.read(),
        inputTransferManagerRight.read()
    };
}

std::array<std::vector<float>, 2> DemoPluginInstance::getLatestOutputSamples() const
{
    return {
        outputTransferManagerLeft.read(),
        outputTransferManagerRight.read()
    };
}

//======================================================================================================================
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DemoPluginInstance{};
}
