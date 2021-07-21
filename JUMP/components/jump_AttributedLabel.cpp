#include "jump_AttributedLabel.h"

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    AttributedLabel::AttributedLabel(const juce::String& initialText)
        : m_attributedString{ initialText }
    {
    }

    //==================================================================================================================
    void AttributedLabel::paint(juce::Graphics& g)
    {
        const auto bounds = m_borderSize.subtractedFrom(getLocalBounds());
        m_attributedString.draw(g, bounds.toFloat());
    }

    //==================================================================================================================
    void AttributedLabel::append(const juce::String& text)
    {
        m_attributedString.append(text);
    }

    void AttributedLabel::append(const juce::String& text, const juce::Font& font)
    {
        m_attributedString.append(text, font);
    }

    void AttributedLabel::append(const juce::String& text, const juce::Colour& colour)
    {
        m_attributedString.append(text, colour);
    }

    void AttributedLabel::append(const juce::String& text, const juce::Font& font, const juce::Colour& colour)
    {
        m_attributedString.append(text, font, colour);
    }

    void AttributedLabel::append(const juce::AttributedString& other)
    {
        m_attributedString.append(other);
    }

    void AttributedLabel::clear()
    {
        m_attributedString.clear();
    }

    //==================================================================================================================
    void AttributedLabel::setText(const juce::String& text)
    {
        m_attributedString.setText(text);
    }

    const juce::String& AttributedLabel::getText() const noexcept
    {
        return m_attributedString.getText();
    }

    void AttributedLabel::setBorderSize(const juce::BorderSize<int>& borderSize)
    {
        m_borderSize = borderSize;
    }

    const juce::BorderSize<int>& AttributedLabel::getBorderSize() const
    {
        return m_borderSize;
    }

    juce::Justification AttributedLabel::getJustification() const noexcept
    {
        return m_attributedString.getJustification();
    }

    void AttributedLabel::setJustification(juce::Justification newJustification) noexcept
    {
        m_attributedString.setJustification(newJustification);
    }

    AttributedLabel::WordWrap AttributedLabel::getWordWrap() const noexcept
    {
        return m_attributedString.getWordWrap();
    }

    void AttributedLabel::setWordWrap(WordWrap newWordWrap) noexcept
    {
        m_attributedString.setWordWrap(newWordWrap);
    }

    AttributedLabel::ReadingDirection AttributedLabel::getReadingDirection() const noexcept
    {
        return m_attributedString.getReadingDirection();
    }

    void AttributedLabel::setReadingDirection(ReadingDirection newReadingDirection) noexcept
    {
        m_attributedString.setReadingDirection(newReadingDirection);
    }

    float AttributedLabel::getLineSpacing() const noexcept
    {
        return m_attributedString.getLineSpacing();
    }

    void AttributedLabel::setLineSpacing(float newLineSpacing) noexcept
    {
        m_attributedString.setLineSpacing(newLineSpacing);
    }

    int AttributedLabel::getNumAttributes() const noexcept
    {
        return m_attributedString.getNumAttributes();
    }

    const AttributedLabel::Attribute& AttributedLabel::getAttribute(int index) const noexcept
    {
        return m_attributedString.getAttribute(index);
    }

    void AttributedLabel::setColour(const juce::Range<int>& range, const juce::Colour& colour)
    {
        m_attributedString.setColour(range, colour);
    }

    void AttributedLabel::setFont(const juce::Range<int>& range, const juce::Font& font)
    {
        m_attributedString.setFont(range, font);
    }

    void AttributedLabel::setFont(const juce::Font& font)
    {
        m_attributedString.setFont(font);
    }

    int getMinimumRequiredWidthForAttribute(const AttributedLabel::Attribute& attribute, const juce::String& text)
    {
        const auto subText = text.substring(attribute.range.getStart(), attribute.range.getEnd());
        return juce::roundToInt(std::ceil(attribute.font.getStringWidthFloat(subText)));
    }

    int AttributedLabel::getMinimumRequiredWidth() const
    {
        auto width = m_borderSize.getLeftAndRight();
        const auto& text = getText();

        for (auto i = 0; i < getNumAttributes(); i++)
        {
            const auto& attribute = getAttribute(i);
            width += getMinimumRequiredWidthForAttribute(attribute, text);
        }

        return width;
    }
} // namespace jump
