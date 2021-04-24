#include "jumpDemo_LookAndFeel.h"

//======================================================================================================================
namespace constants
{
    //==================================================================================================================
    namespace pluginEditor
    {
        constexpr auto demoSelectorWidth = 160;
        constexpr auto demoSelectorHeight = 25;
        constexpr auto demoSelectorMargin = 10.f;
    }   // pluginEditor
}   // namespace constants

//======================================================================================================================
namespace jumpDemo::lookAndFeelImplementations
{
    //==================================================================================================================
    void PluginEditorLookAndFeel::drawSeparator(juce::Graphics& g, const jump::Canvas&) const noexcept
    {
        g.fillAll (jump::MaterialColourPalette::blueGrey800);
    }

    jump::TrackInfoList PluginEditorLookAndFeel::getTemplateColumns() const noexcept
    {
        return {
            juce::Grid::Px{ constants::pluginEditor::demoSelectorWidth + getDemoSelectorMargin().getRightPlusLeft() },
            juce::Grid::Px{ 1 },
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
