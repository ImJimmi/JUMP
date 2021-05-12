#pragma once

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    class BrandLogoComponent    :   public juce::Component
    {
    public:
        //==============================================================================================================
        BrandLogoComponent();
        BrandLogoComponent(const juce::XmlElement& brandLogoSVG, const juce::URL& brandURL);

        //==============================================================================================================
        void setLogo(const juce::XmlElement& brandLogoSVG);
        void setURL(const juce::URL& brandURL);

    private:
        //==============================================================================================================
        void resized() override;

        void mouseUp(const juce::MouseEvent& event) override;

        //==============================================================================================================
        void initialise();

        //==============================================================================================================
        SvgComponent logo;
        juce::URL url;
    };
}   // namespace jump
