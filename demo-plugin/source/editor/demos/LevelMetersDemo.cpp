#include "LevelMetersDemo.h"

#include <JuceHeader.h>

//======================================================================================================================
void setupInputMeters(jump::LevelMeterComponent& meters, jump::LevelMeterComponent::LabelsPosition labelsPosition)
{
    meters.setOrientation(jump::LevelMeter::Orientation::Horizontal);
    meters.setLabelsPosition(labelsPosition);
    meters.setColour(jump::LookAndFeel::levelMeterSafeColourId, jump::LookAndFeel::ColourPalette::blueGrey400);
    meters.setColour(jump::LookAndFeel::levelMeterWarningColourId, jump::LookAndFeel::ColourPalette::blueGrey300);
    meters.setColour(jump::LookAndFeel::levelMeterDangerColourId, jump::LookAndFeel::ColourPalette::blueGrey200);
}

void setupOutputMeters(jump::LevelMeterComponent& meters, jump::LevelMeterComponent::LabelsPosition labelsPosition)
{
    meters.setLabelsPosition(labelsPosition);
}

juce::Font getLabelsFont()
{
    return {"Helvetica", 24.f, 0};
}

LevelMetersDemo::LevelMetersDemo()
{
    // Input meters.
    addAndMakeVisible(inputMetersLabelsBelow);
    setupInputMeters(inputMetersLabelsBelow, jump::LevelMeterComponent::LabelsPosition::Below);

    addAndMakeVisible(inputMetersLabelsCentred);
    setupInputMeters(inputMetersLabelsCentred, jump::LevelMeterComponent::LabelsPosition::Centred);

    addAndMakeVisible(inputMetersLabelsAbove);
    setupInputMeters(inputMetersLabelsAbove, jump::LevelMeterComponent::LabelsPosition::Above);

    // Output meters.
    addAndMakeVisible(outputMetersLabelsRight);
    setupOutputMeters(outputMetersLabelsRight, jump::LevelMeterComponent::LabelsPosition::Right);

    addAndMakeVisible(outputMetersLabelsCentred);
    setupOutputMeters(outputMetersLabelsCentred, jump::LevelMeterComponent::LabelsPosition::Centred);

    addAndMakeVisible(outputMetersLabelsLeft);
    setupOutputMeters(outputMetersLabelsLeft, jump::LevelMeterComponent::LabelsPosition::Left);

    addAndMakeVisible(inputLabel);
    inputLabel.setJustificationType(juce::Justification::centred);
    inputLabel.setFont(getLabelsFont());

    addAndMakeVisible(outputLabel);
    outputLabel.setJustificationType(juce::Justification::centred);
    outputLabel.setFont(getLabelsFont());
}

LevelMetersDemo::~LevelMetersDemo()
{
}

//======================================================================================================================
void LevelMetersDemo::setSampleRate(double sampleRate)
{
    inputEngineLeft.setSampleRate(sampleRate);
    inputEngineRight.setSampleRate(sampleRate);
    outputEngineLeft.setSampleRate(sampleRate);
    outputEngineRight.setSampleRate(sampleRate);
}

void LevelMetersDemo::addInputSamples(const std::array<std::vector<float>, 2>& samples)
{
    inputEngineLeft.addSamples(samples.front());
    inputEngineRight.addSamples(samples.back());
}

void LevelMetersDemo::addOutputSamples(const std::array<std::vector<float>, 2>& samples)
{
    outputEngineLeft.addSamples(samples.front());
    outputEngineRight.addSamples(samples.back());
}

//======================================================================================================================
void LevelMetersDemo::resized()
{
    constexpr auto gap = 20;
    auto bounds = getLocalBounds().reduced(gap);

    {
        const auto height = std::ceil(inputLabel.getFont().getHeight()) + inputLabel.getBorderSize().getTopAndBottom();
        auto row = bounds.removeFromTop(static_cast<int>(height));

        inputLabel.setBounds(row.removeFromLeft(row.getWidth() / 2));
        outputLabel.setBounds(row);
    }

    bounds.removeFromTop(gap);

    {
        juce::FlexBox flex;
        flex.flexDirection = juce::FlexBox::Direction::column;

        flex.items = {
            juce::FlexItem{ inputMetersLabelsBelow }.withFlex(1.f),
            juce::FlexItem{ inputMetersLabelsCentred }.withFlex(1.f).withMargin({ gap, 0.f, gap, 0.f }),
            juce::FlexItem{ inputMetersLabelsAbove }.withFlex(1.f)
        };

        auto inputBounds = bounds.removeFromLeft(bounds.getWidth() / 2);
        inputBounds.removeFromRight (gap / 2);
        flex.performLayout(inputBounds);
    }

    {
        juce::FlexBox flex;

        flex.items = {
            juce::FlexItem{ outputMetersLabelsRight }.withFlex(1.f),
            juce::FlexItem{ outputMetersLabelsCentred }.withFlex(1.f).withMargin({ 0.f, gap, 0.f, gap }),
            juce::FlexItem{ outputMetersLabelsLeft }.withFlex(1.f)
        };

        auto outputBounds = bounds;
        outputBounds.removeFromLeft (gap / 2);
        flex.performLayout(outputBounds);
    }
}
