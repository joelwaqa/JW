#pragma once
#include <JuceHeader.h>
#include "Modules/FabEQModule.h"
#include "Modules/PreMicHandler.h"
#include "Modules/MultibandCompModule.h"
#include "Modules/ExpanderModule.h"
#include "Modules/CLA2AModule.h"
#include "Modules/CLA76Module.h"
#include "Modules/SatDistModule.h"
#include "Modules/VUModule.h"
#include "Modules/VisualiserModule.h"
#include "Modules/SmootherModule.h"

class JayWayProcessor : public juce::AudioProcessor {
public:
    JayWayProcessor();
    ~JayWayProcessor() override = default;
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }
    const juce::String getName() const override { return JucePlugin_Name; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return {}; }
    void changeProgramName(int, const juce::String&) override {}
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;
    juce::AudioProcessorValueTreeState parameters;
    jayway::FabEQModule eq;
    jayway::PreMicHandler preMic;
    jayway::MultibandCompModule multiband;
    jayway::ExpanderModule expander;
    jayway::CLA2AModule opto;
    jayway::CLA76Module fet;
    jayway::SatDistModule saturation;
    jayway::VUModule vu;
    jayway::VisualiserModule visualiser;
    jayway::SmootherModule smoother;
private:
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(JayWayProcessor)
};
