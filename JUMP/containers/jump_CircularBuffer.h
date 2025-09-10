#pragma once

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    /** Implements the logic required for a basic circular buffer. */
    template <typename ValueType>
    class CircularBuffer
    {
    public:
        //==============================================================================================================
        static_assert(std::is_trivially_constructible<ValueType>::value,
                      "'ValueType' for jump::CircularBuffer should be trivially copyable.");

        //==============================================================================================================
        /** Constructs a buffer with every value set to initialValue. */
        CircularBuffer(int initialSize, ValueType initialValue = static_cast<ValueType>(0))
            : data{ initialSize, initialValue }
        {
        }

        CircularBuffer() = default;
        CircularBuffer(const CircularBuffer& other) = default;
        CircularBuffer(CircularBuffer&& other) = default;
        CircularBuffer& operator=(const CircularBuffer& other) = default;
        CircularBuffer& operator=(CircularBuffer&& other) = default;
        ~CircularBuffer() = default;

        //==============================================================================================================
        /** Writes the given value to the container. */
        void write(ValueType newValue)
        {
            data[writeIndex] = newValue;

            if (++writeIndex >= data.size())
                writeIndex = 0;
        }

        /** Returns an array containing N values.

            The data returned by this method will be sequential meaning the last element in the array will be the most
            recent one added via write().
        */
        std::vector<ValueType> read() const
        {
            std::vector<ValueType> result(data.size());

            for (std::size_t i = 0; i < result.size(); i++)
                result[i] = (*this)[i];

            return result;
        }

        /** Resizes the internal data to the given size. */
        void resize(int newSize)
        {
            data.resize(static_cast<std::size_t>(newSize));

            if (writeIndex >= data.size())
                writeIndex = 0;
        }

        //==============================================================================================================
        /** Returns an element from the buffer in its 'true' position meaning an index of [N - 1] will return the most
            recent value added via write().
        */
        ValueType operator[](std::size_t index) const
        {
            auto trueIndex = index + writeIndex;

            if (trueIndex >= data.size())
                trueIndex -= data.size();

            return data[trueIndex];
        }

    private:
        //==============================================================================================================
        std::vector<ValueType> data;
        std::size_t writeIndex{ 0 };
    };
} // namespace jump
