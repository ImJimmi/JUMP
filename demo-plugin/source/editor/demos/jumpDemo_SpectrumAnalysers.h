#pragma once

#include <JuceHeader.h>

//======================================================================================================================
namespace jumpDemo
{
    //==================================================================================================================
    class SpectrumAnalysers :   public jump::Container
    {
    public:
        //==============================================================================================================
        SpectrumAnalysers();

        //==============================================================================================================
        void setSampleRate(double sampleRate);

        void addInputSamples(const std::vector<float>& samples);
        void addOutputSamples(const std::vector<float>& samples);

    private:
        //==============================================================================================================
        jump::SpectrumAnalyserEngine inputEngine;
        jump::SpectrumAnalyserEngine outputEngine;

        //==============================================================================================================
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SpectrumAnalysers)
    };
}   // namespace jumpDemo
