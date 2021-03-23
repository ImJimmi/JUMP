#pragma once

#include <JuceHeader.h>

//======================================================================================================================
class LevelMetersDemo  : public juce::Component
{
public:
    //==================================================================================================================
    LevelMetersDemo();
    ~LevelMetersDemo() override;

    //==================================================================================================================
    void setSampleRate(double sampleRate);

    void addInputSamples (const std::array<std::vector<float>, 2>& samples);
    void addOutputSamples(const std::array<std::vector<float>, 2>& samples);

private:
    //==================================================================================================================
    void resized() override;

    //==================================================================================================================
    jump::LevelMeter::Engine inputEngineLeft;
    jump::LevelMeter::Engine inputEngineRight;
    jump::LevelMeter::Engine outputEngineLeft;
    jump::LevelMeter::Engine outputEngineRight;

    jump::LevelMeterComponent inputMetersLabelsBelow   { { &inputEngineLeft,  &inputEngineRight  } };
    jump::LevelMeterComponent inputMetersLabelsCentred { { &inputEngineLeft,  &inputEngineRight  } };
    jump::LevelMeterComponent inputMetersLabelsAbove   { { &inputEngineLeft,  &inputEngineRight  } };
    jump::LevelMeterComponent outputMetersLabelsRight  { { &outputEngineLeft, &outputEngineRight } };
    jump::LevelMeterComponent outputMetersLabelsCentred{ { &outputEngineLeft, &outputEngineRight } };
    jump::LevelMeterComponent outputMetersLabelsLeft   { { &outputEngineLeft, &outputEngineRight } };

    juce::Label inputLabel { {}, "Input" };
    juce::Label outputLabel{ {}, "Output" };

    //==================================================================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LevelMetersDemo)
};
