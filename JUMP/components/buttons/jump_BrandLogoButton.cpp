#include "jump_BrandLogoButton.h"

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    BrandLogoButton::BrandLogoButton()
        : BrandLogoButton("")
    {
    }

    BrandLogoButton::BrandLogoButton(const juce::String& buttonName)
        : juce::Button{ buttonName }
    {
        addAndMakeVisible(logo);

        setMouseCursor(juce::MouseCursor::PointingHandCursor);
    }

    //==================================================================================================================
    void BrandLogoButton::resized()
    {
        logo.setBounds(getLocalBounds());
    }

    //==================================================================================================================
    void BrandLogoButton::setLogo(const juce::XmlElement& brandLogoSVG)
    {
        logo.setSVG(brandLogoSVG);
    }

    void BrandLogoButton::setURL(const juce::URL& brandURL)
    {
        url = brandURL;
    }

    void BrandLogoButton::clicked()
    {
        if (!url.isEmpty())
            url.launchInDefaultBrowser();
    }
} // namespace jump
