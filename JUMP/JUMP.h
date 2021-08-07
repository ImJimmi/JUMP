#pragma once

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
    } // namespace SharedPropertyIDs

    //==================================================================================================================
    enum class Orientation
    {
        vertical,
        horizontal
    };

    //==================================================================================================================
    using BoolParam = juce::AudioParameterBool;
    using ChoiceParam = juce::AudioParameterChoice;
    using FloatParam = juce::AudioParameterFloat;
    using IntParam = juce::AudioParameterInt;

    using TrackInfoList = juce::Array<juce::Grid::TrackInfo>;

    //==================================================================================================================
    static constexpr auto defaultMinusInfDB = -100.0;
} // namespace jump

//======================================================================================================================
// clang-format off

// Audio
#include "audio/jump_AudioTransferManager.h"
#include "audio/jump_Level.h"
#include "audio/jump_Compressor.h"

// Components
        #include "utilities/jump_LookAndFeelAccessor.h"
    #include "graphics/jump_Canvas.h"
#include "components/jump_AttributedLabel.h"
#include "components/jump_SvgComponent.h"
#include "components/jump_SnapshotGenerator.h"
#include "components/buttons/jump_BrandLogoButton.h"
    #include "interfaces/jump_StatefulObject.h"
    #include "containers/jump_CircularBuffer.h"
    #include "interfaces/jump_AudioComponentEngine.h"
#include "components/level-meter/jump_LevelMeterEngine.h"
    #include "graphics/jump_Container.h"
#include "components/level-meter/jump_LevelMeter.h"
    #include "utilities/jump_Functions.h"
#include "components/level-meter/jump_LevelMeterLabelsComponent.h"
#include "components/level-meter/jump_MultiMeter.h"
#include "components/spectrum-analyser/jump_SpectrumAnalyserEngine.h"
    #include "graphics/jump_PaintOptions.h"
#include "components/spectrum-analyser/jump_SpectrumAnalyser.h"
    #include "utilities/jump_VariantConverters.h"
#include "components/spectrum-analyser/jump_SpectrumAnalyserLabelsComponent.h"
#include "components/spectrum-analyser/jump_MultiAnalyser.h"
#include "components/windows/jump_ModalWindow.h"

// Containers

// Interfaces
#if JUCE_MODULE_AVAILABLE_juce_audio_processors
    #include "interfaces/jump_PluginProcessor.h"
    #include "interfaces/jump_PluginEditor.h"
#endif

// Graphics
#include "graphics/jump_ColourIDs.h"
#include "graphics/jump_LookAndFeel.h"
#include "graphics/jump_Margin.h"
#include "graphics/jump_MaterialColourPalette.h"
#include "graphics/jump_Size.h"

// Utilities

// clang-format on
