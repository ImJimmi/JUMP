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
    }  // namespace SharedPropertyIDs

    //==================================================================================================================
    enum class Orientation
    {
        vertical,
        horizontal
    };

    //==================================================================================================================
    using BoolParam   = juce::AudioParameterBool;
    using ChoiceParam = juce::AudioParameterChoice;
    using FloatParam  = juce::AudioParameterFloat;
    using IntParam    = juce::AudioParameterInt;
}   // namespace jump

//======================================================================================================================
namespace juce
{
    //==================================================================================================================
    template <>
    struct VariantConverter<jump::Orientation>
    {
        //==============================================================================================================
        static jump::Orientation fromVar(const juce::var& v)
        {
            return static_cast<jump::Orientation>(static_cast<int>(v));
        }

        static juce::var toVar(const jump::Orientation& orientation)
        {
            return { static_cast<int>(orientation) };
        }
    };
}

//======================================================================================================================
#include "containers/jump_CircularBuffer.h"
#include "interfaces/jump_StatefulObject.h"
#include "interfaces/jump_AudioComponentEngine.h"
#include "utilities/jump_LookAndFeelAccessor.h"
#include "utilities/jump_Functions.h"
#include "utilities/jump_VariantConverters.h"

#include "interfaces/jump_Canvas.h"
#include "interfaces/jump_Container.h"

#include "audio/jump_AudioTransferManager.h"
#include "interfaces/jump_PluginProcessor.h"
#include "interfaces/jump_PluginEditor.h"

#include "components/level-meter/jump_LevelMeterEngine.h"
#include "components/level-meter/jump_LevelMeterBackgroundCanvas.h"
#include "components/level-meter/jump_LevelMeterRenderer.h"
#include "components/level-meter/jump_LevelMeter.h"
#include "components/level-meter/jump_LevelMeterLabelsComponent.h"
#include "components/level-meter/jump_MultiMeter.h"

#include "graphics/jump_LookAndFeel.h"
