/** BEGIN_JUCE_MODULE_DECLARATION

    ID:                 JUMP
    vendor:             James Johnson
    version:            0.1.0
    name:               JUMP
    description:        James' Utilities Module for Plugins.
    website:            https://github.com/ImJimmi/JUMP
    license:            GNU3

    dependencies:       juce_dsp

    END_JUCE_MODULE_DECLARATION
*/

//======================================================================================================================
#include <cassert>

#include <juce_dsp/juce_dsp.h>

//======================================================================================================================
// Utilities
#include "utilities/jump_CircularBuffer.h"
#include "utilities/jump_Functions.h"

// Audio
#include "audio/jump_AudioTransferManager.h"

// Engines
#include "engines/jump_AudioComponentEngine.h"
#include "engines/jump_SpectrumAnalyserEngine.h"
