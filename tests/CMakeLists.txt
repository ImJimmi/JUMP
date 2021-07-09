project("JUMP Tests" VERSION 1.0.0)

juce_add_console_app(jump-tests)

target_sources(jump-tests
PRIVATE
    src/jump-tests/main.cpp
)
