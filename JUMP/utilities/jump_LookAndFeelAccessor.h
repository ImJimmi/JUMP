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
        void attachTo(juce::Component* componentToAttachTo)
        {
            jassert(componentToAttachTo != nullptr);

            component = componentToAttachTo;

            listener.reset(new LookAndFeelListener{ *componentToAttachTo });
            listener->setCallback([this]() {
                if (onValidLookAndFeelFound != nullptr)
                    onValidLookAndFeelFound();
            });
        }

        //==============================================================================================================
        const LookAndFeelMethods* operator->() const
        {
            if (auto laf = dynamic_cast<LookAndFeelMethods*>(&component->getLookAndFeel()))
                return laf;

            /** The look and feel type being used by the Component this object is attached to isn't of the type given as
                a template argument!

                You should make sure to only use this operator when you're sure the correct LookAndFeel is being used.
            */
            jassertfalse;

            return nullptr;
        }

        operator bool()
        {
            return dynamic_cast<LookAndFeelMethods*>(&component->getLookAndFeel()) != nullptr;
        }

        //==============================================================================================================
        std::function<void(void)> onValidLookAndFeelFound = nullptr;

    private:
        //==============================================================================================================
        class LookAndFeelListener  :    public LookAndFeelListenerBase,
                                        public juce::Component
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
}   // namespace jump
