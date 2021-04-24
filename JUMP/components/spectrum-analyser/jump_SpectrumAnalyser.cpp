#pragma once

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    SpectrumAnalyser::SpectrumAnalyser(const SpectrumAnalyserEngine& engineToUse,
                                       const juce::Identifier& uniqueID, StatefulObject* parentState)
        :   StatefulObject{ uniqueID, parentState },
            engine{ engineToUse }
    {
        initialiseState();
        lookAndFeel.attachTo(this);

        addAndMakeVisible(background);
        background.setDrawFunction([this](juce::Graphics& g) {
            lookAndFeel->drawBackground(g, *this);
        });

        addAndMakeVisible(analyser);
        analyser.setDrawFunction([this](juce::Graphics& g) {
            lookAndFeel->drawSpectrumAnalyser(g, *this, analyserPoints, paintOptions);
        });

        engineToUse.addRenderer(this);
    }

    SpectrumAnalyser::~SpectrumAnalyser()
    {
        engine.removeRenderer(this);
    }

    //==================================================================================================================
    void SpectrumAnalyser::setBackgroundVisible(bool backgroundShouldBeVisible)
    {
        setProperty (PropertyIDs::isBackgroundVisibleId, backgroundShouldBeVisible);
    }

    void SpectrumAnalyser::setPaintOptions(const PaintOptions& options)
    {
        setProperty (PropertyIDs::paintOptionsId, options.serialise());
    }

    const SpectrumAnalyserEngine& SpectrumAnalyser::getEngine() const noexcept
    {
        return engine;
    }

    //==================================================================================================================
    void SpectrumAnalyser::resized()
    {
        const auto bounds = getLocalBounds();

        background.setBounds(bounds);
        analyser.setBounds(bounds);
    }

    void SpectrumAnalyser::newSpectrumAnalyserPointsAvailable(const SpectrumAnalyserEngine&,
                                                              const std::vector<juce::Point<float>>& points)
    {
        analyserPoints = points;
        analyser.repaint();
    }

    void SpectrumAnalyser::propertyChanged(const juce::Identifier& name, const juce::var& newValue)
    {
        if (name == PropertyIDs::isBackgroundVisibleId)
            setBackgroundVisibleInternal(static_cast<bool>(newValue));
        else if (name == PropertyIDs::paintOptionsId)
            setPaintOptionsInternal(PaintOptions{ static_cast<juce::int64>(newValue) });
    }

    //==================================================================================================================
    void SpectrumAnalyser::initialiseState()
    {
        setBackgroundVisible(true);
        setPaintOptions(PaintOptions{ PaintOptions::strokeAndFill });
    }

    //==================================================================================================================
    void SpectrumAnalyser::setBackgroundVisibleInternal (bool shouldBeVisible)
    {
        background.setVisible(shouldBeVisible);
    }

    void SpectrumAnalyser::setPaintOptionsInternal(const PaintOptions& options)
    {
        paintOptions = options;
        repaint();
    }
}   // namespace jump
