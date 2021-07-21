#pragma once

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    class AttributedLabel : public Canvas
    {
    public:
        //==============================================================================================================
        using Attribute = juce::AttributedString::Attribute;
        using ReadingDirection = juce::AttributedString::ReadingDirection;
        using WordWrap = juce::AttributedString::WordWrap;

        //==============================================================================================================
        AttributedLabel() = default;
        explicit AttributedLabel(const juce::String& initialText);

        //==============================================================================================================
        void paint(juce::Graphics& g) override;

        //==============================================================================================================
        void append(const juce::String& text);
        void append(const juce::String& text, const juce::Font& font);
        void append(const juce::String& text, const juce::Colour& colour);
        void append(const juce::String& text, const juce::Font& font, const juce::Colour& colour);
        void append(const juce::AttributedString& other);

        void clear();

        //==============================================================================================================
        void setText(const juce::String& text);
        const juce::String& getText() const noexcept;

        void setBorderSize(const juce::BorderSize<int>& borderSize);
        const juce::BorderSize<int>& getBorderSize() const;

        juce::Justification getJustification() const noexcept;
        void setJustification(juce::Justification newJustification) noexcept;

        WordWrap getWordWrap() const noexcept;
        void setWordWrap(WordWrap newWordWrap) noexcept;

        ReadingDirection getReadingDirection() const noexcept;
        void setReadingDirection(ReadingDirection newReadingDirection) noexcept;

        float getLineSpacing() const noexcept;
        void setLineSpacing(float newLineSpacing) noexcept;

        int getNumAttributes() const noexcept;
        const Attribute& getAttribute(int index) const noexcept;

        void setColour(const juce::Range<int>& range, const juce::Colour& colour);
        void setFont(const juce::Range<int>& range, const juce::Font& font);
        void setFont(const juce::Font& font);

        int getMinimumRequiredWidth() const;

    private:
        //==============================================================================================================
        juce::AttributedString m_attributedString;
        juce::BorderSize<int> m_borderSize{ 1, 5, 1, 5 };

        //==============================================================================================================
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AttributedLabel)
    };
} // namespace jump
