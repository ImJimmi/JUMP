#include "jump_LookAndFeel.h"

#include <set>

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    namespace Constants
    {
        //==============================================================================================================
        constexpr auto widgetCornerRadius = 2.5f;
        constexpr auto widgetBorderThickness = 1.f;

        //==============================================================================================================
        static const std::set<float> levelMeterMajorGridlineLevels{ 0.f, -6.f, -12.f, -24.f, -48.f };

        //==============================================================================================================
        constexpr auto levelMeterPeakIndicatorThickness = 2.5f;
        constexpr auto levelMeterCornerSize = 1.f;
        constexpr auto levelMeterGridlineInterval = 3.f;
    }   // namespace Constants

    //==================================================================================================================
    LookAndFeel::LookAndFeel()
    {
        setColour(widgetBackgroundColourId, ColourPalette::blueGrey800);
        setColour(widgetBorderColourId,     ColourPalette::blueGrey900);

        setColour(levelMeterSafeColourId,    ColourPalette::lightGreen500);
        setColour(levelMeterWarningColourId, ColourPalette::yellow500);
        setColour(levelMeterDangerColourId,  ColourPalette::red500);
        setColour(levelMeterLabelNormalTextColourId,  ColourPalette::blueGrey050);
        setColour(levelMeterLabelHighlightedTextColourId,  ColourPalette::blueGrey100);

        setColour(juce::ResizableWindow::backgroundColourId, ColourPalette::blueGrey850);
        setColour(juce::Label::textColourId, ColourPalette::blueGrey050);
    }

    //==================================================================================================================
    //=========================================//
    // LevelMeterComponent::LookAndFeelMethods //
    //=========================================//
    int LookAndFeel::getLevelMetersGap(const LevelMeterComponent&) const noexcept
    {
        return 5;
    }

    //=====================================================//
    // LevelMeter::BackgroundComponent::LookAndFeelMethods //
    //=====================================================//
    juce::Path createBasicWidgetShape(const juce::Rectangle<float>& bounds, float cornerRadiusModifier = 0.f)
    {
        juce::Path path;
        path.addRoundedRectangle(bounds, Constants::widgetCornerRadius + cornerRadiusModifier);

        return path;
    }

    juce::Path getLevelMeterShape(const juce::Rectangle<float>& meterBounds, float cornerRadiusModifier = 0.f)
    {
        return createBasicWidgetShape(meterBounds, cornerRadiusModifier);
    }

    void LookAndFeel::drawLevelMeterBackground(juce::Graphics& g,
                                               const LevelMeter::BackgroundComponent& component) const
    {
        const auto widgetShape = getLevelMeterShape(component.getLocalBounds().toFloat().reduced(0.5f));

        g.setColour(component.findColour(ColourIds::widgetBackgroundColourId));
        g.fillPath(widgetShape);

        g.setColour(component.findColour(ColourIds::widgetBorderColourId));
        g.strokePath(widgetShape, juce::PathStrokeType{ Constants::widgetBorderThickness });
    }

    float LookAndFeel::getLevelMeterGridlineInterval(const LevelMeter::BackgroundComponent&) const
    {
        return Constants::levelMeterGridlineInterval;
    }

    void LookAndFeel::drawLevelMeterGridline(juce::Graphics& g, const LevelMeter::BackgroundComponent& component,
                                            float normalisedLevel, float decibelLevel,
                                            LevelMeter::Orientation orientation) const
    {
        const auto meterShape = getLevelMeterShape(component.getLocalBounds().reduced(1).toFloat());
        g.reduceClipRegion(meterShape);

        if (normalisedLevel == 0.f || normalisedLevel == 1.f)
            return;

        g.setColour(component.findColour(ColourIds::widgetBorderColourId));

        const auto lineThickness = Constants::levelMeterMajorGridlineLevels.count(decibelLevel) > 0 ? 1.5f : 1.f;

        if (orientation == LevelMeter::Orientation::Vertical)
        {
            const auto y = std::round((1.f - normalisedLevel) * component.getHeight()) - lineThickness / 2.f;
            g.drawLine({ { 0.f, y }, { static_cast<float>(component.getWidth()), y } }, lineThickness);
        }
        else if (orientation == LevelMeter::Orientation::Horizontal)
        {
            const auto x = std::round(normalisedLevel * component.getWidth()) - lineThickness / 2.f;
            g.drawLine({ { x, 0.f }, { x, static_cast<float>(component.getHeight()) } }, lineThickness);
        }
        else
        {
            jassertfalse;
        }
    }

    //=================================================//
    // LevelMeter::DefaultRenderer::LookAndFeelMethods //
    //=================================================//
    juce::Path createLevelMeterPeakShape(LevelMeter::Orientation orientation, const juce::Rectangle<int>& meterBounds,
                                         float peakLevel)
    {
        juce::Rectangle<float> bounds;

        if (orientation == LevelMeter::Orientation::Vertical)
        {
            bounds.setWidth(meterBounds.toFloat().getWidth());
            bounds.setHeight(Constants::levelMeterPeakIndicatorThickness);

            bounds.setCentre(
                meterBounds.toFloat().getCentreX(),
                meterBounds.getHeight() * (1.f - peakLevel)
            );
        }
        else if (orientation == LevelMeter::Orientation::Horizontal)
        {
            bounds.setWidth(Constants::levelMeterPeakIndicatorThickness);
            bounds.setHeight(meterBounds.toFloat().getHeight());

            bounds.setCentre(
                meterBounds.getWidth() * peakLevel,
                meterBounds.toFloat().getCentreY()
            );
        }
        else
        {
            jassertfalse;
        }

        juce::Path path;
        path.addRoundedRectangle(bounds, Constants::levelMeterCornerSize);

        return path;
    }

    juce::Path createLevelMeterRMSShape(LevelMeter::Orientation orientation, const juce::Rectangle<int>& meterBounds,
                                        float rmsLevel)
    {
        juce::Rectangle<float> bounds{ meterBounds.toFloat() };
        juce::Path path;

        if (orientation == LevelMeter::Orientation::Vertical)
        {
            bounds.setTop(bounds.getHeight() * (1.f - rmsLevel));

            path.addRoundedRectangle(bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight(),
                                     Constants::levelMeterCornerSize, Constants::levelMeterCornerSize,
                                     true, true, false, false);
        }
        else if (orientation == LevelMeter::Orientation::Horizontal)
        {
            bounds.setRight(bounds.getWidth() * rmsLevel);

            path.addRoundedRectangle(bounds.getX(), bounds.getY(), bounds.getWidth(), bounds.getHeight(),
                                     Constants::levelMeterCornerSize, Constants::levelMeterCornerSize,
                                     false, true, false, true);
        }
        else
        {
            jassertfalse;
        }

        return path;
    }

    juce::Path LookAndFeel::createLevelMeterPath(const juce::Component& component, LevelMeter::Orientation orientation,
                                                 float peakLevel, float rmsLevel)
    {
        juce::Path path;

        path.addPath(createLevelMeterPeakShape(orientation, component.getLocalBounds(), peakLevel));
        path.addPath(createLevelMeterRMSShape (orientation, component.getLocalBounds(), rmsLevel));

        return path;
    }

    void LookAndFeel::drawLevelMeter(juce::Graphics& g, const juce::Component& component, LevelMeter::Orientation orientation,
                                     const juce::NormalisableRange<float>& decibelRange, const juce::Path& meterPath)
    {
        const auto cornerRadiusModifier = -1.f;
        const auto meterShape = getLevelMeterShape(component.getLocalBounds().reduced(1).toFloat(), cornerRadiusModifier);
        g.reduceClipRegion(meterShape);

        juce::ColourGradient gradient;
        const auto safeColour = component.findColour(levelMeterSafeColourId);
        const auto warningColour = component.findColour(levelMeterWarningColourId);
        const auto dangerColour = component.findColour(levelMeterDangerColourId);

        const auto normalisedNegative12 = decibelRange.convertTo0to1(-12.f);

        if (orientation == LevelMeter::Orientation::Vertical)
        {
            gradient = juce::ColourGradient::vertical(dangerColour, safeColour, component.getLocalBounds());
            gradient.addColour(static_cast<double>(1.f - normalisedNegative12), warningColour);
        }
        else if (orientation == LevelMeter::Orientation::Horizontal)
        {
            gradient = juce::ColourGradient::horizontal(safeColour, dangerColour, component.getLocalBounds());
            gradient.addColour(static_cast<double>(normalisedNegative12), warningColour);
        }

        g.setGradientFill(gradient);
        g.fillPath(meterPath);
    }

    //=================================================//
    // LevelMeter::LabelsComponent::LookAndFeelMethods //
    //=================================================//
    juce::String LookAndFeel::getLevelMeterTextForLevel(float decibelLevel, bool isNegativeInf) const
    {
        static const juce::String units{ "dB" };

        if (isNegativeInf)
            return "-inf" + units;

        return juce::String{ decibelLevel } + units;
    }

    juce::Font::FontStyleFlags getFontFlagsForLevelMeterText(float decibelLevel, bool isNegativeInf)
    {
        if (decibelLevel == 0.f || isNegativeInf)
            return juce::Font::bold;

        return juce::Font::plain;
    }

    juce::Font LookAndFeel::getLevelMeterTextFont(float decibelLevel, bool isNegativeInf) const
    {
        const auto flags = getFontFlagsForLevelMeterText(decibelLevel, isNegativeInf);
        return { "Helvetica", 12.f, flags };
    }

    juce::Colour LookAndFeel::getLevelMeterTextColour(float decibelLevel, bool isNegativeInf) const
    {
        if (decibelLevel == 0.f || isNegativeInf)
            return findColour(levelMeterLabelHighlightedTextColourId);

        return findColour(levelMeterLabelNormalTextColourId);
    }
}   // namespace jump
