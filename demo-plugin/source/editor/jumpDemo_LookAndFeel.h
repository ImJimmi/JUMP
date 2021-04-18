#pragma once

#include "jumpDemo_PluginEditor.h"

//======================================================================================================================
namespace jumpDemo
{
    //==================================================================================================================
    namespace lookAndFeelImplementations
    {
        //==================================================================================================================
        class PluginEditorLookAndFeel :   public PluginEditor::LookAndFeelMethods
        {
            void drawSeparator(juce::Graphics& g, const jump::Canvas& canvas) const noexcept override;

            jump::TrackInfoList getTemplateColumns() const noexcept override;
            jump::Size<int> getDemoSelectorSize() const noexcept override;
            jump::Margin<float> getDemoSelectorMargin() const noexcept override;
        };
    }

    //==================================================================================================================
    class LookAndFeel   :   public jump::LookAndFeel,
                            public lookAndFeelImplementations::PluginEditorLookAndFeel
    {
    public:
        //==============================================================================================================
        LookAndFeel() = default;

    private:
        //==============================================================================================================
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LookAndFeel)
    };
}   // namespace jumpDemo
