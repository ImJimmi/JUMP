#include "jump_LevelMeterComponent.h"

//======================================================================================================================
namespace juce
{
    //==================================================================================================================
    template <>
    struct VariantConverter<jump::LevelMeterComponent::LabelsPosition>
    {
        //==============================================================================================================
        static jump::LevelMeterComponent::LabelsPosition fromVar(const juce::var& v)
        {
            return static_cast<jump::LevelMeterComponent::LabelsPosition>(static_cast<int>(v));
        }

        static juce::var toVar(const jump::LevelMeterComponent::LabelsPosition& position)
        {
            return { static_cast<int>(position) };
        }
    };
}   // namespace juce

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    LevelMeterComponent::LevelMeterComponent(const std::vector<LevelMeter::Engine*>& enginesToUse,
                                             juce::Identifier type, StatefulObject* parentState)
        :   StatefulObject{ type, parentState },
            mainEngine{ *enginesToUse.front() },
            labels{ *enginesToUse.front() }
    {
        initialiseState();

        for (auto& engine : enginesToUse)
        {
            auto renderer = std::make_unique<LevelMeter::DefaultRenderer>(*engine);
            addAndMakeVisible(*renderer);
            meters.add(std::move(renderer));
        }

        addAndMakeVisible(labels);
    }

    //==================================================================================================================
    void LevelMeterComponent::setLabelsPosition(LabelsPosition newPosition)
    {
        setProperty(PropertyIDs::labelsPositionId, juce::VariantConverter<LabelsPosition>::toVar(newPosition));
    }

    void LevelMeterComponent::setShowLabels(bool shouldShowLabels)
    {
        setProperty(PropertyIDs::labelsVisibleId, shouldShowLabels);
    }

    void LevelMeterComponent::setDecibelLevelsForLabels(const std::vector<float>& newLabelLevels)
    {
        setProperty(PropertyIDs::highlightedLevelsId, juce::VariantConverter<std::vector<float>>::toVar(newLabelLevels));
    }

    void LevelMeterComponent::setOrientation(LevelMeter::Orientation newOrientation)
    {
        setProperty(PropertyIDs::orientationId, juce::VariantConverter<LevelMeter::Orientation>::toVar(newOrientation));
    }

    //==================================================================================================================
    juce::StringArray getTemplateAreasForLayout(LevelMeter::Orientation orientation,
                                                LevelMeterComponent::LabelsPosition position, int numMeters,
                                                const juce::String& meterAreaName, const juce::String& labelsAreaName)
    {
        juce::StringArray result;

        if (orientation == LevelMeter::Orientation::Vertical)
        {
            juce::String row;

            for (auto i = 0; i < numMeters; i++)
            {
                row += meterAreaName + juce::String{ i };

                if (i != numMeters - 1)
                    row += " ";
            }

            result.add(row);
        }
        else if (orientation == LevelMeter::Orientation::Horizontal)
        {
            for (auto i = 0; i < numMeters; i++)
                result.add(meterAreaName + juce::String{ i });
        }
        else
        {
            // Unhandled orientation.
            jassertfalse;
        }

        return result;
    }

    juce::Array<juce::Grid::TrackInfo> getTrackInfoArray(LevelMeter::Orientation orientation,
                                                         LevelMeter::Orientation orientationForSingleElement,
                                                         int numElementsOtherwise)
    {
        using Fr = juce::Grid::Fr;

        if (orientation == orientationForSingleElement)
            return { { Fr{1} } };

        juce::Array<juce::Grid::TrackInfo> result;

        for (auto i = 0; i < numElementsOtherwise; i++)
            result.add(Fr{ 1 });

        return result;
    }

    juce::Array<juce::Grid::TrackInfo> getTemplateColumnsForLayout(LevelMeter::Orientation orientation, int numElements)
    {
        return getTrackInfoArray(orientation, LevelMeter::Orientation::Horizontal, numElements);
    }

    juce::Array<juce::Grid::TrackInfo> getTemplateRowsForLayout(LevelMeter::Orientation orientation, int numElements)
    {
        return getTrackInfoArray(orientation, LevelMeter::Orientation::Vertical, numElements);
    }

    void addLabelsElementToTemplateAreas(juce::StringArray& templateAreas, LevelMeterComponent::LabelsPosition position,
                                         LevelMeter::Orientation orientation, const juce::String& labelsAreaName)
    {
        if (position == LevelMeterComponent::LabelsPosition::Left
            || position == LevelMeterComponent::LabelsPosition::Right)
        {
            for (auto i = 0; i < templateAreas.size(); i++)
            {
                if (position == LevelMeterComponent::LabelsPosition::Left)
                    templateAreas.set(i, labelsAreaName + " " + templateAreas[i]);
                else
                    templateAreas.set(i, templateAreas[i] + " " + labelsAreaName);
            }
        }
        else if (position == LevelMeterComponent::LabelsPosition::Above
                 || position == LevelMeterComponent::LabelsPosition::Below)
        {
            const juce::String existingRow{ templateAreas.size() > 0 ? templateAreas[0] : "" };
            const auto existingTokens = juce::StringArray::fromTokens(existingRow, true);

            juce::String newRow;

            for (auto i = 0; i < existingTokens.size(); i++)
            {
                newRow += labelsAreaName;

                if (i != existingTokens.size() - 1)
                    newRow += " ";
            }

            if (position == LevelMeterComponent::LabelsPosition::Above)
                templateAreas.insert(0, newRow);
            else
                templateAreas.add(newRow);
        }
        else if (position == LevelMeterComponent::LabelsPosition::Centred)
        {
            if (orientation == LevelMeter::Orientation::Vertical)
            {
                juce::String row{ templateAreas[0] };
                auto tokens = juce::StringArray::fromTokens(row, true);
                const auto midIndex = tokens.size() / 2;

                tokens.insert(midIndex, labelsAreaName);

                row = tokens.joinIntoString(" ");
                templateAreas.set(0, row);
            }
            else if (orientation == LevelMeter::Orientation::Horizontal)
            {
                const auto midIndex = templateAreas.size() / 2;
                templateAreas.insert(midIndex, labelsAreaName);
            }
            else
            {
                // Unhandled orientation.
                jassertfalse;
            }
        }
        else
        {
            // Unhandled labels position.
            jassertfalse;
        }
    }

    void addLabelsElementToTemplateColumnsOrRows(juce::Array<juce::Grid::TrackInfo>& templateColumns,
                                                 juce::Array<juce::Grid::TrackInfo>& templateRows,
                                                 LevelMeterComponent::LabelsPosition position,
                                                 LevelMeter::Orientation orientation,
                                                 int labelsWidthIfColumn, int labelsHeightIfRow)
    {
        using Px = juce::Grid::Px;

        if (position == LevelMeterComponent::LabelsPosition::Left)
            templateColumns.insert(0, Px{ labelsWidthIfColumn });
        else if (position == LevelMeterComponent::LabelsPosition::Right)
            templateColumns.add(Px{ labelsWidthIfColumn });
        else if (position == LevelMeterComponent::LabelsPosition::Above)
            templateRows.insert(0, Px{ labelsHeightIfRow });
        else if (position == LevelMeterComponent::LabelsPosition::Below)
            templateRows.add(Px{ labelsHeightIfRow });
        else if (position == LevelMeterComponent::LabelsPosition::Centred)
        {
            if (orientation == LevelMeter::Orientation::Vertical)
            {
                const auto midIndex = templateColumns.size() / 2;
                templateColumns.insert(midIndex, Px{ labelsWidthIfColumn });
            }
            else if (orientation == LevelMeter::Orientation::Horizontal)
            {
                const auto midIndex = templateRows.size() / 2;
                templateRows.insert(midIndex, Px{ labelsHeightIfRow });
            }
            else
            {
                // Unhandled orientation.
                jassertfalse;
            }
        }
        else
        {
            // Unhandled position.
            jassertfalse;
        }
    }

    void LevelMeterComponent::resized()
    {
        if (!isVisible() || getWidth() == 0 || getHeight() == 0)
            return;

        auto bounds = getLocalBounds();

        juce::Grid grid;

        static const juce::String meterAreaName{ "meter" };
        grid.templateAreas = getTemplateAreasForLayout(orientation, labelsPosition, meters.size(),
                                                       meterAreaName, "");
        grid.templateColumns = getTemplateColumnsForLayout(orientation, meters.size());
        grid.templateRows = getTemplateRowsForLayout(orientation, meters.size());

        static const juce::String labelsAreaName{ "labels" };
        addLabelsElementToTemplateAreas(grid.templateAreas, labelsPosition, orientation, labelsAreaName);
        addLabelsElementToTemplateColumnsOrRows(grid.templateColumns, grid.templateRows, labelsPosition, orientation,
                                                labels.getMinimumRequiredWidthWhenVertical(),
                                                labels.getMinimumRequiredHeightWhenHorizontal());

        grid.items.add(juce::GridItem{ labels }.withArea(labelsAreaName));

        for (auto i = 0; i < meters.size(); i++)
            grid.items.add(juce::GridItem{ meters[i] }.withArea(meterAreaName + juce::String{ i }));

        grid.setGap(juce::Grid::Px{ lookAndFeel->getLevelMetersGap(*this) });
        grid.performLayout(bounds);
    }

    void LevelMeterComponent::propertyChanged(const juce::Identifier& name, const juce::var& newValue)
    {
        if (name == PropertyIDs::labelsPositionId)
            setLabelsPositionInternal(juce::VariantConverter<LabelsPosition>::fromVar(newValue));
        else if (name == PropertyIDs::labelsVisibleId)
            labels.setVisible(static_cast<bool>(newValue));
        else if (name == PropertyIDs::highlightedLevelsId)
            labels.setHighlightedLevels(juce::VariantConverter<std::vector<float>>::fromVar(newValue));
        else if (name == PropertyIDs::orientationId)
            setOrientationInternal(juce::VariantConverter<LevelMeter::Orientation>::fromVar(newValue));
    }

    void LevelMeterComponent::colourChanged()
    {
        if (isColourSpecified(LookAndFeel::levelMeterSafeColourId))
        {
            for (auto& meter : meters)
            {
                meter->setColour(LookAndFeel::levelMeterSafeColourId,
                                 findColour(LookAndFeel::levelMeterSafeColourId));
            }
        }
        if (isColourSpecified(LookAndFeel::levelMeterWarningColourId))
        {
            for (auto& meter : meters)
            {
                meter->setColour(LookAndFeel::levelMeterWarningColourId,
                                 findColour(LookAndFeel::levelMeterWarningColourId));
            }
        }
        if (isColourSpecified(LookAndFeel::levelMeterDangerColourId))
        {
            for (auto& meter : meters)
            {
                meter->setColour(LookAndFeel::levelMeterDangerColourId,
                                 findColour(LookAndFeel::levelMeterDangerColourId));
            }
        }
    }

    //==================================================================================================================
    void LevelMeterComponent::initialiseState()
    {
        setProperty(PropertyIDs::labelsPositionId,
                    juce::VariantConverter<LabelsPosition>::toVar(LabelsPosition::Left));

        setProperty(PropertyIDs::labelsVisibleId, true);

        setProperty(PropertyIDs::highlightedLevelsId,
                    juce::VariantConverter<std::vector<float>>::toVar({ 0.f, -6.f, -12.f, -24.f, -48.f, -9999.f }));

        setProperty(PropertyIDs::orientationId,
                    juce::VariantConverter<LevelMeter::Orientation>::toVar(LevelMeter::Orientation::Vertical));
    }

    //==================================================================================================================
    void LevelMeterComponent::setLabelsPositionInternal(LabelsPosition newLabelsPosition)
    {
        if (newLabelsPosition == LabelsPosition::Left)
            labels.setJustification(juce::Justification::right);
        else if (newLabelsPosition == LabelsPosition::Right)
            labels.setJustification(juce::Justification::left);
        else
            labels.setJustification(juce::Justification::centred);

        labelsPosition = newLabelsPosition;
        resized();
    }

    void LevelMeterComponent::setOrientationInternal(LevelMeter::Orientation newOrientation)
    {
        if (newOrientation == LevelMeter::Orientation::Vertical)
        {
            if (labelsPosition == LabelsPosition::Above)
                setLabelsPosition(LabelsPosition::Left);
            else if (labelsPosition == LabelsPosition::Below)
                setLabelsPosition(LabelsPosition::Right);
        }
        else if (newOrientation == LevelMeter::Orientation::Horizontal)
        {
            if (labelsPosition == LabelsPosition::Left)
                setLabelsPosition(LabelsPosition::Above);
            else if (labelsPosition == LabelsPosition::Right)
                setLabelsPosition(LabelsPosition::Below);
        }
        else
        {
            // Unhandled orientation.
            jassertfalse;
        }

        labels.setOrientation(newOrientation);

        for (auto& meter : meters)
            meter->setOrientation(newOrientation);

        orientation = newOrientation;
        resized();
    }
}   // namespace jump
