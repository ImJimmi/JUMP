#pragma once

#include <JuceHeader.h>

//======================================================================================================================
namespace jumpDemo
{
    //==================================================================================================================
    class LevelMeters   :   public juce::Component
    {
    public:
        //==============================================================================================================
        LevelMeters();
        ~LevelMeters() override;

        //==============================================================================================================
        void setSampleRate(double sampleRate);

        void addInputSamples(const std::array<std::vector<float>, 2>& samples);
        void addOutputSamples(const std::array<std::vector<float>, 2>& samples);

    private:
        //==============================================================================================================
        void resized() override;

        //==============================================================================================================
        jump::LevelMeterEngine inputEngineLeft;
        jump::LevelMeterEngine inputEngineRight;
        jump::LevelMeterEngine outputEngineLeft;
        jump::LevelMeterEngine outputEngineRight;

        jump::MultiMeter inputMetersLabelsBelow{ { &inputEngineLeft,  &inputEngineRight  } };
        jump::MultiMeter inputMetersLabelsCentred{ { &inputEngineLeft,  &inputEngineRight  } };
        jump::MultiMeter inputMetersLabelsAbove{ { &inputEngineLeft,  &inputEngineRight  } };
        jump::MultiMeter outputMetersLabelsRight{ { &outputEngineLeft, &outputEngineRight } };
        jump::MultiMeter outputMetersLabelsCentred{ { &outputEngineLeft, &outputEngineRight } };
        jump::MultiMeter outputMetersLabelsLeft{ { &outputEngineLeft, &outputEngineRight } };

        juce::Label inputLabel{ {}, "Input" };
        juce::Label outputLabel{ {}, "Output" };

        //==============================================================================================================
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LevelMeters)
    };
}   // namespace jumpDemo
