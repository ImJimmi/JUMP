#pragma once

//======================================================================================================================
namespace juce
{
    //==================================================================================================================
    template <>
    struct VariantConverter<juce::dsp::WindowingFunction<float>::WindowingMethod>
    {
        //==============================================================================================================
        using WindowingMethod = juce::dsp::WindowingFunction<float>::WindowingMethod;

        //==============================================================================================================
        static WindowingMethod fromVar(const juce::var& v)
        {
            return static_cast<WindowingMethod>(static_cast<int>(v));
        }

        static juce::var toVar(const WindowingMethod& method)
        {
            return { static_cast<int>(method) };
        }
    };

    //==================================================================================================================
    template <>
    struct VariantConverter<std::vector<float>>
    {
        //==============================================================================================================
        static std::vector<float> fromVar(const juce::var& v)
        {
            const auto value = juce::JSON::parse(v.toString());

            jassert(value.isArray());

            std::vector<float> result(value.size());

            for (auto i = 0; i < value.size(); i++)
                result[i] = value[i];

            return result;
        }

        static juce::var toVar(const std::vector<float>& values)
        {
            juce::var result;
            result.resize(static_cast<int>(values.size()));

            for (auto i = 0; i < values.size(); i++)
                result[i] = values[i];

            return juce::JSON::toString(result, true);
        }
    };

    //==================================================================================================================
    template <>
    struct VariantConverter<juce::NormalisableRange<float>>
    {
        //==============================================================================================================
        static juce::NormalisableRange<float> fromVar(const juce::var& v)
        {
            auto value = juce::JSON::fromString(v.toString());

            jassert(value.isArray());
            jassert(value.size() == 2 || value.size() == 3);

            const auto min = static_cast<float>(value[0]);
            const auto max = static_cast<float>(value[1]);
            const auto skew = value.size() == 3 ? static_cast<float>(value[2]) : 1.f;
            juce::NormalisableRange<float> range{ min, max };
            range.skew = skew;
            return range;
        }

        static juce::var toVar(const juce::NormalisableRange<float>& range)
        {
            const auto value = juce::var{ juce::Array<juce::var>{ range.start, range.end, range.skew } };
            return juce::JSON::toString(value, true);
        }
    };
}   // namespace juce