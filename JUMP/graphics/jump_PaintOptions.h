#pragma once

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    class PaintOptions
    {
    public:
        //==============================================================================================================
        enum Flags
        {
            strokeFlag = 1 << 0,
            fillFlag = 1 << 1,

            strokeAndFill = strokeFlag + fillFlag
        };

        //==============================================================================================================
        explicit PaintOptions(juce::int64 flags)
            : options{ flags }
        {
        }

        //==============================================================================================================
        bool getShouldStroke() const noexcept
        {
            return (options & strokeFlag) != 0;
        }

        bool getShouldFill() const noexcept
        {
            return (options & fillFlag) != 0;
        }

        juce::int64 serialise() const noexcept
        {
            return options;
        }

    private:
        //==============================================================================================================
        juce::int64 options;
    };
} // namespace jump
