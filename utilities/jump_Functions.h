#pragma once

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    namespace Math
    {
        //==============================================================================================================
        /** Returns the value that's the given proportion between the start and end values. */
        template <typename T>
        inline T linearSpace(T start, T end, float proportion)
        {
            jassert(end >= start);
            jassert(proportion >= 0 && proportion <= 1);

            return start + (end - start) * proportion;
        }

        /** Returns the value that's the given proportion between the start and end values on a logarithmic scale
            (base 2).
        */
        template <typename T>
        inline T logSpace(T start, T end, float proportion)
        {
            jassert(start > static_cast<T>(0));
            jassert(end >= start);
            jassert(proportion >= 0.f && proportion <= 1.f);

            return start * std::pow(end / start, static_cast<T>(proportion));
        }

        /** Returns the proportion between the start and end values that the given value lies on a logarithmic scale
            (base 2)
        */
        template <typename T>
        inline float inverseLogSpace(T start, T end, T value)
        {
            jassert(start > static_cast<T>(0));
            jassert(end >= start);
            jassert(value >= start && value <= end);
            jassert(std::log(end / start) != static_cast<T>(0));

            return std::log(value / start) / std::log(end / start);
        }

        /** Returns a specified number of values evenly spaced between start and end. */
        template <typename T>
        inline std::vector<T> linearRange(T start, T end, int count)
        {
            jassert(end >= start);
            jassert(count >= 2);

            std::vector<T> result(count);

            for (auto i = 0; i < count; i++)
            {
                const auto proportion = i / static_cast<float>(count - 1);
                result[i] = linearSpace(start, end, proportion);
            }

            return result;
        }

        /** Returns a specified number of values logarithmically spaced between start and end. */
        template <typename T>
        inline std::vector<T> logRange(T start, T end, int count)
        {
            jassert(start > static_cast<T>(0));
            jassert(end >= start);
            jassert(count >= 2);

            std::vector<T> result(count);

            for (auto i = 0; i < count; i++)
            {
                const auto proportion = i / static_cast<float>(count - 1);
                result[i] = logSpace(start, end, proportion);
            }

            return result;
        }
    }   // namespace Math

    //==================================================================================================================
    /** Applies hold and decay easing to the given peak level. */
    inline float applyEnvelopeToDecibelLevel(float peakLevelDB, juce::uint32 timeOfPeak, juce::uint32 now,
                                             float holdTime, float maxHoldTime, float decayTime,
                                             const juce::NormalisableRange<float>& decibelRange)
    {
        const auto ellapsedTime = static_cast<float>(now - timeOfPeak);
        auto multiplier = 1.f;

        if (!(ellapsedTime < holdTime || holdTime >= maxHoldTime))
            multiplier = juce::jlimit(0.f, 1.f, 1.f - (ellapsedTime - holdTime) / decayTime);

        return ((peakLevelDB - decibelRange.start) * multiplier) + decibelRange.start;
    }
}   // namespace jump
