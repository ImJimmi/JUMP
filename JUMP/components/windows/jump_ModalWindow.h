#pragma once

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    class ModalWindow : private juce::MouseListener
    {
    public:
        //==============================================================================================================
        ModalWindow();

        //==============================================================================================================
        void show(const juce::Point<int>& newPosition = {});
        void hide();

        //==============================================================================================================
        void setContentComponent(std::unique_ptr<juce::Component> newContentComponent);
        juce::Component& getContentComponent();
        const juce::Component& getContentComponent() const;

    private:
        //==============================================================================================================
        class WindowComponent : public jump::Container
        {
        public:
            explicit WindowComponent(ModalWindow& modalWindow);

            void inputAttemptWhenModal() override;

        private:
            ModalWindow& modalWindow;
        };

        //==============================================================================================================
        void mouseExit(const juce::MouseEvent& mouseEvent) override;

        //==============================================================================================================
        WindowComponent window{ *this };
        std::unique_ptr<juce::Component> contentComponent;
    };
} // namespace jump
