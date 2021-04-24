#pragma once

#include <audio/jumpDemo_AudioProcessor.h>
#include <JuceHeader.h>

//======================================================================================================================
namespace jumpDemo
{
    //==================================================================================================================
    class PluginInstance    :   public jump::PluginProcessor
    {
    public:
        //==============================================================================================================
        PluginInstance();

        //==============================================================================================================
        void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiBuffer) override;

        bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
        juce::AudioProcessorEditor* createEditor() override;

        //==============================================================================================================
        std::array<std::vector<float>, 2> getLatestInputSamples() const noexcept;
        std::array<std::vector<float>, 2> getLatestOutputSamples() const noexcept;
        std::vector<float> getLatestInputSamplesMono() const noexcept;
        std::vector<float> getLatestOutputSamplesMono() const noexcept;

    private:
        //==============================================================================================================
        jumpDemo::AudioProcessor audioProcessor{ *this };

        jump::AudioTransferManager<float, 1024> inputTransferManagerLeft;
        jump::AudioTransferManager<float, 1024> inputTransferManagerRight;
        jump::AudioTransferManager<float, 1024> inputTransferManagerMono;
        jump::AudioTransferManager<float, 1024> outputTransferManagerLeft;
        jump::AudioTransferManager<float, 1024> outputTransferManagerRight;
        jump::AudioTransferManager<float, 1024> outputTransferManagerMono;

        //==============================================================================================================
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginInstance)
    };
}   // namespace jumpDemo
