#pragma once

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    /** Represents the width and height of a GUI element. */
    template <typename ValueType>
    struct Size
    {
        //==============================================================================================================
        juce::Rectangle<ValueType> toJuceRectangle(const juce::Point<ValueType>& position = {}) const noexcept
        {
            return { position.x, position.y, width, height };
        }

        juce::Point<ValueType> toJucePoint() const noexcept
        {
            return { width, height };
        }

        static Size fromJuceRectangle(const juce::Rectangle<ValueType>& rectangle)
        {
            return { rectangle.getWidth(), rectangle.getHeight() };
        }

        static Size fromJucePoint(const juce::Point<ValueType>& point)
        {
            return { point.x, point.y };
        }

        //==============================================================================================================
        ValueType width;
        ValueType height;
    };
}   // namespace jump
