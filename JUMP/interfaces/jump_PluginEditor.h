#pragma once

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    class PluginEditor  :   public juce::AudioProcessorEditor
    {
    public:
        //==============================================================================================================
        explicit PluginEditor(PluginProcessor& p)
            :   juce::AudioProcessorEditor{ &p },
                processor{ p }
        {
        }

    protected:
        //==============================================================================================================
        void setGUIState(juce::ValueTree& valueTreeToUseForGUIState)
        {
            processor.setGUIState(valueTreeToUseForGUIState);
        }

        //==============================================================================================================
        PluginProcessor& processor;
    };
}   // namespace jump
