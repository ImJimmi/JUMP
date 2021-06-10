#pragma once

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    class LevelMeterLabelsComponent : public Container
    {
    public:
        //==============================================================================================================
        struct LookAndFeelMethods
        {
            virtual ~LookAndFeelMethods() = default;

            virtual std::unique_ptr<juce::Label> createLabelForLevel(const LevelMeterLabelsComponent& component,
                                                                     float level) const noexcept = 0;
        };

        //==============================================================================================================
        LevelMeterLabelsComponent(const LevelMeterEngine& engineToUse)
            : engine{ engineToUse }
        {
            lookAndFeel.attachTo(this);
            lookAndFeel.onValidLookAndFeelFound = [this]() {
                updateLabels();
            };
        }

        ~LevelMeterLabelsComponent() override = default;

        //==============================================================================================================
        int getMinimumRequiredWidthWhenVertical() const
        {
            auto result = 0;

            for (const auto& label : labels)
            {
                const auto textWidth = static_cast<int>(std::ceil(label->getFont().getStringWidthFloat(label->getText())));
                const auto xPadding = label->getBorderSize().getLeftAndRight();
                const auto totalWidth = textWidth + xPadding;

                if (totalWidth > result)
                    result = totalWidth;
            }

            return result;
        }

        int getMinimumRequiredHeightWhenHorizontal() const
        {
            auto result = 0;

            for (const auto& label : labels)
            {
                const auto fontHeight = static_cast<int>(std::ceil(label->getFont().getHeight()));

                if (fontHeight > result)
                    result = fontHeight;
            }

            return result;
        }

        void setHighlightedLevels(const std::vector<float>& newHighlightedLevels)
        {
            levels = newHighlightedLevels;

            updateLabels();
        }

        void setJustification(juce::Justification justificationType)
        {
            justification = justificationType;

            updateLabels();
        }

        void setOrientation(Orientation newOrientation)
        {
            orientation = newOrientation;
            resized();
        }

        const LevelMeterEngine& getEngine() const noexcept
        {
            return engine;
        }

    private:
        //==============================================================================================================
        void resized() override
        {
            if (getWidth() <= 0 || getHeight() <= 0)
                return;

            const auto& range = engine.getDecibelRange();

            for (auto& label : labels)
            {
                const auto level = static_cast<float>(label->getProperties()[levelPropertyId]);
                auto normalisedLevel = normaliseDecibelsTo0To1(level, range);

                if (orientation == Orientation::vertical)
                {
                    const auto y = (1.f - normalisedLevel) * getHeight();

                    const auto labelHeight = juce::jmin(static_cast<int>(std::ceil(label->getFont().getHeight())), getHeight());
                    const auto labelY = juce::jlimit(0, getHeight() - labelHeight, juce::roundToInt(y - labelHeight / 2.f));

                    label->setBounds(0, labelY, getWidth(), labelHeight);
                }
                else if (orientation == Orientation::horizontal)
                {
                    const auto x = normalisedLevel * getWidth();

                    const auto textWidth = static_cast<int>(std::ceil(label->getFont().getStringWidthFloat(label->getText())));
                    const auto xPadding = label->getBorderSize().getLeftAndRight();
                    const auto labelWidth = textWidth + xPadding;
                    const auto labelX = juce::jlimit(0, getWidth() - labelWidth, juce::roundToInt(x - labelWidth / 2.f));

                    label->setBounds(labelX, 0, labelWidth, getHeight());
                }
                else
                {
                    // Unhandled orientation.
                    jassertfalse;
                }
            }
        }

        void lookAndFeelChanged() override
        {
            updateLabels();
        }

        //==============================================================================================================
        void updateLabels()
        {
            if (!lookAndFeel)
                return;

            labels.clear();

            for (const auto& level : levels)
            {
                auto label = lookAndFeel->createLabelForLevel(*this, level);
                label->getProperties().set(levelPropertyId, level);
                label->setJustificationType(justification);
                addAndMakeVisible(*label);

                labels.add(std::move(label));
            }

            resized();
        }

        //==============================================================================================================
        const LevelMeterEngine& engine;
        juce::OwnedArray<juce::Label> labels;
        std::vector<float> levels;
        juce::Justification justification{ juce::Justification::left };
        Orientation orientation{ Orientation::vertical };
        static inline const juce::Identifier levelPropertyId{ "level" };

        LookAndFeelAccessor<LookAndFeelMethods> lookAndFeel;

        //==============================================================================================================
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LevelMeterLabelsComponent)
    };
} // namespace jump
