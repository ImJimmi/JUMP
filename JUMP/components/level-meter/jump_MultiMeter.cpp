#include "jump_MultiMeter.h"

//======================================================================================================================
namespace juce
{
    //==================================================================================================================
    template <>
    struct VariantConverter<jump::MultiMeter::LabelsPosition>
    {
        //==============================================================================================================
        static jump::MultiMeter::LabelsPosition fromVar(const juce::var& v)
        {
            return static_cast<jump::MultiMeter::LabelsPosition>(static_cast<int>(v));
        }

        static juce::var toVar(const jump::MultiMeter::LabelsPosition& position)
        {
            return { static_cast<int>(position) };
        }
    };
} // namespace juce

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    MultiMeter::MultiMeter(const std::vector<LevelMeterEngine*>& enginesToUse,
                           juce::Identifier type, StatefulObject* parentState)
        : StatefulObject{ type, parentState }
        , mainEngine{ *enginesToUse.front() }
        , labels{ *enginesToUse.front() }
    {
        lookAndFeel.attachTo(this);

        initialiseState();

        for (auto& engine : enginesToUse)
        {
            auto meter = std::make_unique<LevelMeter>(*engine);
            addAndMakeVisible(*meter);
            meters.add(std::move(meter));
        }

        addAndMakeVisible(labels);
    }

    //==================================================================================================================
    void MultiMeter::setLabelsPosition(LabelsPosition newPosition)
    {
        setProperty(PropertyIDs::labelsPositionId, var_cast<LabelsPosition>(newPosition));
    }

    void MultiMeter::setShowLabels(bool shouldShowLabels)
    {
        setProperty(PropertyIDs::labelsVisibleId, shouldShowLabels);
    }

    void MultiMeter::setDecibelLevelsForLabels(const std::vector<float>& newLabelLevels)
    {
        setProperty(PropertyIDs::highlightedLevelsId, var_cast<std::vector<float>>(newLabelLevels));
    }

    void MultiMeter::setOrientation(Orientation newOrientation)
    {
        setProperty(PropertyIDs::orientationId, var_cast<Orientation>(newOrientation));
    }

    //==================================================================================================================
    juce::StringArray getTemplateAreasForLayout(Orientation orientation,
                                                MultiMeter::LabelsPosition /*position*/, int numMeters,
                                                const juce::String& meterAreaName, const juce::String& /*labelsAreaName*/)
    {
        juce::StringArray result;

        if (orientation == Orientation::vertical)
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
        else if (orientation == Orientation::horizontal)
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

    juce::Array<juce::Grid::TrackInfo> getTrackInfoArray(Orientation orientation,
                                                         Orientation orientationForSingleElement,
                                                         int numElementsOtherwise)
    {
        using Fr = juce::Grid::Fr;

        if (orientation == orientationForSingleElement)
            return { { Fr{ 1 } } };

        juce::Array<juce::Grid::TrackInfo> result;

        for (auto i = 0; i < numElementsOtherwise; i++)
            result.add(Fr{ 1 });

        return result;
    }

    juce::Array<juce::Grid::TrackInfo> getTemplateColumnsForLayout(Orientation orientation, int numElements)
    {
        return getTrackInfoArray(orientation, Orientation::horizontal, numElements);
    }

    juce::Array<juce::Grid::TrackInfo> getTemplateRowsForLayout(Orientation orientation, int numElements)
    {
        return getTrackInfoArray(orientation, Orientation::vertical, numElements);
    }

    void addLabelsElementToTemplateAreas(juce::StringArray& templateAreas, MultiMeter::LabelsPosition position,
                                         Orientation orientation, const juce::String& labelsAreaName)
    {
        if (position == MultiMeter::LabelsPosition::left
            || position == MultiMeter::LabelsPosition::right)
        {
            for (auto i = 0; i < templateAreas.size(); i++)
            {
                if (position == MultiMeter::LabelsPosition::left)
                    templateAreas.set(i, labelsAreaName + " " + templateAreas[i]);
                else
                    templateAreas.set(i, templateAreas[i] + " " + labelsAreaName);
            }
        }
        else if (position == MultiMeter::LabelsPosition::above
                 || position == MultiMeter::LabelsPosition::below)
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

            if (position == MultiMeter::LabelsPosition::above)
                templateAreas.insert(0, newRow);
            else
                templateAreas.add(newRow);
        }
        else if (position == MultiMeter::LabelsPosition::centred)
        {
            if (orientation == Orientation::vertical)
            {
                juce::String row{ templateAreas[0] };
                auto tokens = juce::StringArray::fromTokens(row, true);
                const auto midIndex = tokens.size() / 2;

                tokens.insert(midIndex, labelsAreaName);

                row = tokens.joinIntoString(" ");
                templateAreas.set(0, row);
            }
            else if (orientation == Orientation::horizontal)
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
                                                 MultiMeter::LabelsPosition position,
                                                 Orientation orientation,
                                                 int labelsWidthIfColumn, int labelsHeightIfRow)
    {
        using Px = juce::Grid::Px;

        if (position == MultiMeter::LabelsPosition::left)
            templateColumns.insert(0, Px{ labelsWidthIfColumn });
        else if (position == MultiMeter::LabelsPosition::right)
            templateColumns.add(Px{ labelsWidthIfColumn });
        else if (position == MultiMeter::LabelsPosition::above)
            templateRows.insert(0, Px{ labelsHeightIfRow });
        else if (position == MultiMeter::LabelsPosition::below)
            templateRows.add(Px{ labelsHeightIfRow });
        else if (position == MultiMeter::LabelsPosition::centred)
        {
            if (orientation == Orientation::vertical)
            {
                const auto midIndex = templateColumns.size() / 2;
                templateColumns.insert(midIndex, Px{ labelsWidthIfColumn });
            }
            else if (orientation == Orientation::horizontal)
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

    void MultiMeter::resized()
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

        grid.setGap(juce::Grid::Px{ lookAndFeel->getGapBetweenMeters(*this) });
        grid.performLayout(bounds);
    }

    void MultiMeter::propertyChanged(const juce::Identifier& name, const juce::var& newValue)
    {
        if (name == PropertyIDs::labelsPositionId)
            setLabelsPositionInternal(var_cast<LabelsPosition>(newValue));
        else if (name == PropertyIDs::labelsVisibleId)
            labels.setVisible(static_cast<bool>(newValue));
        else if (name == PropertyIDs::highlightedLevelsId)
            labels.setHighlightedLevels(var_cast<std::vector<float>>(newValue));
        else if (name == PropertyIDs::orientationId)
            setOrientationInternal(var_cast<Orientation>(newValue));
    }

    void MultiMeter::colourChanged()
    {
        if (isColourSpecified(levelMeterSafeColourId))
        {
            for (auto& meter : meters)
                meter->setColour(levelMeterSafeColourId, findColour(levelMeterSafeColourId));
        }
        if (isColourSpecified(levelMeterWarningColourId))
        {
            for (auto& meter : meters)
                meter->setColour(levelMeterWarningColourId, findColour(levelMeterWarningColourId));
        }
        if (isColourSpecified(levelMeterDangerColourId))
        {
            for (auto& meter : meters)
                meter->setColour(levelMeterDangerColourId, findColour(levelMeterDangerColourId));
        }
    }

    //==================================================================================================================
    void MultiMeter::initialiseState()
    {
        setProperty(PropertyIDs::labelsPositionId, var_cast<LabelsPosition>(LabelsPosition::left));

        setProperty(PropertyIDs::labelsVisibleId, true);

        setProperty(PropertyIDs::highlightedLevelsId,
                    var_cast<std::vector<float>>({ 0.f, -6.f, -12.f, -24.f, -48.f, -9999.f }));

        setProperty(PropertyIDs::orientationId, var_cast<Orientation>(Orientation::vertical));
    }

    //==================================================================================================================
    void MultiMeter::setLabelsPositionInternal(LabelsPosition newLabelsPosition)
    {
        if (newLabelsPosition == LabelsPosition::left)
            labels.setJustification(juce::Justification::right);
        else if (newLabelsPosition == LabelsPosition::right)
            labels.setJustification(juce::Justification::left);
        else
            labels.setJustification(juce::Justification::centred);

        labelsPosition = newLabelsPosition;
        resized();
    }

    void MultiMeter::setOrientationInternal(Orientation newOrientation)
    {
        if (newOrientation == Orientation::vertical)
        {
            if (labelsPosition == LabelsPosition::above)
                setLabelsPosition(LabelsPosition::left);
            else if (labelsPosition == LabelsPosition::below)
                setLabelsPosition(LabelsPosition::right);
        }
        else if (newOrientation == Orientation::horizontal)
        {
            if (labelsPosition == LabelsPosition::left)
                setLabelsPosition(LabelsPosition::above);
            else if (labelsPosition == LabelsPosition::right)
                setLabelsPosition(LabelsPosition::below);
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
} // namespace jump
