#pragma once

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    /** A base class for the main audio processing class in a plugin.

        This class provides a default implementation for some of the pure virtual methods provided by
        juce::AudioProcessor that seldom differ across projects and adds the additional features required to have
        parameters controlled by a juce::AudioProcessorValueTreeState object.
    */
    class PluginProcessor   :   public juce::AudioProcessor
    {
    public:
        //==============================================================================================================
        explicit PluginProcessor(const BusesProperties& busesProperties,
                                 juce::AudioProcessorValueTreeState::ParameterLayout parametersLayout,
                                 juce::dsp::ProcessorBase& mainAudioProcessor,
                                 const juce::Identifier& apvtsID = juce::String{ JucePlugin_Name }.replace(" ", "_"))
            :   juce::AudioProcessor{ busesProperties },
                apvts{ *this, &undoManager, apvtsID, std::move(parametersLayout) },
                audioProcessor{ mainAudioProcessor }
        {
        }

        //==============================================================================================================
        void prepareToPlay(double /*sampleRate*/, int /*blockSize*/) override
        {
            prepareAudioProcessor();
        }

        void numChannelsChanged() override
        {
            prepareAudioProcessor();
        }

        void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& /*midiBuffer*/) override
        {
            juce::ScopedNoDenormals noDenormals;

            juce::dsp::AudioBlock<float> block{ buffer };
            juce::dsp::ProcessContextReplacing<float> context{ block };
            audioProcessor.process(context);
        }

        void releaseResources() override
        {
            audioProcessor.reset();
        }

        //==============================================================================================================
        virtual const juce::String getName() const override
        {
            return JucePlugin_Name;
        }

        double getTailLengthSeconds() const override
        {
            return 0.0;
        }

        bool acceptsMidi() const override
        {
            return static_cast<bool>(JucePlugin_WantsMidiInput);
        }

        bool producesMidi() const override
        {
            return static_cast<bool>(JucePlugin_ProducesMidiOutput);
        }

        bool hasEditor() const override
        {
            return true;
        }

        int getNumPrograms() override
        {
            return 1;
        }

        int getCurrentProgram() override
        {
            return 0;
        }

        void setCurrentProgram(int) override
        {
        }

        const juce::String getProgramName(int) override
        {
            return {};
        }

        void changeProgramName(int, const juce::String&) override
        {
        }

        //==============================================================================================================
        template <typename ParameterType>
        ParameterType& getParameter(juce::StringRef parameterID) const noexcept
        {
            const auto& param{ apvts.getParameter(parameterID) };

            // No parameter with the given name was found.
            jassert(param != nullptr);

            return *dynamic_cast<ParameterType*>(param);
        }

    protected:
        //==============================================================================================================
        void getStateInformation(juce::MemoryBlock& destData) override
        {
            if (const auto xml = apvts.copyState().createXml())
                copyXmlToBinary(*xml, destData);
        }

        void setStateInformation(const void* data, int sizeInBytes) override
        {
            if (const auto xml{ getXmlFromBinary(data, sizeInBytes) })
            {
                if (xml->hasTagName(apvts.state.getType()))
                    apvts.replaceState(juce::ValueTree::fromXml(*xml));
            }
        }

    private:
        //==============================================================================================================
        void prepareAudioProcessor()
        {
            juce::dsp::ProcessSpec spec;
            spec.sampleRate = getSampleRate();
            spec.maximumBlockSize = getBlockSize();
            spec.numChannels = juce::jmax(getTotalNumInputChannels(), getTotalNumOutputChannels());

            audioProcessor.prepare(spec);
        }

        //==============================================================================================================
        /** Should be called by the editor to set the ValueTree object to use for the GUI's state information.

            If no matching element is found in the APVTS's state, the given ValueTree will be added as a new child. If
            an existing element is found then the given ValueTree will be set to reference the existing one.
        */
        void setGUIState(juce::ValueTree& valueTreeToUseForGUIState)
        {
            if (auto existingState = apvts.state.getChildWithName(valueTreeToUseForGUIState.getType()); existingState.isValid())
                valueTreeToUseForGUIState = existingState;
            else
                apvts.state.addChild(valueTreeToUseForGUIState, -1, nullptr);
        }

        //==============================================================================================================
        juce::AudioProcessorValueTreeState apvts;
        juce::UndoManager undoManager;

        juce::dsp::ProcessorBase& audioProcessor;

        friend class PluginEditor;
    };
}   // namespace jump
