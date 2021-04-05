#pragma once

#include <JuceHeader.h>

//======================================================================================================================
namespace jumpDemo
{
    //==================================================================================================================
    class SmileyFace :   public jump::Canvas
    {
    public:
        //==============================================================================================================
        struct LookAndFeelMethods
        {
            virtual ~LookAndFeelMethods() = default;

            virtual void drawSmileyFace(juce::Graphics& g, const SmileyFace& smiley) const noexcept = 0;
        };

        SmileyFace();
        ~SmileyFace() override = default;

    private:
        //==============================================================================================================
        void paint(juce::Graphics& g) override;

        //==============================================================================================================
        jump::LookAndFeelAccessor<LookAndFeelMethods> lookAndFeel;
    };

    //==================================================================================================================
    class CanvasContainer   :   public jump::Container
    {
    public:
        //==============================================================================================================
        CanvasContainer();
        ~CanvasContainer() override;

    private:
        //==============================================================================================================
        class LookAndFeel   :   public jump::LookAndFeel,
                                public SmileyFace::LookAndFeelMethods
        {
            void drawSmileyFace(juce::Graphics& g, const SmileyFace& smiley) const noexcept override;
            void drawGloucestershireFlag(juce::Graphics& g, const jump::Canvas& canvas) const noexcept;

            friend class CanvasContainer;
        };

        //==============================================================================================================
        void resized() override;

        //==============================================================================================================
        LookAndFeel lookAndFeel;

        SmileyFace smiley;
        jump::Canvas gloucestershireFlag{ [this](juce::Graphics& g) {
            lookAndFeel.drawGloucestershireFlag(g, gloucestershireFlag);
        } };
    };
}   // jumpDemo
