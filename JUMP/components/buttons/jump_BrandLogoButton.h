#pragma once

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    class BrandLogoButton : public juce::Button
    {
    public:
        //==============================================================================================================
        BrandLogoButton();
        explicit BrandLogoButton(const juce::String& buttonName);

        //==============================================================================================================
        void resized() override;

        //==============================================================================================================
        void setLogo(const juce::XmlElement& brandLogoSVG);
        void setURL(const juce::URL& brandURL);

    protected:
        //==============================================================================================================
        void clicked() override;
        void paintButton(juce::Graphics&, bool, bool) override {}

    private:
        //==============================================================================================================
        SvgComponent logo;
        juce::URL url;

        //==============================================================================================================
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BrandLogoButton)
    };
} // namespace jump
