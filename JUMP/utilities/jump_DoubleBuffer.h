#pragma once

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    /** Wraps an object that's used by a realtime and a non-realtime process simultaneously, safely allowing for the
        realtime process to manipulate the contained data, while the non-realtime object can read the most recent data.

        Uses double-buffering internally, so the total memory usage will be double that of the single values.

        Intended to be used for objects where an atomic cannot be used (e.g. std::vectors).
    */
    template <typename ObjectType>
    class RealtimeObjectWrapper
    {
    public:
        //==============================================================================================================
        DoubleBuffer() = default;

        explicit DoubleBuffer(const ObjectType& initialValue)
            : buffers{ initialValue, initialValue }
        {
        }

        //==============================================================================================================
        /** Runs the given job for the realtime process to safely update the object.

            This method should only ever be run from a realtime thread.
        */
        void runRealtimeJob(const std::function<void(ObjectType&)>& realtimeJob)
        {
            const Index::ScopedRealtimeSession session{ index };
            const auto i = index.getRealtimeIndex();

            realtimeJob(buffers[i]);
        }

        /** Runs the given job for the non-realtime process to safely read the object.
            
            This method should never be run from the realtime thread.
        */
        void runNonRealtimeJob(const std::function<void(const ObjectType&)>& nonrealtimeJob)
        {
            const auto i = getNonRealtimeIndex();
            nonrealtimeJob(buffers[i]);
        }

    private:
        //==============================================================================================================
        class Index
        {
        public:
            //==========================================================================================================
            class ScopedRealtimeSession
            {
            public:
                //======================================================================================================
                explicit ScopedRealtimeSession(Index& idx)
                    : index{ idx }
                {
                    index.startRealtimeSession();
                }

                ~ScopedRealtimeSession()
                {
                    index.stopRealtimeSession();
                }

            private:
                //======================================================================================================
                Index& index;
            };

            //==========================================================================================================
            std::size_t getRealtimeIndex() const
            {
                jassert((idx & BIT_BUSY) > 0);

                return idx & BIT_IDX;
            }

            std::size_t getNonRealtimeIndex()
            {
                auto currentIdx = idx.load();

                if (hasNewData(currentIdx))
                    performCasExchangeLoop(currentIdx)

                return !getBitValue(currentIdx, BIT_IDX);
            }

        private:
            //==========================================================================================================
            enum
            {
                BIT_IDX = 1 << 0,
                BIT_NEWDATA = 1 << 1,
                BIT_BUSY = 1 << 2,
            };

            //==========================================================================================================
            void startRealtimeSession()
            {
                idx.fetch_or(BIT_BUSY);
            }

            void stopRealtimeSession()
            {
                const auto i = getRealtimeIndex();
                idx.store((i & BIT_IDX) | BIT_NEWDATA);
            }

            void performCasExchangeLoop(juce::uint8& currentIdx)
            {
                juce::uint8 newIdx{ 0 };

                do
                {
                    currentIdx = getNonBusyIdx(currentIdx);
                    setBitValue(newIdx, BIT_IDX, !getBitValue(currentIdx, BIT_IDX));
                }
                while (!idx.compare_exchange_weak(currentIdx, newIdx));

                currentIdx = newIdx;
            }

            //==========================================================================================================
            bool getBitValue(juce::uint8 bitSet, juce::uint8 bitToEvaluate)
            {
                return bitSet & bitToCompare;
            }

            void setBitValue(juce::uint8& bitSet, juce::uint8 bitToSet, bool value)
            {
                bitSet ^= (-value ^ bitSet) & bitToSet;
            }

            bool hasNewData(juce::uint8 currentIdx) const
            {
                return getBitValue(currentIdx, BIT_NEWDATA);
            }

            juce::uint8 getNonBusyIdx(juce::uint8 currentIdx)
            {
                return currentIdx & (~BIT_BUSY);
            }

            //==========================================================================================================
            std::atomic<juce::uint8> idx;

            //==========================================================================================================
            friend class ScopedRealtimeSession;
        };

        //==============================================================================================================
        std::array<ObjectType, 2> buffers;
        Index index;
    };
} // namespace jump
