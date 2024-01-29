#include "jump_LookAndFeel.h"

#include <set>

//======================================================================================================================
namespace constants
{
    //==================================================================================================================
    constexpr auto widgetCornerRadius = 2.5f;
    constexpr auto widgetBorderThickness = 1.f;

    constexpr char defaultFontFamily[] = "Helvetica";
    constexpr auto defaultFontSize = 12.f;

    //==================================================================================================================
    constexpr auto warningLevelDecibels = -12.f;

    //==================================================================================================================
    constexpr auto levelMeterGridlinesIntervalDecibels = 3.f;
    static const std::set<float> levelMeterMajorGridlineLevels{ 0.f, -6.f, -12.f, -24.f, -48.f };

    constexpr auto levelMeterPeakIndicatorThickness = 2.5f;
    constexpr auto levelMeterCornerSize = 1.f;
    constexpr auto levelMeterGridlineInterval = 3.f;
    constexpr auto multiMeterGapBetweenMeters = 5;

    //==================================================================================================================
    constexpr auto spectrumAnalyserLineThickness = 1.5f;
    constexpr auto spectrumAnalyserGridlinesIntervalDecibels = 12.f;
    constexpr auto spectrumAnalyserFillOpacity = 0.1f;
    constexpr auto spectrumAnalyserLevelLabelMarginX = 10;
    constexpr auto spectrumAnalyserFrequencyLabelMarginBottom = 3;
} // namespace constants

//======================================================================================================================
namespace jump::lookAndFeelImplementations
{
    //==================================================================================================================
    template <typename RectangleType>
    [[nodiscard]] static auto createRoundedRectanglePath(const juce::Rectangle<RectangleType>& bounds, float cornerRadius)
    {
        juce::Path path;
        path.addRoundedRectangle(bounds, cornerRadius);

        return path;
    }

    [[nodiscard]] static auto getDefaultFont(int flags = juce::Font::FontStyleFlags::plain)
    {
        return juce::Font{ constants::defaultFontFamily, constants::defaultFontSize, flags };
    }

    //==================================================================================================================
    static void drawLevelMeterBackground(juce::Graphics& g, const LevelMeter& meter)
    {
        constexpr auto halfBorderThickness = constants::widgetBorderThickness / 2.f;
        const auto pathBounds = meter.getLocalBounds().toFloat().reduced(halfBorderThickness);
        const auto path = createRoundedRectanglePath(pathBounds, constants::widgetCornerRadius - halfBorderThickness);

        g.setColour(meter.findColour(levelMeterBackgroundColourId));
        g.fillPath(path);

        g.setColour(meter.findColour(levelMeterBorderColourId));
        g.strokePath(path, juce::PathStrokeType{ constants::widgetBorderThickness });
    }

    static void drawLevelMeterGridlines(juce::Graphics& g, const LevelMeter& meter)
    {
        const auto clipBounds = meter.getLocalBounds().reduced(1);
        const auto clipPath = createRoundedRectanglePath(clipBounds,
                                                         constants::widgetCornerRadius - constants::widgetBorderThickness);
        g.reduceClipRegion(clipPath);

        g.setColour(meter.findColour(levelMeterGridlinesColourId));

        const auto& decibelRange = meter.getEngine().getDecibelRange();
        const auto orientation = meter.getOrientation();

        for (auto dB = decibelRange.end; dB >= decibelRange.start; dB -= constants::levelMeterGridlinesIntervalDecibels)
        {
            const auto normalisedLevel = decibelRange.convertTo0to1(dB);
            const auto lineThickness = (constants::levelMeterMajorGridlineLevels.count(dB) > 0 ? 2.f : 1.f) * constants::widgetBorderThickness;

            if (orientation == Orientation::vertical)
            {
                const auto y = std::round((1.f - normalisedLevel) * meter.getHeight()) - lineThickness / 2.f;
                g.drawLine({ { 0.f, y }, { static_cast<float>(meter.getWidth()), y } }, lineThickness);
            }
            else if (orientation == Orientation::horizontal)
            {
                const auto x = std::round(normalisedLevel * meter.getWidth()) - lineThickness / 2.f;
                g.drawLine({ { x, 0.f }, { x, static_cast<float>(meter.getHeight()) } }, lineThickness);
            }
            else
            {
                // Unhandled orientation type.
                jassertfalse;
            }
        }
    }

    void LevelMeterLookAndFeel::drawBackground(juce::Graphics& g, const LevelMeter& meter) const noexcept
    {
        drawLevelMeterBackground(g, meter);
        drawLevelMeterGridlines(g, meter);
    }

    [[nodiscard]] static auto createLevelMeterPeakShape(Orientation orientation, const juce::Rectangle<int>& meterBounds,
                                                        float peakLevel)
    {
        juce::Rectangle<float> bounds;

        if (orientation == Orientation::vertical)
        {
            bounds.setWidth(meterBounds.toFloat().getWidth());
            bounds.setHeight(constants::levelMeterPeakIndicatorThickness);

            bounds.setCentre({ meterBounds.toFloat().getCentreX(),
                               meterBounds.getHeight() * (1.f - peakLevel) });
        }
        else if (orientation == Orientation::horizontal)
        {
            bounds.setWidth(constants::levelMeterPeakIndicatorThickness);
            bounds.setHeight(meterBounds.toFloat().getHeight());

            bounds.setCentre({ meterBounds.getWidth() * peakLevel,
                               meterBounds.toFloat().getCentreY() });
        }
        else
        {
            jassertfalse;
        }

        juce::Path path;
        path.addRoundedRectangle(bounds, constants::levelMeterCornerSize);

        return path;
    }

    [[nodiscard]] static auto createLevelMeterRMSShape(Orientation orientation, const juce::Rectangle<int>& meterBounds,
                                                       float rmsLevel)
    {
        juce::Rectangle<float> bounds{ meterBounds.toFloat() };
        juce::Path path;

        if (orientation == Orientation::vertical)
        {
            bounds.setTop(bounds.getHeight() * (1.f - rmsLevel));

            path.addRoundedRectangle(bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight(),
                                     constants::levelMeterCornerSize, constants::levelMeterCornerSize,
                                     true, true, false, false);
        }
        else if (orientation == Orientation::horizontal)
        {
            bounds.setRight(bounds.getWidth() * rmsLevel);

            path.addRoundedRectangle(bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight(),
                                     constants::levelMeterCornerSize, constants::levelMeterCornerSize,
                                     false, true, false, true);
        }
        else
        {
            jassertfalse;
        }

        return path;
    }

    [[nodiscard]] static auto createLevelMeterPath(Orientation orientation, const juce::Rectangle<int> bounds,
                                                   float peakLevel, float rmsLevel)
    {
        juce::Path path;

        path.addPath(createLevelMeterPeakShape(orientation, bounds, peakLevel));
        path.addPath(createLevelMeterRMSShape(orientation, bounds, rmsLevel));

        return path;
    }

    void LevelMeterLookAndFeel::drawLevelMeter(juce::Graphics& g, const LevelMeter& renderer,
                                               float peakLevelNormalised, float rmsLevelNormalised) const noexcept
    {
        const auto clipBounds = renderer.getLocalBounds().reduced(1);
        const auto clipPath = createRoundedRectanglePath(clipBounds,
                                                         constants::widgetCornerRadius - constants::widgetBorderThickness);
        g.reduceClipRegion(clipPath);

        juce::ColourGradient gradient;
        const auto safeColour = renderer.findColour(levelMeterSafeColourId);
        const auto warningColour = renderer.findColour(levelMeterWarningColourId);
        const auto dangerColour = renderer.findColour(levelMeterDangerColourId);

        const auto& decibelRange = renderer.getEngine().getDecibelRange();
        const auto normalisedWarningLevel = decibelRange.convertTo0to1(constants::warningLevelDecibels);

        const auto orientation = renderer.getOrientation();

        if (orientation == Orientation::vertical)
        {
            gradient = juce::ColourGradient::vertical(dangerColour, safeColour, renderer.getLocalBounds());
            gradient.addColour(static_cast<double>(1.f - normalisedWarningLevel), warningColour);
        }
        else if (orientation == Orientation::horizontal)
        {
            gradient = juce::ColourGradient::horizontal(safeColour, dangerColour, renderer.getLocalBounds());
            gradient.addColour(static_cast<double>(normalisedWarningLevel), warningColour);
        }
        else
        {
            // Unhandled orientation.
            jassertfalse;
        }

        g.setGradientFill(gradient);

        const auto meterPath = createLevelMeterPath(orientation, renderer.getLocalBounds(),
                                                    peakLevelNormalised, rmsLevelNormalised);
        g.fillPath(meterPath);
    }

    [[nodiscard]] static auto getLevelMeterTextForLevel(float decibelLevel, bool isNegativeInf)
    {
        if (isNegativeInf)
            return juce::String{ "-inf" };

        return juce::String{ decibelLevel };
    }

    [[nodiscard]] static auto getFontFlagsForLevelMeterText(float decibelLevel, bool isNegativeInf)
    {
        if (decibelLevel == 0.f || isNegativeInf)
            return juce::Font::bold;

        return juce::Font::plain;
    }

    [[nodiscard]] static auto getLevelMeterLabelFont(float decibelLevel, bool isNegativeInf)
    {
        const auto flags = getFontFlagsForLevelMeterText(decibelLevel, isNegativeInf);
        return getDefaultFont(flags);
    }

    [[nodiscard]] static auto getLevelMeterTextColour(const juce::Component& component,
                                                      const float decibelLevel, bool isNegativeInf)
    {
        if (decibelLevel == 0.f || isNegativeInf)
            return component.findColour(levelMeterLabelHighlightedTextColourId);

        return component.findColour(levelMeterLabelNormalTextColourId);
    }

    std::unique_ptr<juce::Label> LevelMeterLookAndFeel::createLabelForLevel(const LevelMeterLabelsComponent& component,
                                                                            float level) const noexcept
    {
        auto label = std::make_unique<juce::Label>();

        const auto decibelRange = component.getEngine().getDecibelRange();
        const auto isNegativeInf = level <= decibelRange.start;
        label->setText(getLevelMeterTextForLevel(level, isNegativeInf), juce::dontSendNotification);
        label->setFont(getLevelMeterLabelFont(level, isNegativeInf));
        label->setColour(juce::Label::textColourId, getLevelMeterTextColour(component, level, isNegativeInf));

        return label;
    }

    int LevelMeterLookAndFeel::getGapBetweenMeters(const MultiMeter&) const noexcept
    {
        return constants::multiMeterGapBetweenMeters;
    }

    //==================================================================================================================
    static void drawSpectrumAnalyserBackground(juce::Graphics& g, const SpectrumAnalyser& analyser)
    {
        const auto bounds = analyser.getLocalBounds().toFloat().reduced(constants::widgetBorderThickness / 2.f);
        const auto path = createRoundedRectanglePath(bounds, constants::widgetCornerRadius);

        g.setColour(analyser.findColour(ColourIds::spectrumAnalyserBackgroundColourId));
        g.fillPath(path);

        g.setColour(analyser.findColour(ColourIds::spectrumAnalyserBorderColourId));
        g.strokePath(path, juce::PathStrokeType{ constants::widgetBorderThickness });
    }

    static void drawSpectrumAnalyserVerticalGridlines(juce::Graphics& g, const SpectrumAnalyser& analyser)
    {
        const auto& freqRange = analyser.getEngine().getFrequencyRange();
        const auto nyquist = analyser.getEngine().getNyquistFrequency();
        const auto maxOrder = static_cast<float>(std::ceil(std::log10(nyquist)));

        const auto bounds = analyser.getLocalBounds().toFloat().reduced(constants::widgetBorderThickness);

        for (auto order = 0.f; order < maxOrder; order++)
        {
            const auto multipleOfTen = std::pow(10.f, order);

            for (auto multiplier = 1.f; multiplier < 10.f; multiplier++)
            {
                const auto freq = multipleOfTen * multiplier;

                if (freq <= freqRange.start)
                    continue;

                if (freq >= freqRange.end)
                    break;

                const auto normalised = math::inverseLogSpace(freqRange.start, freqRange.end, freq);
                const auto x = bounds.getX() + bounds.proportionOfWidth(normalised) - constants::widgetBorderThickness / 2.f;

                g.fillRect(std::round(x), bounds.getY(), constants::widgetBorderThickness, bounds.getHeight());
            }
        }
    }

    static void drawSpectrumAnalyserHorizontalGridlines(juce::Graphics& g, const SpectrumAnalyser& analyser)
    {
        const auto& decibelRange = analyser.getEngine().getDecibelRange();
        const auto maxAbs = juce::jmax(std::abs(decibelRange.start), std::abs(decibelRange.end));

        const auto bounds = analyser.getLocalBounds().toFloat().reduced(constants::widgetBorderThickness);

        for (auto dB = 0.f; dB < maxAbs; dB += constants::spectrumAnalyserGridlinesIntervalDecibels)
        {
            for (auto level : { dB, -dB })
            {
                if (level <= decibelRange.start || level >= decibelRange.end)
                    continue;

                const auto normalised = decibelRange.convertTo0to1(level);
                const auto y = bounds.getBottom() - bounds.proportionOfHeight(normalised) - constants::widgetBorderThickness / 2.f;

                g.fillRect(bounds.getX(), std::round(y), bounds.getWidth(), constants::widgetBorderThickness);
            }
        }
    }

    static void drawSpectrumAnalyserGridlines(juce::Graphics& g, const SpectrumAnalyser& analyser)
    {
        g.setColour(analyser.findColour(spectrumAnalyserGridlinesColourId));

        drawSpectrumAnalyserVerticalGridlines(g, analyser);
        drawSpectrumAnalyserHorizontalGridlines(g, analyser);
    }

    void SpectrumAnalyserLookAndFeel::drawBackground(juce::Graphics& g, const SpectrumAnalyser& analyser) const noexcept
    {
        drawSpectrumAnalyserBackground(g, analyser);
        drawSpectrumAnalyserGridlines(g, analyser);
    }

    [[nodiscard]] static auto denormalisePoint(const juce::Point<float>& normalisedPoint, const juce::Rectangle<float>& bounds)
    {
        return juce::Point{
            bounds.getX() + bounds.proportionOfWidth(normalisedPoint.x),
            bounds.getY() + bounds.proportionOfHeight(normalisedPoint.y)
        };
    }

    [[nodiscard]] static auto getSpectrumAnalyserGradient(const SpectrumAnalyser& analyser)
    {
        auto gradient = juce::ColourGradient::vertical(analyser.findColour(spectrumAnalyserDangerColourId),
                                                       analyser.findColour(spectrumAnalyserSafeColourId),
                                                       analyser.getLocalBounds().reduced(1));

        const auto& decibelRange = analyser.getEngine().getDecibelRange();
        const auto normalisedWarningLevel = 1.0 - static_cast<double>(decibelRange.convertTo0to1(constants::warningLevelDecibels));

        gradient.addColour(normalisedWarningLevel, analyser.findColour(spectrumAnalyserWarningColourId));

        return gradient;
    }

    void SpectrumAnalyserLookAndFeel::drawSpectrumAnalyser(juce::Graphics& g, const SpectrumAnalyser& analyser,
                                                           const std::vector<juce::Point<float>>& points,
                                                           const PaintOptions& paintOptions) const noexcept
    {
        if (points.size() == 0)
            return;

        const auto bounds = analyser.getLocalBounds().reduced(1).toFloat();
        const auto clippingPath = createRoundedRectanglePath(bounds, constants::widgetCornerRadius - 1.f);
        g.reduceClipRegion(clippingPath);

        juce::Path path;
        path.startNewSubPath(bounds.getX() - 10.f, denormalisePoint(points.front(), bounds).y);

        for (const auto& point : points)
        {
            auto scaledPoint = denormalisePoint(point, bounds);

            if (point.y == 1.f)
                scaledPoint.y += constants::spectrumAnalyserLineThickness / 2.f;

            path.lineTo(scaledPoint);
        }

        path.lineTo(bounds.getRight() + 10.f, denormalisePoint(points.back(), bounds).y);
        path.lineTo(bounds.expanded(constants::spectrumAnalyserLineThickness).getBottomRight());
        path.lineTo(bounds.expanded(constants::spectrumAnalyserLineThickness).getBottomLeft());

        g.setGradientFill(getSpectrumAnalyserGradient(analyser));

        if (paintOptions.getShouldFill())
        {
            juce::Graphics::ScopedSaveState sss{ g };
            g.setOpacity(constants::spectrumAnalyserFillOpacity);

            g.fillPath(path);
        }

        using PST = juce::PathStrokeType;
        g.strokePath(path, PST{ constants::spectrumAnalyserLineThickness, PST::curved, PST::rounded });
    }

    [[nodiscard]] static auto getSpectrumAnalyserLevelLabelMargin()
    {
        return Margin{
            0,
            constants::spectrumAnalyserLevelLabelMarginX,
            0,
            constants::spectrumAnalyserLevelLabelMarginX,
        };
    }

    std::unique_ptr<juce::Label> SpectrumAnalyserLookAndFeel::createLabelForLevel(const SpectrumAnalyserLabelsComponent& component,
                                                                                  float level) const noexcept
    {
        auto label = std::make_unique<juce::Label>();

        const auto decibelRange = component.getEngine().getDecibelRange();
        const auto isNegativeInf = level <= decibelRange.start;
        label->setText(getLevelMeterTextForLevel(level, isNegativeInf), juce::dontSendNotification);
        label->setFont(getLevelMeterLabelFont(level, isNegativeInf));
        label->setColour(juce::Label::textColourId, getLevelMeterTextColour(component, level, isNegativeInf));
        label->setBorderSize(getSpectrumAnalyserLevelLabelMargin().toBorderSize());

        return label;
    }

    [[nodiscard]] static auto getSpectrumAnalyserTextForFrequency(float frequency)
    {
        juce::String suffix{ "" };

        if (frequency >= 1000.f)
        {
            frequency /= 1000.f;
            suffix = "k";
        }

        return juce::String{ juce::roundToInt(frequency) } + suffix;
    }

    [[nodiscard]] static auto getSpectrumAnalyserFrequencyLabelMargin()
    {
        return Margin{ 0, 0, constants::spectrumAnalyserFrequencyLabelMarginBottom, 0 };
    }

    std::unique_ptr<juce::Label> SpectrumAnalyserLookAndFeel::createLabelForFrequency(const SpectrumAnalyserLabelsComponent& component,
                                                                                      float frequency) const noexcept
    {
        auto label = std::make_unique<juce::Label>();

        const auto frequencyRange = component.getEngine().getFrequencyRange();
        label->setText(getSpectrumAnalyserTextForFrequency(frequency), juce::dontSendNotification);
        label->setFont(getDefaultFont());
        label->setBorderSize(getSpectrumAnalyserFrequencyLabelMargin().toBorderSize());

        return label;
    }

    //==================================================================================================================
    void SvgLookAndFeel::drawSvgComponent(juce::Graphics& g, const SvgComponent& component) const
    {
        component.getDrawable().drawWithin(g, component.getLocalBounds().toFloat(), juce::RectanglePlacement::centred,
                                           1.f);
    }
} // namespace jump::lookAndFeelImplementations

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    LookAndFeel::LookAndFeel()
        : LookAndFeel{ getMaterialColourScheme() }
    {
    }

    LookAndFeel::LookAndFeel(const ColourScheme& scheme)
    {
        setColourScheme(scheme);
    }

    //==================================================================================================================
    void LookAndFeel::setColourScheme(const ColourScheme& scheme)
    {
        setColour(levelMeterBackgroundColourId, scheme.widgetBackground);
        setColour(levelMeterBorderColourId, scheme.widgetBorder);
        setColour(levelMeterGridlinesColourId, scheme.widgetBorder);
        setColour(levelMeterSafeColourId, scheme.safe);
        setColour(levelMeterWarningColourId, scheme.warning);
        setColour(levelMeterDangerColourId, scheme.danger);
        setColour(levelMeterLabelNormalTextColourId, scheme.textNormal);
        setColour(levelMeterLabelHighlightedTextColourId, scheme.textBold);

        setColour(spectrumAnalyserBackgroundColourId, scheme.widgetBackground);
        setColour(spectrumAnalyserBorderColourId, scheme.widgetBorder);
        setColour(spectrumAnalyserGridlinesColourId, scheme.widgetBorder);
        setColour(spectrumAnalyserSafeColourId, scheme.safe);
        setColour(spectrumAnalyserWarningColourId, scheme.warning);
        setColour(spectrumAnalyserDangerColourId, scheme.danger);

        setColour(juce::ResizableWindow::backgroundColourId, scheme.windowBackground);
        setColour(juce::Label::textColourId, scheme.textNormal);
    }

    LookAndFeel::ColourScheme LookAndFeel::getMaterialColourScheme()
    {
        ColourScheme scheme;

        scheme.windowBackground = MaterialColourPalette::blueGrey850;

        scheme.widgetBackground = MaterialColourPalette::blueGrey800;
        scheme.widgetBorder = MaterialColourPalette::blueGrey900;
        scheme.textNormal = MaterialColourPalette::blueGrey050;
        scheme.textBold = MaterialColourPalette::blueGrey100;

        scheme.safe = MaterialColourPalette::lightGreen500;
        scheme.warning = MaterialColourPalette::yellow500;
        scheme.danger = MaterialColourPalette::red500;

        return scheme;
    }
} // namespace jump
