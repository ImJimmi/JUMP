#pragma once

#include <JuceHeader.h>
#include <core/jumpDemo_PluginInstance.h>

//======================================================================================================================
namespace jumpDemo
{
    //==================================================================================================================
    class PluginEditor  :   public jump::PluginEditor,
                            private juce::Timer
    {
    public:
        //==============================================================================================================
        struct LookAndFeelMethods
        {
            virtual ~LookAndFeelMethods() = default;

            virtual void drawSeparator(juce::Graphics& g, const jump::Canvas& canvas) const noexcept = 0;

            virtual jump::TrackInfoList getTemplateColumns() const noexcept = 0;
            virtual jump::Size<int> getDemoSelectorSize() const noexcept = 0;
            virtual jump::Margin<float> getDemoSelectorMargin() const noexcept = 0;
        };

        //==============================================================================================================
        PluginEditor(PluginInstance&);
        ~PluginEditor() override;

    private:
        //==============================================================================================================
        void resized() override;
        void timerCallback() override;

        //==============================================================================================================
        PluginInstance& pluginInstance;
        const std::unique_ptr<juce::LookAndFeel> lookAndFeel;
        jump::Canvas backgroundCanvas;

        juce::ComboBox demoSelector;
        jump::Canvas separator;
        std::unique_ptr<juce::Component> activeDemo;

        jump::LookAndFeelAccessor<LookAndFeelMethods> lookAndFeelAccessor;

        //==============================================================================================================
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginEditor)
    };
} // namespace jumpDemo
