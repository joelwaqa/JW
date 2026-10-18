#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Modules/FabEQDisplay.h"
#include "UI/JayWayLookAndFeel.h"

class JayWayEditor : public juce::AudioProcessorEditor, private juce::Timer {
public:
    explicit JayWayEditor(JayWayProcessor&);
    ~JayWayEditor() override;
    void paint(juce::Graphics&) override;
    void resized() override;
private:
    void timerCallback() override;
    void setupModuleRow(juce::Label&, juce::Slider&, juce::ToggleButton&, const juce::String&);
    JayWayProcessor& processor;
    jayway::JayWayLookAndFeel lookAndFeel;
    jayway::FabEQDisplay eqDisplay;
    juce::Label title, subtitle, outputLabel, meterLabel;
    juce::Slider outputSlider;
    juce::Label moduleLabels[8];
    juce::Slider blendSliders[8];
    juce::ToggleButton bypassButtons[8];
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(JayWayEditor)
};
