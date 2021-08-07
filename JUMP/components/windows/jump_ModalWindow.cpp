#include "jump_ModalWindow.h"

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    ModalWindow::ModalWindow()
    {
        constexpr auto windowStyleFlags = juce::ComponentPeer::windowIsTemporary + juce::ComponentPeer::windowHasDropShadow;
        window.addToDesktop(windowStyleFlags);
        window.addMouseListener(this, true);
    }

    //==================================================================================================================
    void ModalWindow::show(const juce::Point<int>& position)
    {
        jassert(contentComponent != nullptr);

        window.setSize(contentComponent->getWidth(), contentComponent->getHeight());
        window.setTopLeftPosition(position.x, position.y);
        window.setVisible(true);
        window.enterModalState();
        window.toFront(true);
    }

    void ModalWindow::hide()
    {
        window.exitModalState(0);
        window.setVisible(false);
    }

    //==================================================================================================================
    void ModalWindow::setContentComponent(std::unique_ptr<juce::Component> newContentComponent)
    {
        // The content component should have its size set before given to the window.
        jassert(newContentComponent->getWidth() > 0 && newContentComponent->getHeight() > 0);

        contentComponent = std::move(newContentComponent);
        window.addAndMakeVisible(*contentComponent);
    }

    juce::Component& ModalWindow::getContentComponent()
    {
        jassert(contentComponent != nullptr);
        return *contentComponent;
    }

    const juce::Component& ModalWindow::getContentComponent() const
    {
        jassert(contentComponent != nullptr);
        return *contentComponent;
    }

    //==================================================================================================================
    ModalWindow::WindowComponent::WindowComponent(ModalWindow& parent)
        : modalWindow{ parent }
    {
    }

    void ModalWindow::WindowComponent::inputAttemptWhenModal()
    {
        modalWindow.hide();
    }

    //==================================================================================================================
    void ModalWindow::mouseExit(const juce::MouseEvent& /*mouseEvent*/)
    {
        if (!window.isMouseOverOrDragging(true))
            hide();
    }
} // namespace jump
