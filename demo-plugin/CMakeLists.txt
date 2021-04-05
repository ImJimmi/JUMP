cmake_minimum_required(VERSION 3.15)

file(READ "version.txt" VERSION)
string(STRIP "${VERSION}" VERSION)

project(demo-plugin
    VERSION ${VERSION}
)

set_property(GLOBAL PROPERTY USE_FOLDERS YES)
set(JUCE_ENABLE_MODULE_SOURCE_GROUPS ON CACHE BOOL "" FORCE)
add_subdirectory(JUCE)

add_subdirectory(../ JUMP)

juce_add_plugin(demo-plugin
    PLUGIN_MANUFACTURER_CODE Me
    PLUGIN_CODE Demo
    FORMATS AU VST3 Standalone
    
    COPY_PLUGIN_AFTER_BUILD True
    VST3_COPY_DIR ${CMAKE_CURRENT_SOURCE_DIR}
    AU_COPY_DIR ${CMAKE_CURRENT_SOURCE_DIR}
)

set(DEMO_PLUGIN_SOURCES
    source/audio/jumpDemo_AudioProcessor.cpp
    source/audio/jumpDemo_AudioProcessor.h

    source/core/jumpDemo_PluginInstance.cpp
    source/core/jumpDemo_PluginInstance.h

    source/editor/jumpDemo_PluginEditor.cpp
    source/editor/jumpDemo_PluginEditor.h
    source/editor/demos/jumpDemo_CanvasContainer.cpp
    source/editor/demos/jumpDemo_CanvasContainer.h
    source/editor/demos/jumpDemo_LevelMeters.cpp
    source/editor/demos/jumpDemo_LevelMeters.h
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
