#include <juce_core/juce_core.h>
#include <jump-tests/jump_TestConstants.h>
#include <JUMP/JUMP.h>

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    bool valueOfLevelIsPersistentAcrossCopies()
    {
        const auto expectedGain = 0.5f;
        const auto originalLevel = jump::Level<float>::fromGain (expectedGain);
        jump::Level<float> constructorCopy{ originalLevel };

        return constructorCopy.toGain() == expectedGain;
    }

    //==================================================================================================================
    class LevelTests : public juce::UnitTest
    {
    public:
        LevelTests()
            : juce::UnitTest{ "jump::Level", testConstants::categoryName }
        {
        }

    private:
        void runTest() override
        {
            testConstructibility();
            testCopyability();
            testMoveability();

            testGainInGainOut();
            testDecibelsInDecibelsOut();
            testGainInDecibelsOut();
            testDecibelsInGainOut();
            testBinaryOperators();
        }

        void testConstructibility()
        {
            beginTest("Constructibility");

            expect(std::is_default_constructible_v<jump::Level<float>>);
            expect(std::is_nothrow_constructible_v<jump::Level<float>>);
            expect(std::is_copy_constructible_v<jump::Level<float>>);
        }

        void testCopyability()
        {
            beginTest("Copyability");

            expect(std::is_copy_assignable_v<jump::Level<float>>);
            expect(valueOfLevelIsPersistentAcrossCopies());
        }

        void testMoveability()
        {
            beginTest("Moveability");

            expect(std::is_move_constructible_v<jump::Level<float>>);
            expect(std::is_move_assignable_v<jump::Level<float>>);
        }

        void testGainInGainOut()
        {
            beginTest("Gain In > Gain Out");

            const auto expectedGain = 0.25f;
            const auto level = jump::Level<float>::fromGain(expectedGain);
            expect(level.toGain() == expectedGain);
        }

        void testDecibelsInDecibelsOut()
        {
            beginTest("Decibels In > Decibels Out");

            const auto expectedDecibels = -4.5f;
            const auto level = jump::Level<float>::fromDecibels(expectedDecibels);
            expect(level.toDecibels() == expectedDecibels);
        }

        void testGainInDecibelsOut()
        {
            beginTest("Gain In > Decibels Out");

            const auto expectedDecibels = -8.2f;
            const auto gain = juce::Decibels::decibelsToGain(expectedDecibels);
            const auto level = jump::Level<float>::fromGain(gain);
            expect(level.toDecibels() == expectedDecibels);
        }

        void testDecibelsInGainOut()
        {
            beginTest("Decibels In > Gain Out");

            const auto expectedGain = 0.83f;
            const auto decibels = juce::Decibels::gainToDecibels(expectedGain);
            const auto level = jump::Level<float>::fromDecibels(decibels);
            expect(level.toGain() == expectedGain);
        }

        void testBinaryOperators()
        {
            beginTest("Binary Operators");

            const auto negative6dB = jump::Level<float>::fromDecibels(-6.f);
            const auto anotherNegative6dB = jump::Level<float>::fromDecibels(-6.f);

            expect(negative6dB == anotherNegative6dB);

            const auto negative12dB = jump::Level<float>::fromDecibels(-12.f);

            expect(negative6dB != negative12dB);

            expect(negative12dB < negative6dB);
            expect(negative6dB > negative12dB);
            expect(negative6dB >= negative6dB && negative6dB >= negative12dB);
            expect(negative12dB <= negative12dB && negative12dB <= negative6dB);
        }
    };
} // namespace jump

//======================================================================================================================
static jump::LevelTests levelTests;
