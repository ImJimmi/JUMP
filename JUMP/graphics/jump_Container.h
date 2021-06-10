#pragma once

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    class Container : public juce::Component
    {
    public:
        //==============================================================================================================
        Container() = default;

        Container(const juce::String& componentName)
            : juce::Component{ componentName }
        {
        }

    private:
        //==============================================================================================================
        void paint(juce::Graphics&) override final {}
        void paintOverChildren(juce::Graphics&) override final {}
    };
} // namespace jump
