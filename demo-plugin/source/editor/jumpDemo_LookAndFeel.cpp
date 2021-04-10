#include "jumpDemo_LookAndFeel.h"

//======================================================================================================================
namespace constants
{
    //==================================================================================================================
    namespace pluginEditor
    {
        constexpr auto demoSelectorWidth = 180;
        constexpr auto demoSelectorHeight = 25;
        constexpr auto demoSelectorMargin = 10.f;
    }   // pluginEditor
}   // namespace constants

//======================================================================================================================
namespace jumpDemo::lookAndFeelImplementations
{
    //==================================================================================================================
    jump::TrackInfoList PluginEditorLookAndFeel::getTemplateColumns() const noexcept
    {
        return {
            juce::Grid::Px{ constants::pluginEditor::demoSelectorWidth },
            juce::Grid::Fr{ 1 }
        };
    }

    jump::Size<int> PluginEditorLookAndFeel::getDemoSelectorSize() const noexcept
    {
        return {
            constants::pluginEditor::demoSelectorWidth,
            constants::pluginEditor::demoSelectorHeight
        };
    }

    jump::Margin<float> PluginEditorLookAndFeel::getDemoSelectorMargin() const noexcept
    {
        return jump::Margin<float>{ constants::pluginEditor::demoSelectorMargin };
    }
}   // namespace jumpDemo
