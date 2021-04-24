#include "jumpDemo_PluginInstance.h"

#include <editor/jumpDemo_PluginEditor.h>

//======================================================================================================================
namespace jumpDemo
{
    //==================================================================================================================
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
    {
        std::vector<std::unique_ptr<juce::AudioProcessorParameterGroup>> groups;
        return { groups.begin(), groups.end() };
    }

    //==================================================================================================================
    PluginInstance::PluginInstance()
        : jump::PluginProcessor{
                BusesProperties{}.withInput("Stereo Input",  juce::AudioChannelSet::stereo())
                                 .withOutput("Stereo Output", juce::AudioChannelSet::stereo()),
                createParameterLayout(),
                audioProcessor
    }
    {
    }

    //==================================================================================================================
    void writeToTransferManagers(juce::AudioBuffer<float>& buffer,
                                 jump::AudioTransferManager<float, 1024>& transferManagerLeft,
                                 jump::AudioTransferManager<float, 1024>& transferManagerRight,
                                 jump::AudioTransferManager<float, 1024>& transferManagerMono)
    {
        const auto numSamples = buffer.getNumSamples();
        const auto numChannels = buffer.getNumChannels();

        transferManagerLeft.write(buffer.getReadPointer(0), numSamples);

        if (numChannels > 1)
        {
            transferManagerRight.write(buffer.getReadPointer(1), numSamples);

            for (auto i = 0; i < numSamples; i++)
            {
                auto mono = 0.f;

                for (auto channel = 0; channel < numChannels; channel++)
                    mono += buffer.getReadPointer(channel)[i];

                mono /= static_cast<float>(numChannels);
                transferManagerMono.write(mono);
            }
        }
        else
        {
            transferManagerMono.write(buffer.getReadPointer(0), numSamples);
        }
    }

    void PluginInstance::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiBuffer)
    {
        writeToTransferManagers(buffer, inputTransferManagerLeft, inputTransferManagerRight, inputTransferManagerMono);
        jump::PluginProcessor::processBlock(buffer, midiBuffer);
        writeToTransferManagers(buffer, outputTransferManagerLeft, outputTransferManagerRight, outputTransferManagerMono);
    }

    bool PluginInstance::isBusesLayoutSupported(const BusesLayout& /*layouts*/) const
    {
        return true;
    }

    juce::AudioProcessorEditor* PluginInstance::createEditor()
    {
        return new PluginEditor{ *this };
    }

    //==================================================================================================================
    std::array<std::vector<float>, 2> PluginInstance::getLatestInputSamples() const noexcept
    {
        return {
            inputTransferManagerLeft.read(),
            inputTransferManagerRight.read()
        };
    }

    std::array<std::vector<float>, 2> PluginInstance::getLatestOutputSamples() const noexcept
    {
        return {
            outputTransferManagerLeft.read(),
            outputTransferManagerRight.read()
        };
    }

    std::vector<float> PluginInstance::getLatestInputSamplesMono() const noexcept
    {
        return inputTransferManagerMono.read();
    }

    std::vector<float> PluginInstance::getLatestOutputSamplesMono() const noexcept
    {
        return outputTransferManagerMono.read();
    }
}   // namespace jumpDemo

//======================================================================================================================
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new jumpDemo::PluginInstance{};
}
