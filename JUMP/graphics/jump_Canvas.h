#pragma once

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    /** Represents a Component that draws some graphics to the screen and thus has no child Components.

        You can either inherit from this class in your custom Component class, or simply create an instance of
        jump::Canvas and provide the instance a lambda function so it can draw itself.

        This class is intended to to be used only to draw graphics and not to contain any child Components or to
        override any callbacks from mouse or keyboard events. Instead, you should let a parent Component handle any user
        interactions and pass the information to your custom Canvas instead.
    */
    class Canvas    :   public juce::Component
    {
    public:
        //==============================================================================================================
        /** Creates a default, blank Canvas. */
        Canvas() = default;

        /** Creates a blank Canvas with the given Component name. */
        Canvas(const juce::String& componentName)
            :   juce::Component{ componentName }
        {
        }

        /** Creates a Canvas that will use the given function for its drawing. */
        Canvas(const std::function<void(juce::Graphics&)>& drawFunctionToUse)
        {
            setDrawFunction(drawFunctionToUse);
        }

        //==============================================================================================================
        /** Specifies the drawing function that will be used by this canvas.

            Triggers a repaint after setting the draw function to the one provided.
        */
        void setDrawFunction(const std::function<void(juce::Graphics&)>& drawFunctionToUse)
        {
            draw = drawFunctionToUse;
            repaint();
        }

        //==============================================================================================================
        /** If not nullptr, this function will be called by the default paint method for a Canvas. */
        std::function<void(juce::Graphics&)> draw = nullptr;

    private:
        //==============================================================================================================
        void paint(juce::Graphics& g) override
        {
            if (draw != nullptr)
                draw(g);
        }

        void childrenChanged() override final
        {
            for (auto child : getChildren())
            {
                juce::ignoreUnused(child);

                // Canvas's should never have child components (with the exception of the LookAndFeelListener used by
                // the LookAndFeelAccessor class since it's always invisible).
                jassert(dynamic_cast<LookAndFeelListenerBase*>(child) != nullptr);
            }
        }

        bool hitTest(int, int) override final
        {
            return false;
        }

        bool keyPressed(const juce::KeyPress&) override final
        {
            return false;
        }

        bool keyStateChanged(bool) override final
        {
            return false;
        }

        void modifierKeysChanged(const juce::ModifierKeys&) override {}

        void focusGained(FocusChangeType) override final
        {
            // Canvas's should never have keyboard focus.
            jassertfalse;
        }

        void focusLost(FocusChangeType) override final {}
        void focusOfChildComponentChanged(FocusChangeType) override final {}
        void childBoundsChanged(juce::Component*) override final {}

        void mouseMove       (const juce::MouseEvent&) override final {}
        void mouseEnter      (const juce::MouseEvent&) override final {}
        void mouseExit       (const juce::MouseEvent&) override final {}
        void mouseDown       (const juce::MouseEvent&) override final {}
        void mouseDrag       (const juce::MouseEvent&) override final {}
        void mouseUp         (const juce::MouseEvent&) override final {}
        void mouseDoubleClick(const juce::MouseEvent&) override final {}
        void mouseWheelMove  (const juce::MouseEvent&, const juce::MouseWheelDetails&) override final {}
        void mouseMagnify    (const juce::MouseEvent&, float) override final {}
    };
}   // namespace vocaster
