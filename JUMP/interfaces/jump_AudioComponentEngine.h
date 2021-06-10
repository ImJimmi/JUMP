#pragma once

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    /** Base class for audio component engines.

        The derived engines should be used to implement the logic for a specific type of audio visualiser such as a
        spectrum analyser or level meter.
    */
    template <typename RendererType>
    class AudioComponentEngine
        : protected StatefulObject
        , private juce::Timer
    {
    public:
        //==============================================================================================================
        AudioComponentEngine() = default;

        AudioComponentEngine(const juce::Identifier& uniqueID, StatefulObject* parentState)
            : StatefulObject{ uniqueID, parentState }
        {
        }

        virtual ~AudioComponentEngine() override = default;

        //==============================================================================================================
        /** Writes a stream of samples to the sample buffer.

            This should be called regularly with sequential blocks of audio samples, preferably from an
            AudioTransportManager to ensure thread safety.

            @param samples  The block of samples to write to the buffer.
        */
        virtual void addSamples(const std::vector<float>& samples) = 0;

        /** Registers a renderer that will receive callbacks when new points are calculated by this engine.

            @param newRenderer  The renderer to add.
        */
        void addRenderer(RendererType* rendererToAdd) const
        {
            renderers.add(rendererToAdd);
        }

        /** Removes a renderer from this engine so that it will no longer receive callbacks.

            @param renderToRemove   The renderer that should be removed.
        */
        void removeRenderer(RendererType* rendererToRemove) const
        {
            renderers.remove(rendererToRemove);
        }

        //==============================================================================================================
        void setFPS(int newFPS)
        {
            stopTimer();
            startTimerHz(newFPS);
        }

    protected:
        //==============================================================================================================
        virtual void update(juce::uint32 now) = 0;

        //==============================================================================================================
        mutable juce::ListenerList<RendererType> renderers;

    private:
        //==============================================================================================================
        struct StartTimerOnCreation
        {
            StartTimerOnCreation(juce::Timer& timerToStart, int timerHz)
            {
                timerToStart.startTimerHz(timerHz);
            }
        };

        //==============================================================================================================
        void timerCallback() override
        {
            const auto now = juce::Time::getMillisecondCounter();
            update(now);
        }

        //==============================================================================================================
        CircularBuffer<float> buffer;
        StartTimerOnCreation timerStarter{ *this, 60 };
    };
} // namespace jump
