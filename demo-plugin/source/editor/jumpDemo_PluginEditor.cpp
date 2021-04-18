#include "jumpDemo_PluginEditor.h"

#include "jumpDemo_LookAndFeel.h"
#include "demos/jumpDemo_CanvasContainer.h"
#include "demos/jumpDemo_LevelMeters.h"
#include "demos/jumpDemo_SpectrumAnalysers.h"

//======================================================================================================================
namespace jumpDemo
{
    //==================================================================================================================
    namespace constants
    {
        //==============================================================================================================
        namespace demoNames
        {
            static const juce::String canvases         { "Canvases" };
            static const juce::String levelMeters      { "Level Meters" };
            static const juce::String spectrumAnalysers{ "Spectrum Analysers" };
        }   // namespace DemoNames
    }   // namespace Constants

    //==================================================================================================================
    std::unique_ptr<juce::Component> getDemoFromName(const juce::String& demoName, double sampleRate)
    {
        if (demoName == constants::demoNames::canvases)
            return std::make_unique<CanvasContainer>();
        if (demoName == constants::demoNames::levelMeters)
        {
            auto demo = std::make_unique<LevelMeters>();
            demo->setSampleRate(sampleRate);
            return demo;
        }
        if (demoName == constants::demoNames::spectrumAnalysers)
        {
            auto demo = std::make_unique<SpectrumAnalysers>();
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
            pluginInstance{ p },
            lookAndFeel{ std::make_unique<LookAndFeel>() }
    {
        setLookAndFeel(lookAndFeel.get());

        addAndMakeVisible(backgroundCanvas);
        backgroundCanvas.setDrawFunction([this](juce::Graphics& g) {
            g.fillAll(findColour(juce::ResizableWindow::backgroundColourId));
        });

        addAndMakeVisible(demoSelector);
        demoSelector.setEditableText(false);
        const juce::StringArray demos {
            constants::demoNames::canvases,
            constants::demoNames::levelMeters,
            constants::demoNames::spectrumAnalysers
        };
        demoSelector.addItemList(demos,
                                 demoSelector.getNumItems() + 1);
        demoSelector.onChange = [this]() {
            activeDemo = getDemoFromName(demoSelector.getText(), pluginInstance.getSampleRate());
            addAndMakeVisible(*activeDemo);
            resized();
        };
        demoSelector.setSelectedItemIndex(demos.indexOf(constants::demoNames::levelMeters));

        addAndMakeVisible(separator);
        separator.setDrawFunction([this](juce::Graphics& g) {
            lookAndFeelAccessor->drawSeparator(g, separator);
        });

        lookAndFeelAccessor.attachTo(this);

        setSize(870, 320);
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

        juce::Grid grid;
        grid.templateRows = { juce::Grid::Fr {1} };
        grid.templateColumns = lookAndFeelAccessor->getTemplateColumns();

        const auto demoSelectorSize = lookAndFeelAccessor->getDemoSelectorSize();
        const auto demoSelectorMargin = lookAndFeelAccessor->getDemoSelectorMargin().toGridMargin();
        grid.items.add(juce::GridItem{ demoSelector }.withSize(demoSelectorSize.width, demoSelectorSize.height)
                                                     .withMargin(demoSelectorMargin));

        grid.items.add(juce::GridItem{ separator });

        if (activeDemo != nullptr)
            grid.items.add(juce::GridItem{ *activeDemo });

        grid.performLayout(bounds);
    }

    void PluginEditor::timerCallback()
    {
        if (auto levelMetersDemo = dynamic_cast<LevelMeters*>(activeDemo.get()))
        {
            const auto inputSamples = pluginInstance.getLatestInputSamples();
            const auto outputSamples = pluginInstance.getLatestOutputSamples();

            levelMetersDemo->addInputSamples(inputSamples);
            levelMetersDemo->addOutputSamples(outputSamples);
        }
        else if (auto spectrumAnalysersDemo = dynamic_cast<SpectrumAnalysers*>(activeDemo.get()))
        {
            const auto inputSamples = pluginInstance.getLatestInputSamplesMono();
            const auto outputSamples = pluginInstance.getLatestOutputSamplesMono();

            spectrumAnalysersDemo->addInputSamples(inputSamples);
            spectrumAnalysersDemo->addOutputSamples(outputSamples);
        }
    }
} // namespace jumpDemo
