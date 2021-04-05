#include "jumpDemo_CanvasContainer.h"

//======================================================================================================================
namespace jumpDemo
{
    //==================================================================================================================
    SmileyFace::SmileyFace()
    {
        lookAndFeel.attachTo(this);
    }

    void SmileyFace::paint(juce::Graphics& g)
    {
        lookAndFeel->drawSmileyFace(g, *this);
    }

    //==================================================================================================================
    void CanvasContainer::LookAndFeel::drawSmileyFace(juce::Graphics& g, const SmileyFace& smiley) const noexcept
    {
        const auto bounds = smiley.getLocalBounds().toFloat();

        g.setGradientFill(juce::ColourGradient::vertical(juce::Colours::yellow.brighter(),
                                                         juce::Colours::yellow.darker(),
                                                         bounds));
        g.fillEllipse(bounds);

        g.addTransform(juce::AffineTransform::rotation(juce::MathConstants<float>::halfPi,
                                                       bounds.getCentreX(), bounds.getCentreY()));
        g.addTransform(juce::AffineTransform::translation(bounds.getWidth() * 0.05f, 0.f));
        g.setColour(juce::Colours::black);
        g.setFont({"Comic Sans", bounds.getHeight() * 0.8f, 0});
        g.drawText(":)", smiley.getLocalBounds().toFloat(), juce::Justification::centred);
    }

    void CanvasContainer::LookAndFeel::drawGloucestershireFlag(juce::Graphics& g,
                                                               const jump::Canvas& canvas) const noexcept
    {
        g.fillAll(juce::Colour{ 0xFF69C071 });

        const auto bounds = canvas.getLocalBounds().toFloat();

        g.setColour(juce::Colour{ 0xFFF3EACD });
        g.drawLine({ { bounds.getCentreX(), bounds.getY() }, { bounds.getCentreX(), bounds.getBottom() } },
                   bounds.getWidth() / 5.f);
        g.drawLine({ { bounds.getX(), bounds.getCentreY() }, { bounds.getRight(), bounds.getCentreY() } },
                   bounds.getHeight() / 2.5f);

        g.setColour(juce::Colour{ 0xFF0072BB });
        g.drawLine({ { bounds.getCentreX(), bounds.getY() }, { bounds.getCentreX(), bounds.getBottom() } },
                   bounds.getWidth() / 10.f);
        g.drawLine({ { bounds.getX(), bounds.getCentreY() }, { bounds.getRight(), bounds.getCentreY() } },
                   bounds.getHeight() / 5.f);
    }

    //==================================================================================================================
    CanvasContainer::CanvasContainer()
    {
        setLookAndFeel(&lookAndFeel);

        addAndMakeVisible(smiley);
        addAndMakeVisible(gloucestershireFlag);
    }

    CanvasContainer::~CanvasContainer()
    {
        setLookAndFeel(nullptr);
    }

    //==================================================================================================================
    void CanvasContainer::resized()
    {
        auto bounds = getLocalBounds();

        smiley.setSize(150, 150);
        smiley.setCentrePosition({ bounds.getX() + bounds.proportionOfWidth(0.25f), bounds.getCentreY() });

        gloucestershireFlag.setSize(250, 125);
        gloucestershireFlag.setCentrePosition({ bounds.getX() + bounds.proportionOfWidth(0.75f), bounds.getCentreY() });
    }
}   // namespace jumpDemo
