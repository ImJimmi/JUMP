cmake_minimum_required(VERSION 3.15)

file(READ "version.txt" VERSION)
string(STRIP "${VERSION}" VERSION)

project(demo-plugin
    VERSION ${VERSION}
)

set_property(GLOBAL PROPERTY USE_FOLDERS YES)
set(JUCE_ENABLE_MODULE_SOURCE_GROUPS ON CACHE BOOL "" FORCE)
set(JUCE_COPY_PLUGIN_AFTER_BUILD ON CACHE BOOL "" FORCE)
add_subdirectory(JUCE)

juce_add_module(../JUMP ALIAS_NAMESPACE jump)

option(
    COPY_DIR
    "Where to copy plugins after a build."
    ${CMAKE_CURRENT_SOURCE_DIR}
)

juce_add_plugin(demo-plugin
    PLUGIN_MANUFACTURER_CODE Jump
    PLUGIN_CODE Demo
    FORMATS AU VST3 Standalone

    COMPANY_NAME JUMP

    VST3_COPY_DIR ${COPY_DIR}
    AU_COPY_DIR ${COPY_DIR}
)

set(DEMO_PLUGIN_SOURCES
    source/audio/jumpDemo_AudioProcessor.cpp
    source/audio/jumpDemo_AudioProcessor.h

    source/core/jumpDemo_PluginInstance.cpp
    source/core/jumpDemo_PluginInstance.h

    source/editor/jumpDemo_LookAndFeel.cpp
    source/editor/jumpDemo_LookAndFeel.h
    source/editor/jumpDemo_PluginEditor.cpp
    source/editor/jumpDemo_PluginEditor.h
    source/editor/demos/jumpDemo_CanvasContainer.cpp
    source/editor/demos/jumpDemo_CanvasContainer.h
    source/editor/demos/jumpDemo_LevelMeters.cpp
    source/editor/demos/jumpDemo_LevelMeters.h
    source/editor/demos/jumpDemo_SpectrumAnalysers.cpp
    source/editor/demos/jumpDemo_SpectrumAnalysers.h
)

target_sources(demo-plugin
PRIVATE
    ${DEMO_PLUGIN_SOURCES}
)

target_include_directories(demo-plugin
PRIVATE
    source
)

target_compile_definitions(demo-plugin
PRIVATE
    JUCE_DISPLAY_SPLASH_SCREEN=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
    JUCE_WEB_BROWSER=0
)

if (MSVC)
    target_compile_options(demo-plugin PRIVATE /W4 /WX)
else()
    target_compile_options(demo-plugin PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

target_link_libraries(demo-plugin
PRIVATE
    juce::juce_audio_devices
    juce::juce_audio_utils

    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags

    jump::JUMP
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${DEMO_PLUGIN_SOURCES})

juce_generate_juce_header(demo-plugin)
