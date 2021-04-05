#pragma once

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    class LevelMeter    :   public Container
    {
    public:
        //==============================================================================================================
        LevelMeter(const LevelMeterEngine& engineToUse);

        //==============================================================================================================
        void setOrientation(Orientation newOrientation);

    private:
        //==============================================================================================================
        void resized() override;
        void colourChanged() override;

        //==============================================================================================================
        LevelMeterBackgroundCanvas background;
        LevelMeterRenderer renderer;

        //==============================================================================================================
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LevelMeter)
    };
}   // namespace jump
