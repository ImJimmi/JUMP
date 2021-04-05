#include "jumpDemo_PluginEditor.h"

#include "demos/jumpDemo_CanvasContainer.h"
#include "demos/jumpDemo_LevelMeters.h"

//======================================================================================================================
namespace jumpDemo
{
    //==================================================================================================================
    namespace Constants
    {
        //==============================================================================================================
        namespace DemoNames
        {
            static const juce::String canvases   { "Canvases" };
            static const juce::String levelMeters{ "Level Meters" };
        }   // namespace DemoNames

        //==============================================================================================================
        static constexpr auto demoSelectorColumnWidth = 180;
    }   // namespace Constants

    //==================================================================================================================
    std::unique_ptr<juce::Component> getDemoFromName(const juce::String& demoName, double sampleRate)
    {
        if (demoName == Constants::DemoNames::canvases)
            return std::make_unique<CanvasContainer>();
        if (demoName == Constants::DemoNames::levelMeters)
        {
            auto demo = std::make_unique<LevelMeters>();
            demo->setSampleRate(sampleRate);
            return demo;
        }

        // Unhandled item index.
        jassertfalse;
        return {};
    }

    //==================================================================================================================
    PluginEditor::PluginEditor(PluginInstance& p)
        :   jump::PluginEditor{ p },
            pluginInstance{ p }
    {
        setLookAndFeel(&jumpLookAndFeel);
        addAndMakeVisible(backgroundCanvas);
        backgroundCanvas.setDrawFunction([this](juce::Graphics& g) {
            g.fillAll(findColour(juce::ResizableWindow::backgroundColourId));

            g.setColour(juce::Colours::white.withAlpha(0.2f));
            g.drawVerticalLine(Constants::demoSelectorColumnWidth, 0.f, static_cast<float>(getHeight()));
        });

        addAndMakeVisible(demoSelector);
        demoSelector.setEditableText(false);
        const juce::StringArray demos {
            Constants::DemoNames::canvases,
            Constants::DemoNames::levelMeters
        };
        demoSelector.addItemList(demos,
                                 demoSelector.getNumItems() + 1);
        demoSelector.onChange = [this]() {
            activeDemo = getDemoFromName(demoSelector.getText(), pluginInstance.getSampleRate());
            addAndMakeVisible(*activeDemo);
            resized();
        };
        demoSelector.setSelectedItemIndex(demos.indexOf(Constants::DemoNames::levelMeters));

        setSize(720, 360);
        startTimerHz(60);
    }

    PluginEditor::~PluginEditor()
    {
        setLookAndFeel(nullptr);
    }

    //==================================================================================================================
    void PluginEditor::resized()
    {
        auto bounds = getLocalBounds();
        backgroundCanvas.setBounds(bounds);

        {
            auto column = bounds.removeFromLeft(Constants::demoSelectorColumnWidth);
            demoSelector.setBounds(column.removeFromTop(45).reduced(10));
            bounds.removeFromLeft(1);
        }

        if (activeDemo != nullptr)
            activeDemo->setBounds(bounds);
    }

    void PluginEditor::timerCallback()
    {
        const auto inputSamples = pluginInstance.getLatestInputSamples();
        const auto outputSamples = pluginInstance.getLatestOutputSamples();

        if (auto levelMetersDemo = dynamic_cast<LevelMeters*>(activeDemo.get()))
        {
            levelMetersDemo->addInputSamples(inputSamples);
            levelMetersDemo->addOutputSamples(outputSamples);
        }
    }
} // namespace jumpDemo
