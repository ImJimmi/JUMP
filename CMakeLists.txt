cmake_minimum_required(VERSION 3.12)

option(
    BUILD_DEMO_PLUGIN,
    "Whether or not to build the demo plugin included with JUMP."
    Off
)

if (BUILD_DEMO_PLUGIN)
    project(JUMP)
    add_subdirectory(demo-plugin)
else()
    juce_add_module(JUMP ALIAS_NAMESPACE jump)
endif()
