#pragma once

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    class SnapshotGenerator
        : private juce::Component
        , private juce::AsyncUpdater
    {
    public:
        //==============================================================================================================
        SnapshotGenerator(juce::Component& targetComponent, const juce::File& outputDirectory);

    private:
        //==============================================================================================================
        void paint(juce::Graphics&) override;
        void handleAsyncUpdate() override;

        //==============================================================================================================
        juce::Component& component;
        const juce::File file;
    };
} // namespace jump
