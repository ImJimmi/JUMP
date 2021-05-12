#include "jump_SvgComponent.h"

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    SvgComponent::SvgComponent()
    {
        initialise();
    }

    SvgComponent::SvgComponent(const juce::XmlElement& svg)
        :   drawable {juce::Drawable::createFromSVG(svg)}
    {
        initialise();
    }

    //==================================================================================================================
    const juce::Drawable& SvgComponent::getDrawable() const noexcept
    {
        // You can't call this method without first calling setSVG().
        jassert(drawable != nullptr);

        return *drawable;
    }

    void SvgComponent::setSVG(const juce::XmlElement& svg)
    {
        drawable = juce::Drawable::createFromSVG(svg);
    }

    //==================================================================================================================
    void SvgComponent::paint(juce::Graphics& g)
    {
        lookAndFeel->drawSvgComponent(g, *this);
    }

    //==================================================================================================================
    void SvgComponent::initialise()
    {
        lookAndFeel.attachTo(this);
        setInterceptsMouseClicks(false, false);
    }
}   // namespace jump
