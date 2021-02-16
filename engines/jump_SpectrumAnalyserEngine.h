#pragma once

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    /** Implements the logic required for a spectrum analyser.

        Given a stream of samples, this class calculates points that can be used to plot a spectrum analyser. The
        resultant points are normalised so they need only be scaled with the desired width and height in order to
        display an accurate representation of the stream's frequency content.
        
        The number of points to generate must be specified by the template parameter however it should be noted that
        some points are dropped from the lower end in order to avoid the stepped appearance of the spectrum.

        @tparam numPoints       The number of points to generate (see above).
        @tparam windowingMethod The windowing method to apply to samples before they're passed to the FFT. The default
                                is the hann window.
    */
    template <int numPoints,
              juce::dsp::WindowingFunction<float>::WindowingMethod windowingMethod = juce::dsp::WindowingFunction<float>::hann>
    class SpectrumAnalyserEngine    :   public AudioComponentEngine<float>
    {
    public:
        //==============================================================================================================
        /** Defines the properties used by the state getters and setters.
        
            This avoids the need for hard-coded strings and makes it easier to
            see what properties are available in the state node.
        */
        struct PropertyIDs
        {
            PropertyIDs() = delete;

            static constexpr char frequencyRangeStartId[] = "frequencyRange.start";
            static constexpr char frequencyRangeEndId[]   = "frequencyRange.end";
            static constexpr char decibelRangeStartId[]   = "decibelRange.start";
            static constexpr char decibelRangeEndId[]     = "decibelRange.end";
            static constexpr char holdTimeId[]            = "holdTime";
            static constexpr char maxHoldTimeId[]         = "maxHoldTime";
            static constexpr char decayTimeId[]           = "decayTime";
        };

        //==============================================================================================================
        SpectrumAnalyserEngine()
        {
            previousDBLevels.fill(-1000.f);
        }

        //==============================================================================================================
        /** Changes the size of the FFT to 2 ^ newFFTOrder.

            The nyquist frequency is required to update the range of FFT bins that are used in rendering so
            setSampleRate() must be called at least once before calling this.

            @param newFFTOrder  The new order to use for the FFT.
        */
        void setFFTOrder(int newFFTOrder)
        {
            fft.reset(new juce::dsp::FFT(newFFTOrder));
            windowingFunction.reset(new juce::dsp::WindowingFunction<float>(static_cast<std::size_t>(1 << newFFTOrder), windowingMethod));
            setBufferSize(1 << newFFTOrder);

            // You need to call setSampleRate first before calling setFFTOrder.
            jassert(nyquistFrequency > 0.f);

            updateBinRange();
        }

        /** Specifies the current sample rate being used.

            This is important in order for the frequency of the FFT bins to be accurately calculated.

            @param newSampleRate    The new sample rate being used.
        */
        void setSampleRate(double newSampleRate)
        {
            jassert(newSampleRate > 0);

            nyquistFrequency = static_cast<float>(newSampleRate) / 2.f;
            updateBinRange();
        }

        /** Changes the range of frequencies for which the points will be calculated.

            The lower value should be greater than 0, and the upper value should be no greater than the nyquist limit.

            The default is 20Hz to 20kHz.

            @param newFrequencyRange    The new frequency range to use.
        */
        void setFrequencyRange(const juce::NormalisableRange<float>& newFrequencyRange)
        {
            jassert(newFrequencyRange.start > 0.f);
            jassert(newFrequencyRange.end <= nyquistFrequency);

            frequencyRange = newFrequencyRange;

            updateBinRange();
        }

        /** Changes the decibel range used to determine points' Y coordinates.

            Bins will almost always have negative dB levels so you'll likely want ot make sure the lower limit is
            negative although there's no actual restrictions on the range of decibels that can be used.

            The default is -100dB to 0dB.

            @param newDecibalRange  The new decibel range to use.
        */
        void setDecibelRange(const juce::NormalisableRange<float>& newDecibelRange)
        {
            decibelRange = newDecibelRange;
        }

        /** Changes the time, in milliseconds, for which peaks are held before they start to decay.

            This value should be positive and no greater than the maxium hold time (for which the default is 1000ms). If
            the hold time is equal to the maximum hold time, peaks will never decay.

            This value is rounded to the nearest millisecond.

            The default is 100ms;

            @param newHoldTimeMs    The new hold time to use, in milliseconds.
        */
        void setHoldTime(float newHoldTimeMs)
        {
            jassert(newHoldTimeMs >= 0.f);
            jassert(newHoldTimeMs <= maxHoldTime);

            holdTime = static_cast<juce::uint32>(juce::roundToInt(newHoldTimeMs));
        }

        /** Changes the maximum value for the hold time.

            If the hold time is euqal to the maximum hold time then peaks will never decay.

            This value is rounded to the nearest millisecond.

            The default is 1000ms;

            @param newMaxHoldTimeMs The new maximum hold time to use, in
                                    milliseconds.
        */
        void setMaxHoldTime(float newMaxHoldTimeMs)
        {
            jassert(newMaxHoldTimeMs >= 0.f);
            jassert(holdTime <= newMaxHoldTimeMs);

            maxHoldTime = static_cast<juce::uint32>(juce::roundToInt(newMaxHoldTimeMs));
        }

        /** Changes the time in which peaks will decay to -inf Decibels.

            This value should be positive.

            The default is 500ms;

            @param newDecayTimeMs   The new decay time to use, in milliseconds.
        */
        void setDecayTime(float newDecayTimeMs)
        {
            decayTime = newDecayTimeMs;
        }

        juce::ValueTree getStateInformation(const juce::String& nodeName) const override
        {
            return {
                nodeName,
                {
                    { PropertyIDs::frequencyRangeStartId, frequencyRange.start },
                    { PropertyIDs::frequencyRangeEndId,   frequencyRange.end },
                    { PropertyIDs::decibelRangeStartId,   decibelRange.start },
                    { PropertyIDs::decibelRangeEndId,     decibelRange.end },
                    { PropertyIDs::holdTimeId,            holdTime },
                    { PropertyIDs::maxHoldTimeId,         maxHoldTime },
                    { PropertyIDs::decayTimeId,           decayTime }
                }
            };
        }

        void setStateInformation(const juce::ValueTree& node) override
        {
            frequencyRange.start = static_cast<float>(node[PropertyIDs::frequencyRangeStartId]);
            frequencyRange.end   = static_cast<float>(node[PropertyIDs::frequencyRangeEndId]);
            decibelRange.start   = static_cast<float>(node[PropertyIDs::decibelRangeStartId]);
            decibelRange.end     = static_cast<float>(node[PropertyIDs::decibelRangeEndId]);

            holdTime    = static_cast<float>(node[PropertyIDs::holdTimeId]);
            maxHoldTime = static_cast<float>(node[PropertyIDs::maxHoldTimeId]);
            decayTime   = static_cast<float>(node[PropertyIDs::decayTimeId]);
        }

        //==============================================================================================================
        /** Components should implement this method to receive the latest set of points every time the engine updates
            them and then use them to draw the analyser.
        */
        std::function<void(const std::vector<juce::Point<float>>&)> onNewPointsAvailable = nullptr;

    private:
        //==============================================================================================================
        /** Holds information about peaks in the spectrum. */
        struct FrequencyPeakData
        {
            juce::uint32 timeOfPeak;    // The time at which the peak occured.
            float dB = 0.f;             // The level of the peak in Decibels.
        };

        //==============================================================================================================
        void update() override
        {
            // If there FFT hasn't been created yet just do nothing.
            if (fft.get() == nullptr)
                return;

            // Get the latest samples from the buffer.
            auto fftData = getSampleBuffer().read();

            // Pad the data with zeros.
            fftData.resize(fft->getSize() * 2, 0.f);

            // Apply the windowing function to the data.
            windowingFunction->multiplyWithWindowingTable(fftData.data(), fftData.size());

            // Process the FFT.
            fft->performFrequencyOnlyForwardTransform(fftData.data());

            auto prevBin = binRange.getStart();
            const auto now = juce::Time::getMillisecondCounter();

            std::vector<juce::Point<float>> points;
            points.reserve(numPoints);

            // Loop through each point and calculate its frequency and dB level.
            for (int i = 0; i < numPoints; i++)
            {
                // Calculate the frequency using a logarithmic scale then
                // calculate the correct bin from the FFT data to use for that
                // frequency.
                const auto proportion = i / static_cast<float>(numPoints - 1);
                const auto frequency = Math::logspace(frequencyRange.start, frequencyRange.end, proportion);
                const auto currentBin = juce::roundToInt(juce::jmap(frequency, frequencyRange.start, frequencyRange.end,
                                                                    static_cast<float>(binRange.getStart()), static_cast<float>(binRange.getEnd())));

                // Calculate the dB level for this bin.
                const auto gain = fftData[currentBin] / (fft->getSize() * 2.f);
                auto dB = juce::Decibels::gainToDecibels(gain, decibelRange.start);

                // If the index of this bin is equal to that of the previously
                // calculated bin don't add a point for it.
                if (prevBin == currentBin && i != 0)
                {
                    previousDBLevels[i] = dB;
                    continue;
                }

                // Check if the new dB level is greater or less than the
                // previous level for this bin.
                if (dB < previousDBLevels[i])
                {
                    // The level was less so apply decay to this bin instead of
                    // using its actual dB level.
                    const auto ellapsedTime = static_cast<float>(now - peaksData[i].timeOfPeak);
                    auto multiplier = 1.f;

                    if (!(ellapsedTime < holdTime || holdTime == maxHoldTime))
                        multiplier = juce::jlimit(0.f, 1.f, 1.f - (ellapsedTime - holdTime) / decayTime);

                    dB = ((peaksData[i].dB - decibelRange.start) * multiplier) + decibelRange.start;
                }
                else
                {
                    // The level was greater so register it as a peak.
                    peaksData[i].dB = dB;
                    peaksData[i].timeOfPeak = now;
                }

                // Add the point whilst normalising the frequency and dB.
                points.push_back({ proportion, 1.f - decibelRange.convertTo0to1(dB) });

                previousDBLevels[i] = dB;
                prevBin = currentBin;
            }

            if (onNewPointsAvailable != nullptr)
                onNewPointsAvailable(points);
        }

        void setBufferSize(int newSize) override
        {
            AudioComponentEngine::setBufferSize(newSize);
        }

        //==============================================================================================================
        /** Updates the range of FFT bins that correspond to the current frequency range and sample rate settings.

            This should be called anytime any of those parameters are updated.
        */
        void updateBinRange()
        {
            // No point in updating the range yet if the FFT hasn't been instantiated.
            if (fft.get() == nullptr)
                return;

            // Find the total number of bins in the useable range (i.e. from 0Hz to the nyquist limit, which is half the
            // size of the FFT since the FFT generates frequencies up to the sample rate).
            const auto totalNumBins = fft->getSize() / 2;

            // Update the range's limits.
            binRange.setStart(juce::roundToInt(totalNumBins * frequencyRange.start / nyquistFrequency));
            binRange.setEnd  (juce::roundToInt(totalNumBins * frequencyRange.end / nyquistFrequency));
        }

        //==============================================================================================================
        std::unique_ptr<juce::dsp::FFT> fft;
        std::unique_ptr<juce::dsp::WindowingFunction<float>> windowingFunction;
        juce::Range<int> binRange;

        std::array<float, numPoints> previousDBLevels;      // Holds the previous dB level for each point.
        std::array<FrequencyPeakData, numPoints> peaksData; // Holds info about peaks.

        // Configurable options (see setter methods).
        float nyquistFrequency = 0.f;
        juce::NormalisableRange<float> frequencyRange{ 20.f, 20000.f };
        juce::NormalisableRange<float> decibelRange{ -100.f, 0.f };
        float holdTime = 100.f;
        float maxHoldTime = 1000.f;
        float decayTime = 500.f;

        //==============================================================================================================
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpectrumAnalyserEngine)
    };
}   // namespace jump
