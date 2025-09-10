#include "JUMP.h"

//======================================================================================================================
// clang-format off

// Audio
#include "audio/jump_Compressor.cpp" // NOLINT

// Components
#include "components/jump_AttributedLabel.cpp" // NOLINT
#include "components/jump_SnapshotGenerator.cpp" // NOLINT
#include "components/jump_SvgComponent.cpp" // NOLINT
#include "components/buttons/jump_BrandLogoButton.cpp" // NOLINT
#include "components/level-meter/jump_LevelMeterEngine.cpp" // NOLINT
#include "components/level-meter/jump_MultiMeter.cpp" // NOLINT
#include "components/spectrum-analyser/jump_SpectrumAnalyserEngine.cpp" // NOLINT
#include "components/spectrum-analyser/jump_SpectrumAnalyser.cpp" // NOLINT
#include "components/spectrum-analyser/jump_MultiAnalyser.cpp" // NOLINT
#include "components/windows/jump_ModalWindow.cpp" // NOLINT

// Containers

// Interfaces
#ifdef JucePlugin_Name
    #include "interfaces/jump_PluginEditor.cpp" // NOLINT
    #include "interfaces/jump_PluginProcessor.cpp" // NOLINT
#endif

// Graphics
#include "graphics/jump_LookAndFeel.cpp" // NOLINT

// Utilities

// clang-format on
