#include "jumpDemo_SpectrumAnalysers.h"

//======================================================================================================================
namespace jumpDemo
{
    //==================================================================================================================
    SpectrumAnalysers::SpectrumAnalysers()
    {
    }

    //==================================================================================================================
    void SpectrumAnalysers::setSampleRate(double sampleRate)
    {
        inputEngine.setSampleRate(sampleRate);
        outputEngine.setSampleRate(sampleRate);
    }

    void SpectrumAnalysers::addInputSamples(const std::vector<float>& samples)
    {
        inputEngine.addSamples(samples);
    }

    void SpectrumAnalysers::addOutputSamples(const std::vector<float>& samples)
    {
        outputEngine.addSamples(samples);
    }
}   // namespace jumpDemo
