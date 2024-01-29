#pragma once

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    template <typename ValueType = float>
    class Level
    {
    public:
        //==============================================================================================================
        Level() = default;

        Level(const Level& other)
            : gain{ other.gain }
            , decibels{ other.decibels }
        {
        }

        Level(Level&& other)
            : gain{ other.gain }
            , decibels{ other.decibels }
        {
        }

        ~Level()
        {
            static_assert(std::is_trivially_copyable<ValueType>::value,
                          "`ValueType` for jump::Level<ValueType> must be trivially copyable!");
        }

        //==============================================================================================================
        ValueType toGain() const noexcept
        {
            return gain;
        }

        ValueType toDecibels() const noexcept
        {
            return decibels;
        }

        //==============================================================================================================
        static Level fromGain(ValueType gain, ValueType minusInfDB = static_cast<ValueType>(jump::defaultMinusInfDB))
        {
            Level level;
            level.gain = gain;
            level.decibels = juce::Decibels::gainToDecibels(gain, minusInfDB);

            return level;
        }

        static Level fromDecibels(ValueType dB,
                                  ValueType minusInfDB = static_cast<ValueType>(jump::defaultMinusInfDB))
        {
            Level level;
            level.decibels = dB;
            level.gain = juce::Decibels::decibelsToGain(dB, minusInfDB);

            return level;
        }

        //==============================================================================================================
        Level& operator=(const Level& other)
        {
            gain = other.gain;
            decibels = other.decibels;

            return *this;
        }

        bool operator==(const Level& other) const
        {
            return juce::approximatelyEqual(gain, other.gain);
        }

        bool operator!=(const Level& other) const
        {
            return !(*this == other);
        }

        bool operator>(const Level& other) const
        {
            return gain > other.gain;
        }

        bool operator>=(const Level& other) const
        {
            return gain >= other.gain;
        }

        bool operator<(const Level& other) const
        {
            return gain < other.gain;
        }

        bool operator<=(const Level& other) const
        {
            return gain <= other.gain;
        }

    private:
        //==============================================================================================================
        ValueType gain{ static_cast<ValueType>(0) };
        ValueType decibels{ static_cast<ValueType>(jump::defaultMinusInfDB) };
    };
} // namespace jump
