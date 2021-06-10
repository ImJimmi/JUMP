#include "jump_SnapshotGenerator.h"

//======================================================================================================================
namespace jump
{
    //==================================================================================================================
    SnapshotGenerator::SnapshotGenerator(juce::Component& targetComponent, const juce::File& outputDirectory)
        : component{ targetComponent }
        , file{ outputDirectory.getChildFile(juce::Time::getCurrentTime().toISO8601(false) + ".png") }
    {
        component.addAndMakeVisible(this);
        setSize(1, 1);
    }

    //==================================================================================================================
    void SnapshotGenerator::paint(juce::Graphics&)
    {
        triggerAsyncUpdate();
        setVisible(false);
    }

    bool snapshotIsIdenticalToMostRecentSavedSnapshot(const juce::Image& snapshotImage, const juce::File& snapshotFile)
    {
        juce::StringArray snapshotNames;

        for (auto entry : juce::RangedDirectoryIterator{ snapshotFile.getParentDirectory(), false, "*.png" })
            snapshotNames.add(entry.getFile().getFileName());

        if (snapshotNames.isEmpty())
            return false;

        snapshotNames.sortNatural();

        const auto mostRecentSnapshotFile = snapshotFile.getSiblingFile(*(snapshotNames.end() - 1));
        jassert(mostRecentSnapshotFile.existsAsFile());

        const auto mostRecentSnapshotImage = juce::ImageCache::getFromFile(mostRecentSnapshotFile);

        if (mostRecentSnapshotImage.getBounds() != snapshotImage.getBounds())
            return false;

        for (auto y = 0; y < snapshotImage.getHeight(); y++)
        {
            auto rowsMatch = true;

            for (auto x = 0; x < snapshotImage.getWidth(); x++)
                rowsMatch = rowsMatch && (snapshotImage.getPixelAt(x, y) == mostRecentSnapshotImage.getPixelAt(x, y));

            if (!rowsMatch)
                return false;
        }

        return true;
    }

    void SnapshotGenerator::handleAsyncUpdate()
    {
        const auto snapshot = component.createComponentSnapshot(component.getLocalBounds());

        if (!snapshotIsIdenticalToMostRecentSavedSnapshot(snapshot, file))
        {
            auto outputStream = file.createOutputStream();
            juce::PNGImageFormat pngFormat;
            pngFormat.writeImageToStream(snapshot, *outputStream);

            DBG("[JUMP SnapshotGenerator] Snapshot saved to: " << file.getFullPathName());
        }
        else
        {
            DBG("[JUMP SnapshotGenerator] No visual changes detected.");
        }
    }
} // namespace jump
