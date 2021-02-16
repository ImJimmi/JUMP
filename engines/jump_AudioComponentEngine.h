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
        /** Default constructor.

            Initialises the engine with an empty sample buffer.
        */
        AudioComponentEngine()
        {
        }

        /** Initialisation constructor.

            Initialises the engine by given the sample buffer an initial size and an initial value (which defaults to 0)
            for each element in the buffer.
        */
        AudioComponentEngine(int initialBufferSize, SampleType initialValue = static_cast<SampleType>(0))
            :   buffer(initialBufferSize, initialValue)
        {
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
        void addSamples(const std::vector<SampleType>& samples)
        {
            for (auto& sample : samples)
                buffer.write(sample);

            startTimerHz(desiredFPS);
        }

        //==============================================================================================================
        /** Changes the size of the internal sample buffer.

            @param newSize  The new number of samples the buffer should hold.
        */
        virtual void setBufferSize(int newSize)
        {
            buffer.resize(newSize);
        }

        /** Engines should override this method to provide a way to serialise
            their settings.
        */
        virtual juce::ValueTree getStateInformation(const juce::String& nodeName) const = 0;

        /** Engines should override this method to be able load settings from
            the given ValueTree node.
        */
        virtual void setStateInformation(const juce::ValueTree& node) = 0;

    protected:
        //==============================================================================================================
        /** Returns a reference to the sample buffer. */
        const CircularBuffer<SampleType>& getSampleBuffer() const
        {
            return buffer;
        }

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
