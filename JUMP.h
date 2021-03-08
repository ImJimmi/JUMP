/** BEGIN_JUCE_MODULE_DECLARATION

    ID:                 JUMP
    vendor:             James Johnson
    version:            0.1.0
    name:               JUMP
    description:        James' Utilities Module for Plugins.
    website:            https://github.com/ImJimmi/JUMP
    license:            GNU3
    minimumCppStandard: 17

    dependencies:       juce_audio_processors, juce_dsp

    END_JUCE_MODULE_DECLARATION
*/

//======================================================================================================================
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>

//======================================================================================================================
namespace juce
{
    //==================================================================================================================
    template <>
    struct VariantConverter<juce::dsp::WindowingFunction<float>::WindowingMethod>
    {
        //==============================================================================================================
        using WindowingMethod = juce::dsp::WindowingFunction<float>::WindowingMethod;

        //==============================================================================================================
        static WindowingMethod fromVar(const juce::var& v)
        {
            return static_cast<WindowingMethod>(static_cast<int>(v));
        }

        static juce::var toVar(const WindowingMethod& method)
        {
            return { static_cast<int>(method) };
        }
    };
}   // namespace juce

namespace jump
{
    //==================================================================================================================
    namespace SharedPropertyIDs
    {
        //==============================================================================================================
        const static juce::Identifier sampleRateId{ "sampleRate" };
    };  // namespace SharedPropertyIDs

    //==================================================================================================================
    using BoolParam   = juce::AudioParameterBool;
    using ChoiceParam = juce::AudioParameterChoice;
    using FloatParam  = juce::AudioParameterFloat;
    using IntParam    = juce::AudioParameterInt;
}   // namespace jump

//======================================================================================================================
#include "interfaces/jump_StatefulObject.h"
#include "utilities/jump_Functions.h"

#include "audio/jump_AudioTransferManager.h"
#include "containers/jump_CircularBuffer.h"
#include "engines/jump_AudioComponentEngine.h"
#include "engines/jump_LevelMeterEngine.h"
#include "engines/jump_SpectrumAnalyserEngine.h"
#include "interfaces/jump_PluginProcessor.h"
#include "interfaces/jump_PluginEditor.h"
