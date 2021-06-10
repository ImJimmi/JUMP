#include "jump_BrandLogoComponent.h"

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    BrandLogoComponent::BrandLogoComponent()
    {
        initialise();
    }

    BrandLogoComponent::BrandLogoComponent(const juce::XmlElement& brandLogoSVG, const juce::URL& brandURL)
        : logo{ brandLogoSVG }
        , url{ brandURL }
    {
        initialise();
    }

    //==================================================================================================================
    void BrandLogoComponent::setLogo(const juce::XmlElement& brandLogoSVG)
    {
        logo.setSVG(brandLogoSVG);
    }

    void BrandLogoComponent::setURL(const juce::URL& brandURL)
    {
        url = brandURL;
        setMouseCursor(juce::MouseCursor::PointingHandCursor);
    }

    //==================================================================================================================
    void BrandLogoComponent::resized()
    {
        logo.setBounds(getLocalBounds());
    }

    void BrandLogoComponent::mouseUp(const juce::MouseEvent& /*event*/)
    {
        if (!url.isEmpty())
            url.launchInDefaultBrowser();
    }

    //==================================================================================================================
    void BrandLogoComponent::initialise()
    {
        addAndMakeVisible(logo);
    }
} // namespace jump
