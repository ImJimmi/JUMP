#pragma once

#ifndef JucePlugin_Name
    #define JucePlugin_Name "Unnamed Plugin"
#endif

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    /** A base class for the main audio processing class in a plugin.

        This class provides a default implementation for some of the pure virtual methods provided by
        juce::AudioProcessor that seldom differ across projects and adds the additional features required to have
        parameters controlled by a juce::AudioProcessorValueTreeState object.
    */
    class PluginProcessor : public juce::AudioProcessor
    {
    public:
        //==============================================================================================================
        PluginProcessor(juce::dsp::ProcessorBase& mainAudioProcessor,
                        const BusesProperties& busesProperties = BusesProperties{}.withInput("Stereo Input",
                                                                                             juce::AudioChannelSet::stereo())
                                                                     .withOutput("Stereo Output",
                                                                                 juce::AudioChannelSet::stereo()));

        //==============================================================================================================
        void prepareToPlay(double sampleRate, int blockSize) override;
        void numChannelsChanged() override;
        void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& /*midiBuffer*/) override;
        void releaseResources() override;

        //==============================================================================================================
        const juce::String getName() const override;
        double getTailLengthSeconds() const override;
        bool acceptsMidi() const override;
        bool producesMidi() const override;
        bool hasEditor() const override;
        int getNumPrograms() override;
        int getCurrentProgram() override;
        void setCurrentProgram(int) override;
        const juce::String getProgramName(int) override;
        void changeProgramName(int, const juce::String&) override;

        void getStateInformation(juce::MemoryBlock& destData) override;
        void setStateInformation(const void* data, int sizeInBytes) override;

    private:
        //==============================================================================================================
        juce::dsp::ProcessorBase& audioProcessor;
    };
} // namespace jump
