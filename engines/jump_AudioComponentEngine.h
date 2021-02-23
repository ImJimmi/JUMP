#pragma once

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    /** Base class for audio component engines.

        The derived engines should be used to implement the logic for a specific type of audio visualiser such as a
        spectrum analyser or level meter.

        @tparam SampleType  The data type of samples that will be added via addSamples().
        @tparam desiredFPS  The frequency at which the update() method should be called. The default is 60Hz.
    */
    template <typename SampleType, int desiredFPS = 60>
    class AudioComponentEngine  :   private juce::Timer
    {
    public:
        //==============================================================================================================
        AudioComponentEngine()
        {
            startTimerHz(desiredFPS);
        }

        ~AudioComponentEngine()
        {
            static_assert(std::is_trivially_copyable<SampleType>::value,
                        "'SampleType' for jump::AudioComponentEngine should be "
                        "trivially copyable.");
        }

        //==============================================================================================================
        /** Writes a stream of samples to the sample buffer.

            This should be called regularly with sequential blocks of audio samples, preferably from an
            AudioTransportManager to ensure thread safety.

            @param samples  The block of samples to write to the buffer.
        */
        virtual void addSamples(const std::vector<SampleType>& samples) = 0;

        //==============================================================================================================
        /** Returns a ValueTree that represents the current state of this
            engine.

            @param nodeName The name that should be used for the returned node.

            @returns A juce::ValueTree to represent this engine.
        */
        virtual juce::ValueTree getStateInformation(const juce::String& nodeName) const = 0;

        /** Changes the properties of this engine to those in the given state
            node.

            @param node The node representing the state that this engine should
                        take.
        */
        virtual void setStateInformation(const juce::ValueTree& node) = 0;

    protected:
        //==============================================================================================================
        virtual void update() = 0;

    private:
        //==============================================================================================================
        void timerCallback() override
        {
            update();
        }

        //==============================================================================================================
        CircularBuffer<SampleType> buffer;
    };
}
