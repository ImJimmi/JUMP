juce_add_module(JUMP ALIAS_NAMESPACE jump)

# ==============================================================================
option(JUMP_BUILD_TESTS_TARGET
    "Whether or not to build a target for JUMP's suite of tests."
    Off
)

if (JUMP_BUILD_TESTS_TARGET)
    set(CMAKE_MODULE_PATH
        ${CMAKE_MODULE_PATH}
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake
    )

    include(jump_warnings)
    add_subdirectory(tests)
endif()
