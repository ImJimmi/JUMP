juce_add_module(JUMP ALIAS_NAMESPACE jump)

# ==============================================================================
option(JUMP_BUILD_TESTS_TARGET
    "Whether or not to build a target for JUMP's suite of tests."
    Off
)

if (JUMP_BUILD_TESTS_TARGET)
    add_subdirectory(tests)
endif()
