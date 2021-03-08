#pragma once

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    class StatefulObject    :   private juce::ValueTree::Listener
    {
    public:
        //==============================================================================================================
        StatefulObject(const juce::Identifier& type, StatefulObject* parentState = nullptr)
            :   valueTree{ type },
                parent{ parentState }
        {
            if (parentState != nullptr)
                parentState->addChild(this);

            valueTree.addListener(this);
        }

        //==============================================================================================================
        StatefulObject& setProperty(const juce::Identifier& name, const juce::var& newValue)
        {
            setPropertyRecursively(name, newValue, valueTree);

            return *this;
        }

        juce::var getProperty(const juce::Identifier& name) const noexcept
        {
            return getPropertyRecursively(name);
        }

    protected:
        //==============================================================================================================
        virtual void propertyChanged(const juce::Identifier& name, const juce::var& newValue) = 0;

        //==============================================================================================================
        juce::ValueTree& getState()
        {
            return valueTree;
        }

    private:
        //==============================================================================================================
        void valueTreePropertyChanged(juce::ValueTree& tree, const juce::Identifier& name) override
        {
            if (tree != valueTree)
                return;
            
            callPropertyChangedRecursively(tree, name);
        }

        void valueTreeRedirected(juce::ValueTree& tree) override
        {
            if (tree != valueTree)
                return;
            
            for (auto i = 0; i < tree.getNumProperties(); i++)
            {
                const auto name = tree.getPropertyName(i);

                callPropertyChangedRecursively(tree, name);
            }
        }

        //==============================================================================================================
        void addChild(StatefulObject* child)
        {
            children.addIfNotAlreadyThere(child);

            if (!valueTree.getChildWithName(child->valueTree.getType()).isValid())
                valueTree.addChild(juce::ValueTree{ child->valueTree.getType() }, -1, nullptr);

            child->valueTree = valueTree.getChildWithName(child->valueTree.getType());

            jassert(child->valueTree.isValid());
        }

        void callPropertyChangedRecursively(const juce::ValueTree& originTree, const juce::Identifier& name)
        {
            propertyChanged(name, originTree[name]);

            for (auto& child : children)
                child->callPropertyChangedRecursively(originTree, name);
        }

        //==============================================================================================================
        void setPropertyRecursively(const juce::Identifier& name, const juce::var& newValue, juce::ValueTree& originTree)
        {
            if (valueTree.hasProperty(name))
                valueTree.setProperty(name, newValue, nullptr);
            else if (parent != nullptr)
                parent->setPropertyRecursively(name, newValue, originTree);
            else
                originTree.setProperty(name, newValue, nullptr);
        }

        juce::var getPropertyRecursively(const juce::Identifier& name) const noexcept
        {
            if (valueTree.hasProperty(name))
                return valueTree[name];

            if (parent != nullptr)
                return parent->getPropertyRecursively(name);

            return {};
        }

        //==============================================================================================================
        juce::ValueTree valueTree;

        StatefulObject* parent{ nullptr };
        juce::Array<StatefulObject*> children;
    };
}   // namespace jump
