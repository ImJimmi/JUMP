#include "jump_PluginProcessor.h"


#ifndef JucePlugin_WantsMidiInput
    #define JucePlugin_WantsMidiInput false
#endif

#ifndef JucePlugin_ProducesMidiOutput
    #define JucePlugin_ProducesMidiOutput false
#endif

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    PluginProcessor::PluginProcessor(juce::dsp::ProcessorBase& mainAudioProcessor,
                                     const BusesProperties& busesProperties)
        : juce::AudioProcessor{ busesProperties }
        , audioProcessor{ mainAudioProcessor }
    {
    }

    //==================================================================================================================
    void prepareAudioProcessor(juce::dsp::ProcessorBase& audioProcessor,
                               double sampleRate, juce::uint32 blockSize, juce::uint32 numChannels)
    {
        juce::dsp::ProcessSpec spec;
        spec.sampleRate = sampleRate;
        spec.maximumBlockSize = blockSize;
        spec.numChannels = numChannels;

        audioProcessor.prepare(spec);
    }

    void PluginProcessor::prepareToPlay(double, int)
    {
        prepareAudioProcessor(audioProcessor,
                              getSampleRate(),
                              static_cast<juce::uint32>(getBlockSize()),
                              static_cast<juce::uint32>(juce::jmax(getTotalNumInputChannels(),
                                                                   getTotalNumOutputChannels())));
    }

    void PluginProcessor::numChannelsChanged()
    {
        prepareAudioProcessor(audioProcessor,
                              getSampleRate(),
                              static_cast<juce::uint32>(getBlockSize()),
                              static_cast<juce::uint32>(juce::jmax(getTotalNumInputChannels(),
                                                                   getTotalNumOutputChannels())));
    }

    void PluginProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
    {
        juce::ScopedNoDenormals noDenormals;

        juce::dsp::AudioBlock<float> block{ buffer };
        juce::dsp::ProcessContextReplacing<float> context{ block };
        audioProcessor.process(context);
    }

    void PluginProcessor::releaseResources()
    {
        audioProcessor.reset();
    }

    //==================================================================================================================
    const juce::String PluginProcessor::getName() const
    {
        return JucePlugin_Name;
    }

    double PluginProcessor::getTailLengthSeconds() const
    {
        return 0.0;
    }

    bool PluginProcessor::acceptsMidi() const
    {
        return static_cast<bool>(JucePlugin_WantsMidiInput);
    }

    bool PluginProcessor::producesMidi() const
    {
        return static_cast<bool>(JucePlugin_ProducesMidiOutput);
    }

    bool PluginProcessor::hasEditor() const
    {
        return true;
    }

    int PluginProcessor::getNumPrograms()
    {
        return 1;
    }

    int PluginProcessor::getCurrentProgram()
    {
        return 0;
    }

    void PluginProcessor::setCurrentProgram(int)
    {
    }

    const juce::String PluginProcessor::getProgramName(int)
    {
        return {};
    }

    void PluginProcessor::changeProgramName(int, const juce::String&)
    {
    }

    //==================================================================================================================
    void PluginProcessor::getStateInformation(juce::MemoryBlock&)
    {
    }

    void PluginProcessor::setStateInformation(const void*, int)
    {

    }
} // namespace jump
