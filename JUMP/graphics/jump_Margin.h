#pragma once

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    /** A generic Margin object that can be converted to various JUCE types.

        A Margin object can be constructed in a similar way to a CSS margin, and can then be converted to a
        juce::BorderSize, juce::GridItem::Margin, or juce::FlexItem::Margin.
    */
    template <typename ValueType>
    class Margin
    {
    public:
        //==============================================================================================================
        Margin() = default;

        Margin(ValueType topEdge, ValueType rightEdge, ValueType bottomEdge, ValueType leftEdge)
            :   top   { topEdge },
                right { rightEdge },
                bottom{ bottomEdge },
                left  { leftEdge }
        {
        }

        Margin(ValueType topEdge, ValueType rightAndLeftEdges, ValueType bottomEdge)
            :   Margin{ topEdge, rightAndLeftEdges, bottomEdge, rightAndLeftEdges }
        {
        }

        Margin(ValueType topAndBottomEdges, ValueType rightAndLeftEdges)
            :   Margin{ topAndBottomEdges, rightAndLeftEdges, topAndBottomEdges }
        {
        }

        explicit Margin(ValueType allEdges)
            :   Margin{ allEdges, allEdges }
        {
        }

        //==============================================================================================================
        template <typename BorderSizeType>
        juce::BorderSize<BorderSizeType> toBorderSize() const noexcept
        {
            return juce::BorderSize<BorderSizeType>{
                static_cast<BorderSizeType>(top), static_cast<BorderSizeType>(left),
                static_cast<BorderSizeType>(bottom), static_cast<BorderSizeType>(right)
            };
        }

        juce::GridItem::Margin toGridMargin() const noexcept
        {
            return {
                static_cast<float>(top), static_cast<float>(left),
                static_cast<float>(bottom), static_cast<float>(right)
            };
        }

        juce::FlexItem::Margin toFlexMargin() const noexcept
        {
            return {
                static_cast<float>(top), static_cast<float>(left),
                static_cast<float>(bottom), static_cast<float>(right)
            };
        }

    private:
        //==============================================================================================================
        ValueType top;
        ValueType right;
        ValueType bottom;
        ValueType left;
    };
}   // namespace jump
