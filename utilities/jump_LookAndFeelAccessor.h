#pragma once

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    struct LookAndFeelAccessorListener
    {
        virtual ~LookAndFeelAccessorListener() = default;

        virtual void validLookAndFeelInUse() = 0;
    };

    //==================================================================================================================
    template <typename LookAndFeelMethods>
    class LookAndFeelAccessor   :   private juce::Component
    {
    public:
        //==============================================================================================================
        using Listener = LookAndFeelAccessorListener;

        //==============================================================================================================
        LookAndFeelAccessor(juce::Component& parent)
        {
            parent.addChildComponent(*this);
        }

        //==============================================================================================================
        void addListener(Listener* newListener)
        {
            listeners.add(newListener);
        }

        void removeListener(Listener* listenerToRemove)
        {
            listeners.remove(listenerToRemove);
        }

        //==============================================================================================================
        LookAndFeelMethods* operator->()
        {
            /** The look and feel type being used by the parent component isn't of the type given as a template
                argument!

                You should make sure to only use this operator when you're sure the correct LookAndFeel is being used.
            */
            jassert(lookAndFeel != nullptr);

            return lookAndFeel;
        }

        operator bool()
        {
            return lookAndFeel != nullptr;
        }

    private:
        //==============================================================================================================
        void parentHierarchyChanged() override
        {
            lookAndFeelChanged();
        }

        void lookAndFeelChanged() override
        {
            if (auto laf = dynamic_cast<LookAndFeelMethods*>(&getLookAndFeel()))
            {
                lookAndFeel = laf;

                listeners.call(&Listener::validLookAndFeelInUse);
            }
        }

        //==============================================================================================================
        LookAndFeelMethods* lookAndFeel{ nullptr };
        juce::ListenerList<Listener> listeners;
    };
}   // namespace jump
