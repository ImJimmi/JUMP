#include "jump_TestConstants.h"

#include <juce_core/juce_core.h>

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    class TestRunner
    {
    public:
        //==============================================================================================================
        TestRunner()
        {
            testRunner.setAssertOnFailure(false);
            testRunner.setPassesAreLogged(false);
        }

        //==============================================================================================================
        void runTests()
        {
            testRunner.runTestsInCategory (testConstants::categoryName);
        }

        //==============================================================================================================
        int getNumFailures() const
        {
            auto numFailed = 0;

            for (auto i = 0; i < testRunner.getNumResults(); i++)
            {
                if (auto result = testRunner.getResult (i))
                    numFailed += result->failures;
            }

            return numFailed;
        }

    private:
        //==============================================================================================================
        juce::UnitTestRunner testRunner;
    };
} // namespace jump

//======================================================================================================================
void printNumFailuresMessage(int numFailures)
{
    std::cout << std::endl;
    std::cout << numFailures << " JUMP test(s) failed!" << std::endl;
    std::cout << std::endl;
}

int getExitCodeForNumFailures(int numFailures)
{
    return numFailures > 0 ? 1 : 0;
}

int main()
{
    jump::TestRunner testRunner;
    testRunner.runTests();

    const auto numFailures = testRunner.getNumFailures();
    printNumFailuresMessage(numFailures);

    return getExitCodeForNumFailures(numFailures);
}
