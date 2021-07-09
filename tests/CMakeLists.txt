project("JUMP Tests" VERSION 1.0.0)

# ==============================================================================
set(JUMP_TEST_SOURCES
    source/jump-tests/audio/jump_LevelTests.cpp

    source/jump-tests/jump_TestConstants.h
    source/jump-tests/main.cpp
)

# ==============================================================================
juce_add_console_app(jump-tests)

target_sources(jump-tests
PRIVATE
    ${JUMP_TEST_SOURCES}
)

target_link_libraries(jump-tests
PRIVATE
    juce::juce_core
    jump::JUMP
    jump_warnings
)

target_compile_features(jump-tests
PRIVATE
    cxx_std_17
)

target_include_directories(jump-tests
PRIVATE
    source
)


source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${JUMP_TEST_SOURCES})
