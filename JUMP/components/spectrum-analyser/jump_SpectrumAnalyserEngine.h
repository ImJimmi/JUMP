#pragma once

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    class SpectrumAnalyserEngine;

    //==================================================================================================================
    struct SpectrumAnalyserRendererBase
    {
        //==============================================================================================================
        virtual ~SpectrumAnalyserRendererBase() = default;

        //==============================================================================================================
        /** Derived classes must override this method in order to receive callbacks when a new set of points had been
            calculated by the given engine.
        */
        virtual void newSpectrumAnalyserPointsAvailable(const SpectrumAnalyserEngine& engine,
                                                        const std::vector<juce::Point<float>>& points) = 0;
    };

    //==================================================================================================================
    /** Implements the logic required for a spectrum analyser.

        Given a stream of samples, this class calculates points that can be used to plot a spectrum analyser. The
        resultant points are normalised so they need only be scaled with the desired width and height in order to
        display an accurate representation of the stream's frequency content.

        The Y axis of the analyser uses Decibels and will be linear.
        The X axis of the analyser is in Hz and will use a logarithmic scale (base 2) to more accurately represent how
        we as humans perceive pitch.
    */
    class SpectrumAnalyserEngine : public AudioComponentEngine<SpectrumAnalyserRendererBase>
    {
    public:
        //==============================================================================================================
        struct PropertyIDs
        {
            static const inline juce::Identifier windowingMethodId{ "windowingMethod" };
            static const inline juce::Identifier fftOrderId{ "fftOrder" };
            static const inline juce::Identifier frequencyRangeId{ "frequencyRange" };
            static const inline juce::Identifier decibelRangeId{ "decibelRange" };
            static const inline juce::Identifier holdTimeId{ "holdTime" };
            static const inline juce::Identifier maxHoldTimeId{ "maxHoldTime" };
            static const inline juce::Identifier decayTimeId{ "decayTime" };
            static const inline juce::Identifier numPointsId{ "numPoints" };
        };

        //==============================================================================================================
        using WindowingMethod = juce::dsp::WindowingFunction<float>::WindowingMethod;

        //==============================================================================================================
        SpectrumAnalyserEngine();
        SpectrumAnalyserEngine(const juce::Identifier& uniqueID, StatefulObject* parentState);

        //==============================================================================================================
        void addSamples(const std::vector<float>& samples) override;

        //==============================================================================================================
        /** Specifies the sample rate of the samples being added to this engine.

            The default is 0Hz (so a real value must be set before adding samples).

            @param newSampleRate    The new sample rate to use.
        */
        void setSampleRate(double newSampleRate);

        /** Specifies the windowing method to use with the FFT.

            The default is a hann window.

            @param newWindowingMethod   The new windowing method to use.
        */
        void setWindowingMethod(WindowingMethod newWindowingMethod);

        /** Changes the size of the FFT to 2 ^ newFFTOrder.

            The nyquist frequency is required to update the range of FFT bins that are used in rendering so
            setSampleRate() must be called at least once before calling this.

            @param newFFTOrder  The new order to use for the FFT.
        */
        void setFFTOrder(int newFFTOrder);

        /** Changes the range of frequencies for which the points will be calculated.

            The lower value should be greater than 0, and the upper value should be no greater than the nyquist limit.

            The default is 20Hz to 20kHz.

            @param newFrequencyRange    The new frequency range to use.
        */
        void setFrequencyRange(const juce::NormalisableRange<float>& newFrequencyRange);

        /** Returns the current frequency range being used by this engine. */
        const juce::NormalisableRange<float>& getFrequencyRange() const noexcept;

        /** Changes the decibel range of the analyser.

            The start of the range will be treated as -inf Decibels and will be normalised to a value of 0 while the end
            of the range will be normalised to a value of 1.

            The default is -100dB to 0dB.

            @param newDecibelRange  The new decibel range to use.
        */
        void setDecibelRange(const juce::NormalisableRange<float>& newDecibelRange);

        /** Returns the current decibel range being used by this engine. */
        const juce::NormalisableRange<float>& getDecibelRange() const noexcept;

        /** Changes the time, in milliseconds, for which peaks are held before they start to decay.

            This value should be positive and no greater than the maxium hold time (for which the default is 1000ms). If
            the hold time is equal to the maximum hold time, peaks will never decay.

            This value is rounded to the nearest millisecond.

            The default is 100ms;

            @param newHoldTimeMs    The new hold time to use, in milliseconds.
        */
        void setHoldTime(float newHoldTimeMs);

        /** Changes the maximum value for the hold time.

            If the hold time is euqal to the maximum hold time then peaks will never decay.

            This value is rounded to the nearest millisecond.

            The default is 1000ms;

            @param newMaxHoldTimeMs The new maximum hold time to use, in
                                    milliseconds.
        */
        void setMaxHoldTime(float newMaxHoldTimeMs);

        /** Changes the time in which peaks will decay to -inf Decibels.

            This value should be positive.

            The default is 500ms;

            @param newDecayTimeMs   The new decay time to use, in milliseconds.
        */
        void setDecayTime(float newDecayTimeMs);

        /** Changes the number of points to calculate for the spectrum.

            Note that this is the maximum number of points as some points may be removed if they share the same
            frequency bin as an existing point (i.e. at low frequencies).

            The default is 256.

            @param newNumPoints The new number of points to calculate.
        */
        void setNumPoints(int newNumPoints);

        /** Returns the current sample rate being used by this engine. */
        double getNyquistFrequency() const noexcept;

    private:
        //==============================================================================================================
        class AnalyserPointInfo
        {
        public:
            AnalyserPointInfo(int fftBinIndex, float frequency, const juce::NormalisableRange<float>& freqRange);

            void update(const std::vector<float>& fftData, int fftSize,
                        const juce::NormalisableRange<float>& decibelRange, juce::uint32 now, float holdTime,
                        float maxHoldTime, float decayTime);

            juce::Point<float> normalise(const juce::NormalisableRange<float>& decibelRange);

            const int binIndex;

        private:
            const float normalisedX;

            float dB{ std::numeric_limits<float>::lowest() };

            juce::uint32 timeOfLatestPeak{ 0 };
            float levelOfLatestPeak{ std::numeric_limits<float>::lowest() };
        };

        //==============================================================================================================
        void update(juce::uint32 now) override;
        void propertyChanged(const juce::Identifier& name, const juce::var& newValue) override;

        //==============================================================================================================
        void initialise();
        void updateBinRange();

        //==============================================================================================================
        void setFFTOrderInternal(int newFFTOrder);
        void setSampleRateInternal(double newSampleRate);
        void setFrequencyRangeInternal(const juce::NormalisableRange<float>& newFrequencyRange);

        //==============================================================================================================
        CircularBuffer<float> buffer;

        std::unique_ptr<juce::dsp::FFT> fft;
        juce::dsp::WindowingFunction<float>::WindowingMethod windowingMethod;
        std::unique_ptr<juce::dsp::WindowingFunction<float>> windowingFunction;
        juce::Range<int> binRange;

        std::vector<AnalyserPointInfo> pointsInfo;

        float nyquistFrequency{ 0.f };
        juce::NormalisableRange<float> frequencyRange;
        juce::NormalisableRange<float> decibelRange;
        float holdTime{ 0.f };
        float maxHoldTime{ 0.f };
        float decayTime{ 0.f };
        int numPoints{ 0 };

        //==============================================================================================================
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpectrumAnalyserEngine)
    };
} // namespace jump
