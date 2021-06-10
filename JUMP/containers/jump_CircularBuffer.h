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
        /** Default constructor.

            Creates the buffer with uninitialised data.
        */
        CircularBuffer()
        {
        }

        /** Constructs a buffer with every value set to initialValue. */
        CircularBuffer(int initialSize, ValueType initialValue = static_cast<ValueType>(0))
            : data{ initialSize, initialValue }
        {
        }

        /** Copy constructor.

            Copies data and write index from other.
        */
        CircularBuffer(const CircularBuffer& other)
            : data{ other.data }
            , writeIndex{ other.writeIndex }
        {
        }

        /** Move constructor.

            Moves data and write index from other.
        */
        CircularBuffer(CircularBuffer&& other)
            : data{ std::move(other.data) }
            , writeIndex{ std::exchange(other.writeIndex) }
        {
        }

        ~CircularBuffer()
        {
            static_assert(std::is_trivially_constructible<ValueType>::value,
                          "'ValueType' for jump::CircularBuffer should be trivially copyable.");
        }

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
        /** Copy assignment - copies data and write index from other. */
        CircularBuffer& operator=(const CircularBuffer& other)
        {
            data = other.data;
            writeIndex = other.writeIndex;

            return *this;
        }

        /** Move assignment - moves data and write index from other. */
        CircularBuffer& operator=(CircularBuffer&& other)
        {
            data = std::move(other.data);
            writeIndex = std::exchange(other.writeIndex);

            return *this;
        }

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
