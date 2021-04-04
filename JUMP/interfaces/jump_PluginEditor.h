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
                pluginProcessor{ p }
        {
        }

    protected:
        //==============================================================================================================
        void setGUIState(juce::ValueTree& valueTreeToUseForGUIState)
        {
            pluginProcessor.setGUIState(valueTreeToUseForGUIState);
        }

        //==============================================================================================================
        PluginProcessor& pluginProcessor;
    };
}   // namespace jump
