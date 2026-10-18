#pragma once
#include <JuceHeader.h>
namespace jayway {
class PresetManager {
public:
    static juce::File defaultPresetFolder() {
        return juce::File::getSpecialLocation(juce::File::userDocumentsDirectory).getChildFile("JAYWAY Audio").getChildFile("Presets");
    }
    static bool save(const juce::AudioProcessorValueTreeState& state, const juce::File& file) {
        auto xml = state.copyState().createXml();
        return xml != nullptr && xml->writeTo(file);
    }
    static bool load(juce::AudioProcessorValueTreeState& state, const juce::File& file) {
        auto xml = juce::XmlDocument::parse(file);
        if (xml == nullptr) return false;
        auto tree = juce::ValueTree::fromXml(*xml);
        if (!tree.isValid()) return false;
        state.replaceState(tree);
        return true;
    }
};
}
