#pragma once

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    class SpectrumAnalyserLabelsComponent :   public Container,
                                              private StatefulObject
    {
    public:
        //==============================================================================================================
        struct LookAndFeelMethods
        {
            virtual ~LookAndFeelMethods() = default;

            virtual std::unique_ptr<juce::Label> createLabelForLevel(const SpectrumAnalyserLabelsComponent& component,
                                                                     float level) const noexcept = 0;
            virtual std::unique_ptr<juce::Label> createLabelForFrequency(const SpectrumAnalyserLabelsComponent& component,
                                                                         float frequency) const noexcept = 0;
        };

        struct PropertyIDs
        {
            static const inline juce::Identifier highlightedLevelsPropertyId     { "highlightedLevels" };
            static const inline juce::Identifier highlightedFrequenciesPropertyId{ "highlightedFrequencies" };
        };

        //==============================================================================================================
        SpectrumAnalyserLabelsComponent(const SpectrumAnalyserEngine& engineToUse,
                                        const juce::Identifier& uniqueID, StatefulObject* parentState = nullptr)
            :   StatefulObject{ uniqueID, parentState },
                engine{ engineToUse }
        {
            initialiseState();

            lookAndFeel.attachTo(this);
            lookAndFeel.onValidLookAndFeelFound = [this]() {
                updateLevelLabels();
                updateFrequencyLabels();
            };
        }

        ~SpectrumAnalyserLabelsComponent() override = default;

        //==============================================================================================================
        void setHighlightedLevels(const std::vector<float>& newHighlightedLevels)
        {
            setProperty(PropertyIDs::highlightedLevelsPropertyId, var_cast<std::vector<float>>(newHighlightedLevels));
        }

        void setHighlightedFrequencies(const std::vector<float>& newHighlightedFrequencies)
        {
            setProperty(PropertyIDs::highlightedFrequenciesPropertyId,
                        var_cast<std::vector<float>>(newHighlightedFrequencies));
        }

        const SpectrumAnalyserEngine& getEngine() const noexcept
        {
            return engine;
        }

    private:
        //==============================================================================================================
        void resized() override
        {
            positionLevelLabels();
            positionFrequencyLabels();
        }

        void lookAndFeelChanged() override
        {
            updateLevelLabels();
            updateFrequencyLabels();
        }

        void propertyChanged(const juce::Identifier& name, const juce::var& newValue) override
        {
            if (name == PropertyIDs::highlightedLevelsPropertyId)
                setHighlightedLevelsInternal(var_cast<std::vector<float>>(newValue));
            else if (name == PropertyIDs::highlightedFrequenciesPropertyId)
                setHighlightedFrequenciesInternal(var_cast<std::vector<float>>(newValue));
        }

        //==============================================================================================================
        void updateLevelLabels()
        {
            if (!lookAndFeel)
                return;

            levelLabels.clear();

            for (const auto& level : levels)
            {

                auto label = lookAndFeel->createLabelForLevel(*this, level);
                label->getProperties().set(levelPropertyId, level);
                addAndMakeVisible(*label);

                levelLabels.add(std::move(label));
            }

            positionLevelLabels();
        }

        void positionLevelLabels()
        {
            if (!isVisible() || getWidth() <= 0 || getHeight() <= 0)
                return;

            const auto& decibelRange = engine.getDecibelRange();

            for (auto& label : levelLabels)
            {
                const auto level = static_cast<float>(label->getProperties()[levelPropertyId]);
                const auto normalisedLevel = normaliseDecibelsTo0To1(level, decibelRange);
                const auto y = (1.f - normalisedLevel) * getHeight();
                const auto labelHeight = getMinimumRequiredHeightForLabel(*label);
                const auto labelY = juce::jlimit(0, getHeight() - labelHeight, juce::roundToInt(y - labelHeight / 2.f));
                const auto labelWidth = getMinimumWidthRequiredForLabel(*label);

                label->setBounds(0, labelY, labelWidth, labelHeight);
            }
        }

        void updateFrequencyLabels()
        {
            if (!lookAndFeel)
                return;

            frequencyLabels.clear();

            for (const auto& freq : frequencies)
            {
                auto label = lookAndFeel->createLabelForFrequency(*this, freq);
                label->getProperties().set(frequencyPropertyId, freq);
                addAndMakeVisible(*label);

                frequencyLabels.add(std::move(label));
            }

            positionFrequencyLabels();
        }

        void positionFrequencyLabels()
        {
            if (!isVisible() || getWidth() <= 0 || getHeight() <= 0)
                return;
                
            const auto& freqRange = engine.getFrequencyRange();

            for (auto& label : frequencyLabels)
            {
                const auto freq = static_cast<float>(label->getProperties()[frequencyPropertyId]);
                const auto normalisedLevel = math::inverseLogSpace(freqRange.start, freqRange.end, freq);
                const auto x = normalisedLevel * getWidth();
                const auto labelWidth = getMinimumWidthRequiredForLabel(*label);
                const auto labelX = juce::jlimit(0, getWidth() - labelWidth, juce::roundToInt(x - labelWidth / 2.f));
                const auto labelHeight = getMinimumRequiredHeightForLabel(*label);

                label->setBounds(labelX, getHeight() - labelHeight, labelWidth, labelHeight);
            }
        }

        void initialiseState()
        {
            setProperty(PropertyIDs::highlightedLevelsPropertyId,
                        var_cast<std::vector<float>>({ -12.f, -24.f, -36.f, -48.f, -60.f, -72.f, -84.f, -96.f }));
            setProperty(PropertyIDs::highlightedFrequenciesPropertyId,
                        var_cast<std::vector<float>>({ 50.f, 100.f, 200.f, 500.f, 1000.f, 2000.f, 5000.f, 10000.f }));
        }

        //==============================================================================================================
        void setHighlightedLevelsInternal(const std::vector<float>& newHighlightedLevels)
        {
            levels = newHighlightedLevels;
            updateLevelLabels();
        }

        void setHighlightedFrequenciesInternal(const std::vector<float>& newHighlightedFrequencies)
        {
            frequencies = newHighlightedFrequencies;
            updateFrequencyLabels();
        }

        //==============================================================================================================
        const SpectrumAnalyserEngine& engine;
        juce::OwnedArray<juce::Label> levelLabels;
        juce::OwnedArray<juce::Label> frequencyLabels;
        std::vector<float> levels;
        std::vector<float> frequencies;
        static inline const juce::Identifier levelPropertyId{ "level" };
        static inline const juce::Identifier frequencyPropertyId{ "frequency" };

        LookAndFeelAccessor<LookAndFeelMethods> lookAndFeel;

        //==============================================================================================================
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpectrumAnalyserLabelsComponent)
    };
}   // namespace jump
