#include "PluginProcessor.h"
#include "PluginEditor.h"

JayWayProcessor::JayWayProcessor()
    : AudioProcessor(BusesProperties().withInput("Input", juce::AudioChannelSet::stereo(), true)
                                      .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      parameters(*this, nullptr, "JAYWAY_STATE", createParameterLayout()) {}

juce::AudioProcessorValueTreeState::ParameterLayout JayWayProcessor::createParameterLayout() {
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    layout.add(std::make_unique<juce::AudioParameterFloat>("output", "Output", juce::NormalisableRange<float>(-24.0f, 12.0f, 0.1f), 0.0f));
    return layout;
}

void JayWayProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    const int channels = getTotalNumOutputChannels();
    eq.prepare(sampleRate, samplesPerBlock, channels);
    preMic.prepare(sampleRate, samplesPerBlock, channels);
    multiband.prepare(sampleRate, samplesPerBlock, channels);
    expander.prepare(sampleRate, samplesPerBlock, channels);
    opto.prepare(sampleRate, samplesPerBlock, channels);
    fet.prepare(sampleRate, samplesPerBlock, channels);
    saturation.prepare(sampleRate, samplesPerBlock, channels);
    vu.prepare(sampleRate, samplesPerBlock, channels);
    visualiser.prepare(sampleRate, samplesPerBlock, channels);
    smoother.prepare(sampleRate, samplesPerBlock, channels);
}
void JayWayProcessor::releaseResources() {}

bool JayWayProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const {
    const auto out = layouts.getMainOutputChannelSet();
    const auto in = layouts.getMainInputChannelSet();
    return (out == juce::AudioChannelSet::mono() || out == juce::AudioChannelSet::stereo()) && in == out;
}

void JayWayProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi) {
    juce::ignoreUnused(midi);
    juce::ScopedNoDenormals noDenormals;
    for (int ch = getTotalNumInputChannels(); ch < getTotalNumOutputChannels(); ++ch)
        buffer.clear(ch, 0, buffer.getNumSamples());

    preMic.process(buffer);
    eq.process(buffer);
    expander.process(buffer);
    multiband.process(buffer);
    opto.process(buffer);
    fet.process(buffer);
    saturation.process(buffer);
    smoother.process(buffer);
    vu.process(buffer);
    visualiser.process(buffer);

    const float outputDb = parameters.getRawParameterValue("output")->load();
    buffer.applyGain(juce::Decibels::decibelsToGain(outputDb));
}

void JayWayProcessor::getStateInformation(juce::MemoryBlock& destData) {
    auto state = parameters.copyState();
    auto xml = state.createXml();
    if (xml != nullptr) copyXmlToBinary(*xml, destData);
}
void JayWayProcessor::setStateInformation(const void* data, int sizeInBytes) {
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
    if (xml != nullptr && xml->hasTagName(parameters.state.getType()))
        parameters.replaceState(juce::ValueTree::fromXml(*xml));
}
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new JayWayProcessor(); }
