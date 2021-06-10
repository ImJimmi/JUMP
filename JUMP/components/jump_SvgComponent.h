#pragma once

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    class SvgComponent : public Canvas
    {
    public:
        //==============================================================================================================
        struct LookAndFeelMethods
        {
            virtual ~LookAndFeelMethods() = default;

            virtual void drawSvgComponent(juce::Graphics& g, const SvgComponent& component) const = 0;
        };

        //==============================================================================================================
        SvgComponent();
        SvgComponent(const juce::XmlElement& svg);

        //==============================================================================================================
        const juce::Drawable& getDrawable() const noexcept;
        void setSVG(const juce::XmlElement& svg);

    private:
        //==============================================================================================================
        void paint(juce::Graphics& g) override final;

        //==============================================================================================================
        void initialise();

        //==============================================================================================================
        std::unique_ptr<juce::Drawable> drawable;

        LookAndFeelAccessor<LookAndFeelMethods> lookAndFeel;
    };
} // namespace jump
