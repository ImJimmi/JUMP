#include <juce_core/juce_core.h>
#include <jump-tests/jump_TestConstants.h>
#include <JUMP/JUMP.h>

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    class LinearSpaceTests : public juce::UnitTest
    {
    public:
        //==============================================================================================================
        LinearSpaceTests()
            : juce::UnitTest{ "jump::math::linearSpace", testConstants::categoryName }
        {
        }

        //==============================================================================================================
        void runTest() override
        {
            testIntegerInput();
            testUint8Input();
            testFloatInput();
            testDoubleInput();
        }

    private:
        //==============================================================================================================
        void testIntegerInput()
        {
            beginTest("Integer input");

            constexpr auto start = 10;
            constexpr auto end = 20;

            expectEquals(math::linearSpace(start, end, 0.f), 10);
            expectEquals(math::linearSpace(start, end, 0.5f), 15);
            expectEquals(math::linearSpace(start, end, 0.678f), 17);
            expectEquals(math::linearSpace(start, end, 1.f), 20);
        }

        void testUint8Input()
        {
            beginTest("Uint8 input");

            constexpr juce::uint8 start = 22;
            constexpr juce::uint8 end = 222;

            expectEquals(math::linearSpace(start, end, 0.f), static_cast<juce::uint8>(22));
            expectEquals(math::linearSpace(start, end, 0.34567f), static_cast<juce::uint8>(91));
            expectEquals(math::linearSpace(start, end, 0.5f), static_cast<juce::uint8>(122));
            expectEquals(math::linearSpace(start, end, 0.8976f), static_cast<juce::uint8>(202));
            expectEquals(math::linearSpace(start, end, 1.f), static_cast<juce::uint8>(222));
        }

        void testFloatInput()
        {
            beginTest("Float input");

            constexpr auto start = 13.f;
            constexpr auto end = 31.f;

            expectWithinAbsoluteError(math::linearSpace(start, end, 0.f), 13.f, testConstants::floatTolerance);
            expectWithinAbsoluteError(math::linearSpace(start, end, 0.23456f), 17.22208f, testConstants::floatTolerance);
            expectWithinAbsoluteError(math::linearSpace(start, end, 0.5f), 22.f, testConstants::floatTolerance);
            expectWithinAbsoluteError(math::linearSpace(start, end, 0.678f), 25.204f, testConstants::floatTolerance);
            expectWithinAbsoluteError(math::linearSpace(start, end, 1.f), 31.f, testConstants::floatTolerance);
        }

        void testDoubleInput()
        {
            beginTest("Double input");

            constexpr auto start = 13.579;
            constexpr auto end = 24.680;

            expectWithinAbsoluteError(math::linearSpace(start, end, 0.f), 13.579, testConstants::doubleTolerance);
            expectWithinAbsoluteError(math::linearSpace(start, end, 0.13579f), 15.08640479, testConstants::doubleTolerance);
            expectWithinAbsoluteError(math::linearSpace(start, end, 0.5f), 19.1295, testConstants::doubleTolerance);
            expectWithinAbsoluteError(math::linearSpace(start, end, 0.8642f), 23.1724842, testConstants::doubleTolerance);
            expectWithinAbsoluteError(math::linearSpace(start, end, 1.f), 24.680, testConstants::doubleTolerance);
        }
    };

    //==================================================================================================================
    class LogSpaceTests : public juce::UnitTest
    {
    public:
        //==============================================================================================================
        LogSpaceTests()
            : juce::UnitTest{ "jump::math::logSpace", testConstants::categoryName }
        {
        }

        //==============================================================================================================
        void runTest() override
        {
            testIntegerInput();
            testUint8Input();
            testFloatInput();
            testDoubleInput();
        }

    private:
        //==============================================================================================================
        void testIntegerInput()
        {
            beginTest("Integer input");

            constexpr auto start = 10;
            constexpr auto end = 20;

            expectEquals(math::logSpace(start, end, 0.0), 10);
            expectEquals(math::logSpace(start, end, 0.50), 14);
            expectEquals(math::logSpace(start, end, 0.6780), 16);
            expectEquals(math::logSpace(start, end, 1.0), 20);
        }

        void testUint8Input()
        {
            beginTest("Uint8 input");

            constexpr juce::uint8 start = 22;
            constexpr juce::uint8 end = 222;

            expectEquals(math::logSpace(start, end, 0.0), static_cast<juce::uint8>(22));
            expectEquals(math::logSpace(start, end, 0.34567), static_cast<juce::uint8>(49));
            expectEquals(math::logSpace(start, end, 0.5), static_cast<juce::uint8>(70));
            expectEquals(math::logSpace(start, end, 0.8976), static_cast<juce::uint8>(175));
            expectEquals(math::logSpace(start, end, 1.0), static_cast<juce::uint8>(222));
        }

        void testFloatInput()
        {
            beginTest("Float input");

            constexpr auto start = 13.f;
            constexpr auto end = 31.f;

            expectWithinAbsoluteError(math::logSpace(start, end, 0.f), 13.f, testConstants::floatTolerance);
            expectWithinAbsoluteError(math::logSpace(start, end, 0.23456f), 15.93935f, testConstants::floatTolerance);
            expectWithinAbsoluteError(math::logSpace(start, end, 0.5f), 20.07486f, testConstants::floatTolerance);
            expectWithinAbsoluteError(math::logSpace(start, end, 0.678f), 23.433275f, testConstants::floatTolerance);
            expectWithinAbsoluteError(math::logSpace(start, end, 1.f), 31.f, testConstants::floatTolerance);
        }

        void testDoubleInput()
        {
            beginTest("Double input");

            constexpr auto start = 13.579;
            constexpr auto end = 24.680;

            expectWithinAbsoluteError(math::logSpace(start, end, 0.0), 13.579, testConstants::doubleTolerance);
            expectWithinAbsoluteError(math::logSpace(start, end, 0.13579), 14.726590817426743, testConstants::doubleTolerance);
            expectWithinAbsoluteError(math::logSpace(start, end, 0.5), 18.306548555093613, testConstants::doubleTolerance);
            expectWithinAbsoluteError(math::logSpace(start, end, 0.8642), 22.756639460245474, testConstants::doubleTolerance);
            expectWithinAbsoluteError(math::logSpace(start, end, 1.0), 24.680, testConstants::doubleTolerance);
        }
    };

    //==================================================================================================================
    class InverseLogSpaceTests : public juce::UnitTest
    {
    public:
        //==============================================================================================================
        InverseLogSpaceTests()
            : juce::UnitTest{ "jump::math::inverseLogSpace", testConstants::categoryName }
        {
        }

        //==============================================================================================================
        void runTest() override
        {
            testIntegerInput();
            testUint8Input();
            testFloatInput();
            testDoubleInput();
        }

    private:
        //==============================================================================================================
        void testIntegerInput()
        {
            beginTest("Integer input");

            constexpr auto start = 10;
            constexpr auto end = 20;

            expectWithinAbsoluteError(math::inverseLogSpace(start, end, 10), 0.0, testConstants::doubleTolerance);
            expectWithinAbsoluteError(math::inverseLogSpace(start, end, 13), 0.37851162325372983, testConstants::doubleTolerance);
            expectWithinAbsoluteError(math::inverseLogSpace(start, end, 17), 0.765534746362977, testConstants::doubleTolerance);
            expectWithinAbsoluteError(math::inverseLogSpace(start, end, 20), 1.0, testConstants::doubleTolerance);
        }

        void testUint8Input()
        {
            beginTest("Uint8 input");

            constexpr juce::uint8 start = 22;
            constexpr juce::uint8 end = 222;

            expectWithinAbsoluteError(math::inverseLogSpace(start, end, static_cast<juce::uint8>(22)), 0.0, testConstants::doubleTolerance);
            expectWithinAbsoluteError(math::inverseLogSpace(start, end, static_cast<juce::uint8>(33)), 0.17540187817148875, testConstants::doubleTolerance);
            expectWithinAbsoluteError(math::inverseLogSpace(start, end, static_cast<juce::uint8>(88)), 0.5997029825168245, testConstants::doubleTolerance);
            expectWithinAbsoluteError(math::inverseLogSpace(start, end, static_cast<juce::uint8>(111)), 0.7001485087415876, testConstants::doubleTolerance);
            expectWithinAbsoluteError(math::inverseLogSpace(start, end, static_cast<juce::uint8>(222)), 1.0, testConstants::doubleTolerance);
        }

        void testFloatInput()
        {
            beginTest("Float input");

            constexpr auto start = 13.f;
            constexpr auto end = 31.f;

            expectWithinAbsoluteError(math::inverseLogSpace(start, end, 13.f), 0.f, testConstants::floatTolerance);
            expectWithinAbsoluteError(math::inverseLogSpace(start, end, 18.f), 0.37446287f, testConstants::floatTolerance);
            expectWithinAbsoluteError(math::inverseLogSpace(start, end, 24.f), 0.70549804f, testConstants::floatTolerance);
            expectWithinAbsoluteError(math::inverseLogSpace(start, end, 27.f), 0.8410307f, testConstants::floatTolerance);
            expectWithinAbsoluteError(math::inverseLogSpace(start, end, 31.f), 1.f, testConstants::floatTolerance);
        }

        void testDoubleInput()
        {
            beginTest("Double input");

            constexpr auto start = 13.579;
            constexpr auto end = 24.680;

            expectWithinAbsoluteError(math::inverseLogSpace(start, end, 13.579), 0.0, testConstants::doubleTolerance);
            expectWithinAbsoluteError(math::inverseLogSpace(start, end, 15.397), 0.21030089088951642, testConstants::doubleTolerance);
            expectWithinAbsoluteError(math::inverseLogSpace(start, end, 19.993), 0.647494537133225, testConstants::doubleTolerance);
            expectWithinAbsoluteError(math::inverseLogSpace(start, end, 21.436587), 0.7641816887566711, testConstants::doubleTolerance);
            expectWithinAbsoluteError(math::inverseLogSpace(start, end, 24.680), 1.0, testConstants::doubleTolerance);
        }
    };
} // namespace jump

//======================================================================================================================
static jump::LinearSpaceTests linearSpaceTests;
static jump::LogSpaceTests logSpaceTests;
static jump::InverseLogSpaceTests inverseLogSpaceTests;
