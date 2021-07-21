#pragma once

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    struct LookAndFeelListenerBase
    {
        virtual ~LookAndFeelListenerBase() = default;

        virtual void setCallback(const std::function<void(void)> callbackFunction) = 0;
    };

    //==================================================================================================================
    template <typename LookAndFeelMethods>
    class LookAndFeelAccessor
    {
    public:
        //==============================================================================================================
        LookAndFeelAccessor() = default;

        //==============================================================================================================
        void attachToComponentWithoutListener(juce::Component* componentToAttachTo)
        {
            jassert(componentToAttachTo != nullptr);
            component = componentToAttachTo;
        }

        void attachTo(juce::Component* componentToAttachTo)
        {
            attachToComponentWithoutListener(componentToAttachTo);

            listener.reset(new LookAndFeelListener{ *componentToAttachTo });
            listener->setCallback([this]() {
                if (onValidLookAndFeelFound != nullptr)
                    onValidLookAndFeelFound();
            });
        }

        //==============================================================================================================
        bool hasValidLookAndFeel() const
        {
            jassert(component != nullptr);
            return dynamic_cast<LookAndFeelMethods*>(&component->getLookAndFeel()) != nullptr;
        }

        LookAndFeelMethods& getLookAndFeel() const
        {
            /** The look and feel type being used by the Component this object is attached to isn't of the type given as
                a template argument!

                You should make sure to only use this operator when you're sure the correct LookAndFeel is being used.
            */
            jassert (hasValidLookAndFeel());

            return *dynamic_cast<LookAndFeelMethods*>(&component->getLookAndFeel());
        }

        //==============================================================================================================
        const LookAndFeelMethods* operator->() const
        {
            if (auto* laf = &getLookAndFeel())
                return laf;

            return nullptr;
        }

        operator bool()
        {
            return hasValidLookAndFeel();
        }

        //==============================================================================================================
        std::function<void(void)> onValidLookAndFeelFound = nullptr;

    private:
        //==============================================================================================================
        class LookAndFeelListener
            : public LookAndFeelListenerBase
            , public juce::Component
        {
        public:
            LookAndFeelListener(juce::Component& comp)
            {
                comp.addChildComponent(this);
                lookAndFeelChanged();
            }

            void setCallback(const std::function<void(void)> callbackFunction) override
            {
                validLookAndFeelFoundCallback = callbackFunction;

                if (callbackFunction != nullptr)
                    lookAndFeelChanged();
            }

        private:
            void parentHierarchyChanged() override
            {
                lookAndFeelChanged();
            }

            void lookAndFeelChanged() override
            {
                if (auto laf = dynamic_cast<LookAndFeelMethods*>(&getLookAndFeel()))
                {
                    if (validLookAndFeelFoundCallback != nullptr)
                        validLookAndFeelFoundCallback();
                }
            }

            std::function<void(void)> validLookAndFeelFoundCallback = nullptr;
        };

        //==============================================================================================================
        std::unique_ptr<LookAndFeelListener> listener;
        juce::Component::SafePointer<juce::Component> component;
    };
} // namespace jump
