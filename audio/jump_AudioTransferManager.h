#pragma once

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    /** Manages the transfer of a stream of audio samples from a real-time thread to a non real-time thread (e.g.
        getting samples from an audio thread to a GUI thread).

        This container only stores a single stream of samples so for multichannel applications a separate ATM is
        required for each channel.

        This class aims be thread safe via to the use of double buffering.

        @tparam SampleType  The data type of samples to use (usually float or double).
        @tparam capacity    The maximum number of samples allowed in each of the two internal buffers (so the total
                            number of samples held by this container could be anything up to capacity * 2).
                            You'll want to make sure the capacity is large enough to store every new sample that comes
                            in between calls to read(). An easy way of calculating this is to divide the maximum
                            expected sample rate by the rate at which you're reading. For example for a sample rate of
                            192kHz and reading with a timer set to 60Hz, the capacity will need to be at least 3200. If
                            you don't do this, you'll start losing more samples the higher your sample rate and the
                            lower your reading rate which may affect the accuracy of your audio visualisers.
    */
    template <typename SampleType, int capacity>
    class AudioTransferManager
    {
    public:
        //==============================================================================================================
        /** Default constructor.

            Creates an empty container.
        */
        AudioTransferManager()
        {
        }

        /** Copy constructor.

            Copies state and values from other.

            @param other    The other ATM object to copy from.
        */
        AudioTransferManager(const AudioTransferManager& other)
            :   buffers(other.buffers),
                idx(other.idx.load())
        {
        }

        ~AudioTransferManager()
        {
            static_assert(std::is_trivially_copyable<SampleType>::value,
                          "'SampleType' for AudioTransferManager must be "
                          "trivially copyable.");
        }

        //==============================================================================================================
        /** Writes a single sample to the container.

            If the number of samples written to the container since it was last read from reaches capacity, old, unread
            samples will be overwritten by this method. For that reason you should try to make sure that capacity is
            suitably large enough for no samples to ever be lost while a reader is active.

            @param sample   The sample to write to the container.
        */
        void write(SampleType sample)
        {
            const auto writeIndex = idx.fetch_or(DataFlags::Busy) & DataFlags::IDX;
            buffers[writeIndex].write(sample);
            idx.store((writeIndex & DataFlags::IDX) | DataFlags::NewData);
        }

        /** Returns a vector containing all new samples added to this container since it was last read from (or, if it
            hasn't been read from yet, since it was created).

            Note that the size of the vector will vary and is not guaranteed to be equal to the capacity.

            @returns    A vector containing the latest samples.
        */
        std::vector<SampleType> read()
        {
            // Get the current state of the idx.
            auto currentIDX = idx.load();

            // Check if new data has been added since the last read.
            if ((currentIDX & DataFlags::NewData) != 0)
            {
                auto newValue = -1;

                // CAS loop.
                // Keep looping until the idx is no longer busy.
                do
                {
                    currentIDX &= ~DataFlags::Busy;
                    newValue = (currentIDX ^ DataFlags::IDX) & DataFlags::IDX;
                } while (!idx.compare_exchange_weak(currentIDX, newValue));

                // Something went wrong here - the value never got changed!
                assert(newValue != -1);

                currentIDX = newValue;
            }

            const auto readIndex = (currentIDX & DataFlags::IDX) ^ 1;
            return buffers[readIndex].read();
        }

        /** Returns the capacity of this container. */
        constexpr int getCapacity() const noexcept
        {
            return capacity;
        }

    private:
        //==============================================================================================================
        /** Implements the logic for reading and writing from the buffers using a fixed-size array and an incrementing
            write index.
        */
        struct BufferWrapper
        {
            void write(SampleType sample)
            {
                if (writeIndex >= capacity)
                    writeIndex = 0;

                buffer[writeIndex++] = sample;
            }

            std::vector<SampleType> read()
            {
                std::vector<SampleType> result(writeIndex);
                std::copy(buffer.begin(), buffer.begin() + writeIndex, result.begin());

                writeIndex = 0;

                return result;
            }

            int writeIndex = 0;
            std::array<SampleType, capacity> buffer;
        };

        //==============================================================================================================
        std::array<BufferWrapper, 2> buffers;

        enum DataFlags
        {
            IDX     = 1 << 0,
            NewData = 1 << 1,
            Busy    = 1 << 2
        };

        std::atomic<int> idx{ 0 };
    };
}   // namespace jump
