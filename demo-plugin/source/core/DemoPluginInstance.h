#pragma once

#include <audio/DemoPluginAudioProcessor.h>
#include <JuceHeader.h>

//======================================================================================================================
class DemoPluginInstance   :   public jump::PluginProcessor
{
public:
    //==================================================================================================================
    DemoPluginInstance();

    //==================================================================================================================
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiBuffer) override;

    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
    juce::AudioProcessorEditor* createEditor() override;

    //==================================================================================================================
    std::array<std::vector<float>, 2> getLatestInputSamples() const;
    std::array<std::vector<float>, 2> getLatestOutputSamples() const;

private:
    //==================================================================================================================
    DemoPluginAudioProcessor audioProcessor{ *this };

    jump::AudioTransferManager<float, 1024> inputTransferManagerLeft;
    jump::AudioTransferManager<float, 1024> inputTransferManagerRight;
    jump::AudioTransferManager<float, 1024> outputTransferManagerLeft;
    jump::AudioTransferManager<float, 1024> outputTransferManagerRight;

    //==================================================================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DemoPluginInstance)
};
