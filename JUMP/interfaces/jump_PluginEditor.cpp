#include "jump_PluginEditor.h"

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    PluginEditor::PluginEditor(PluginProcessor& p)
        : juce::AudioProcessorEditor{ &p }
    {
    }
} // namespace jump
