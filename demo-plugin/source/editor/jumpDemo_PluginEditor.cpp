#include "jumpDemo_PluginEditor.h"

#include "jumpDemo_LookAndFeel.h"
#include "demos/jumpDemo_CanvasContainer.h"
#include "demos/jumpDemo_LevelMeters.h"

//======================================================================================================================
namespace jumpDemo
{
    //==================================================================================================================
    namespace constants
    {
        //==============================================================================================================
        namespace demoNames
        {
            static const juce::String canvases   { "Canvases" };
            static const juce::String levelMeters{ "Level Meters" };
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
            constants::demoNames::levelMeters
        };
        demoSelector.addItemList(demos,
                                 demoSelector.getNumItems() + 1);
        demoSelector.onChange = [this]() {
            activeDemo = getDemoFromName(demoSelector.getText(), pluginInstance.getSampleRate());
            addAndMakeVisible(*activeDemo);
            resized();
        };
        demoSelector.setSelectedItemIndex(demos.indexOf(constants::demoNames::levelMeters));

        lookAndFeelAccessor.attachTo(this);

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

        juce::Grid grid;
        grid.templateRows = { juce::Grid::Fr {1} };
        grid.templateColumns = lookAndFeelAccessor->getTemplateColumns();

        const auto demoSelectorSize = lookAndFeelAccessor->getDemoSelectorSize();
        const auto demoSelectorMargin = lookAndFeelAccessor->getDemoSelectorMargin().toGridMargin();
        grid.items.add(juce::GridItem{ demoSelector }.withSize(demoSelectorSize.width, demoSelectorSize.height)
                                                     .withMargin(demoSelectorMargin));

        if (activeDemo != nullptr)
            grid.items.add(juce::GridItem{ *activeDemo });

        grid.performLayout(bounds);
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
