#pragma once

//======================================================================================================================
namespace jump::LevelMeter
{
    //==================================================================================================================
    enum class Orientation
    {
        Horizontal,
        Vertical
    };
}   // namespace jump::LevelMeter

//======================================================================================================================
namespace juce
{
    //==================================================================================================================
    template <>
    struct VariantConverter<jump::LevelMeter::Orientation>
    {
        //==============================================================================================================
        static jump::LevelMeter::Orientation fromVar(const juce::var& v)
        {
            return static_cast<jump::LevelMeter::Orientation>(static_cast<int>(v));
        }

        static juce::var toVar(const jump::LevelMeter::Orientation& orientation)
        {
            return { static_cast<int>(orientation) };
        }
    };
}   // namespace juce
