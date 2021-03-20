#pragma once

//======================================================================================================================
namespace jump::LevelMeter
{
    //==================================================================================================================
    class LabelsComponent   :   public juce::Component,
                                private LookAndFeelAccessorListener
    {
    public:
        //==============================================================================================================
        struct LookAndFeelMethods
        {
            virtual ~LookAndFeelMethods() = default;

            virtual juce::String getLevelMeterTextForLevel(float decibelLevel, bool isNegativeInf) const = 0;
            virtual juce::Font getLevelMeterTextFont(float decibelLevel, bool isNegativeInf) const = 0;
            virtual juce::Colour getLevelMeterTextColour(float decibelLevel, bool isNegativeInf) const = 0;
        };

        //==============================================================================================================
        LabelsComponent(const Engine& engineToUse)
            :   engine{ engineToUse }
        {
            lookAndFeel.addListener(this);
        }

        ~LabelsComponent()
        {
            lookAndFeel.removeListener(this);
        }

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
                const auto fontHeight = label->getFont().getHeight();

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

        void setOrientation(LevelMeter::Orientation newOrientation)
        {
            orientation = newOrientation;
            resized();
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
                const auto level = static_cast<float>(label->getProperties()[valuePropertyId]);
                auto normalisedLevel = normaliseDecibelsTo0To1(level, range);

                if (orientation == Orientation::Vertical)
                {
                    const auto y = (1.f - normalisedLevel) * getHeight();

                    const auto labelHeight = static_cast<int>(std::ceil(label->getFont().getHeight()));
                    const auto labelY = juce::jlimit(0, getHeight() - labelHeight, juce::roundToInt(y - labelHeight / 2.f));

                    label->setBounds(0, labelY, getWidth(), labelHeight);
                }
                else if (orientation == Orientation::Horizontal)
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

        void validLookAndFeelInUse() override
        {
            for (auto& label : labels)
            {
                const auto level = static_cast<float>(label->getProperties()[valuePropertyId]);
                const auto isNegativeInf = level <= engine.getDecibelRange().start;
                const auto text = lookAndFeel->getLevelMeterTextForLevel(level, isNegativeInf);
                label->setText(text, juce::dontSendNotification);

                const auto font = lookAndFeel->getLevelMeterTextFont(level, isNegativeInf);
                label->setFont(font);

                const auto colour = lookAndFeel->getLevelMeterTextColour(level, isNegativeInf);
                label->setColour(juce::Label::textColourId, colour);
            }
        }

        //==============================================================================================================
        void updateLabels()
        {
            labels.clear();

            for (const auto& level : levels)
            {
                juce::String text;
                juce::Font font;
                juce::Colour colour;

                if (lookAndFeel)
                {
                    const auto isNegativeInf = level <= engine.getDecibelRange().start;
                    text = lookAndFeel->getLevelMeterTextForLevel(level, isNegativeInf);
                    font = lookAndFeel->getLevelMeterTextFont(level, isNegativeInf);
                    colour = lookAndFeel->getLevelMeterTextColour(level, isNegativeInf);
                }

                auto label = std::make_unique<juce::Label>("", text);
                label->setFont(font);
                label->getProperties().set(valuePropertyId, level);
                label->setColour(juce::Label::textColourId, colour);
                label->setJustificationType(justification);
                label->setBorderSize(juce::BorderSize<int>{ 0 });
                addAndMakeVisible(*label);

                labels.add(std::move(label));
            }

            resized();
        }

        //==============================================================================================================
        const Engine& engine;
        juce::OwnedArray<juce::Label> labels;
        std::vector<float> levels;
        juce::Justification justification{ juce::Justification::left };
        LevelMeter::Orientation orientation{ LevelMeter::Orientation::Vertical };
        static inline const juce::Identifier valuePropertyId{ "value" };

        LookAndFeelAccessor<LookAndFeelMethods> lookAndFeel{ *this };
    };
}   // namespace jump::LevelMeter
