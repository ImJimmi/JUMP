#include "jumpDemo_LevelMeters.h"

#include <JuceHeader.h>

//======================================================================================================================
namespace jumpDemo
{
    //==================================================================================================================
    void setupInputMeter(jump::MultiMeter& multiMeter, jump::MultiMeter::LabelsPosition labelsPosition)
    {
        multiMeter.setOrientation(jump::Orientation::horizontal);
        multiMeter.setLabelsPosition(labelsPosition);
        multiMeter.setColour(jump::LookAndFeel::levelMeterSafeColourId, jump::LookAndFeel::ColourPalette::blueGrey400);
        multiMeter.setColour(jump::LookAndFeel::levelMeterWarningColourId, jump::LookAndFeel::ColourPalette::blueGrey300);
        multiMeter.setColour(jump::LookAndFeel::levelMeterDangerColourId, jump::LookAndFeel::ColourPalette::blueGrey200);
    }

    void setupOutputMeter(jump::MultiMeter& meters, jump::MultiMeter::LabelsPosition labelsPosition)
    {
        meters.setLabelsPosition(labelsPosition);
    }

    juce::Font getLabelsFont()
    {
        return { "Helvetica", 24.f, 0 };
    }

    LevelMeters::LevelMeters()
    {
        // Input meters.
        addAndMakeVisible(inputMetersLabelsBelow);
        setupInputMeter(inputMetersLabelsBelow, jump::MultiMeter::LabelsPosition::below);

        addAndMakeVisible(inputMetersLabelsCentred);
        setupInputMeter(inputMetersLabelsCentred, jump::MultiMeter::LabelsPosition::centred);

        addAndMakeVisible(inputMetersLabelsAbove);
        setupInputMeter(inputMetersLabelsAbove, jump::MultiMeter::LabelsPosition::above);

        // Output meters.
        addAndMakeVisible(outputMetersLabelsRight);
        setupOutputMeter(outputMetersLabelsRight, jump::MultiMeter::LabelsPosition::right);

        addAndMakeVisible(outputMetersLabelsCentred);
        setupOutputMeter(outputMetersLabelsCentred, jump::MultiMeter::LabelsPosition::centred);

        addAndMakeVisible(outputMetersLabelsLeft);
        setupOutputMeter(outputMetersLabelsLeft, jump::MultiMeter::LabelsPosition::left);

        addAndMakeVisible(inputLabel);
        inputLabel.setJustificationType(juce::Justification::centred);
        inputLabel.setFont(getLabelsFont());

        addAndMakeVisible(outputLabel);
        outputLabel.setJustificationType(juce::Justification::centred);
        outputLabel.setFont(getLabelsFont());
    }

    LevelMeters::~LevelMeters()
    {
    }

    //==================================================================================================================
    void LevelMeters::setSampleRate(double sampleRate)
    {
        inputEngineLeft.setSampleRate(sampleRate);
        inputEngineRight.setSampleRate(sampleRate);
        outputEngineLeft.setSampleRate(sampleRate);
        outputEngineRight.setSampleRate(sampleRate);
    }

    void LevelMeters::addInputSamples(const std::array<std::vector<float>, 2>& samples)
    {
        inputEngineLeft.addSamples(samples.front());
        inputEngineRight.addSamples(samples.back());
    }

    void LevelMeters::addOutputSamples(const std::array<std::vector<float>, 2>& samples)
    {
        outputEngineLeft.addSamples(samples.front());
        outputEngineRight.addSamples(samples.back());
    }

    //==================================================================================================================
    void LevelMeters::resized()
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
            inputBounds.removeFromRight(gap / 2);
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
            outputBounds.removeFromLeft(gap / 2);
            flex.performLayout(outputBounds);
        }
    }
}   // namespace jumpDemo
