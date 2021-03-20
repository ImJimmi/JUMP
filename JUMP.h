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
#include "containers/jump_CircularBuffer.h"
#include "interfaces/jump_StatefulObject.h"
#include "interfaces/jump_AudioComponentEngine.h"
#include "utilities/jump_LookAndFeelAccessor.h"
#include "utilities/jump_Functions.h"
#include "utilities/jump_VariantConverters.h"

#include "audio/jump_AudioTransferManager.h"
#include "engines/jump_SpectrumAnalyserEngine.h"
#include "interfaces/jump_PluginProcessor.h"
#include "interfaces/jump_PluginEditor.h"

#include "components/level_meter/jump_LevelMeter.h"
#include "components/level_meter/jump_LevelMeterEngine.h"
#include "components/level_meter/jump_LevelMeterBackgroundComponent.h"
#include "components/level_meter/jump_LevelMeterLabelsComponent.h"
#include "components/level_meter/jump_LevelMeterRendererComponent.h"
#include "components/level_meter/jump_LevelMeterComponent.h"

#include "graphics/jump_LookAndFeel.h"
