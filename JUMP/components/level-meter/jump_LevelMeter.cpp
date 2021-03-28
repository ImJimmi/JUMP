#include "jump_LevelMeter.h"

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    LevelMeter::LevelMeter(const LevelMeterEngine& engineToUse)
        :   background{ engineToUse },
            renderer{ engineToUse }
    {
        addAndMakeVisible(background);
        addAndMakeVisible(renderer);
    }

    //==================================================================================================================
    void LevelMeter::setOrientation(Orientation newOrientation)
    {
        background.setOrientation(newOrientation);
        renderer.setOrientation(newOrientation);
    }

    //==================================================================================================================
    void LevelMeter::resized()
    {
        const auto bounds = getLocalBounds();

        background.setBounds(bounds);
        renderer.setBounds(bounds);
    }

    void LevelMeter::colourChanged()
    {
        if (isColourSpecified(LookAndFeel::levelMeterSafeColourId))
        {
            renderer.setColour(LookAndFeel::levelMeterSafeColourId,
                               findColour(LookAndFeel::levelMeterSafeColourId));
        }
        if (isColourSpecified(LookAndFeel::levelMeterWarningColourId))
        {
            renderer.setColour(LookAndFeel::levelMeterWarningColourId,
                               findColour(LookAndFeel::levelMeterWarningColourId));
        }
        if (isColourSpecified(LookAndFeel::levelMeterDangerColourId))
        {
            renderer.setColour(LookAndFeel::levelMeterDangerColourId,
                               findColour(LookAndFeel::levelMeterDangerColourId));
        }
    }
}   // namespace jump
