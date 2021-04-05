#pragma once

//======================================================================================================================
namespace jump
{
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
            component = componentToAttachTo;
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

    private:
        //==============================================================================================================
        juce::Component* component{ nullptr };
    };
}   // namespace jump
