#include "JUMP.h"

//======================================================================================================================
// clang-format off

// Audio
#include "audio/jump_Compressor.cpp"

// Components
#include "components/jump_AttributedLabel.cpp"
#include "components/jump_SnapshotGenerator.cpp"
#include "components/jump_SvgComponent.cpp"
#include "components/buttons/jump_BrandLogoButton.cpp"
#include "components/level-meter/jump_LevelMeterEngine.cpp"
#include "components/level-meter/jump_MultiMeter.cpp"
#include "components/spectrum-analyser/jump_SpectrumAnalyserEngine.cpp"
#include "components/spectrum-analyser/jump_SpectrumAnalyser.cpp"
#include "components/spectrum-analyser/jump_MultiAnalyser.cpp"

// Containers

// Interfaces
#ifdef JucePlugin_Name
    #include "interfaces/jump_PluginEditor.cpp"
    #include "interfaces/jump_PluginProcessor.cpp"
#endif

// Graphics
#include "graphics/jump_LookAndFeel.cpp"

// Utilities

// clang-format on
