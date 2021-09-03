#pragma once

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    template <typename ComponentType>
    class ComponentLayout : private juce::ComponentListener
    {
    public:
        //==============================================================================================================
        explicit ComponentLayout(ComponentType& targetComponent)
            : component{ targetComponent }
        {
            component.addComponentListener(this);
        }

        ~ComponentLayout()
        {
            static_assert(std::is_base_of_v<juce::Component, ComponentType>);

            component.removeComponentListener(this);
        }

        //==============================================================================================================
        virtual void updateLayout() const = 0;

    protected:
        //==============================================================================================================
        ComponentType& component;

    private:
        //==============================================================================================================
        void componentMovedOrResized (juce::Component&, bool /*wasMoved*/, bool wasResized) final
        {
            if (!wasResized)
                return;

            if (component.getWidth() == 0 || component.getHeight() == 0)
                return;

            updateLayout();
        }
    };
} // namespace jump
