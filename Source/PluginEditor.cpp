#include "PluginEditor.h"

JayWayEditor::JayWayEditor(JayWayProcessor& p)
    : AudioProcessorEditor(&p), processor(p), eqDisplay(p.eq) {
    setLookAndFeel(&lookAndFeel);
    setResizable(true, true);
    setResizeLimits(1000, 700, 2200, 1400);
    setSize(1500, 950);
    title.setText("JAYWAY SERIES", juce::dontSendNotification);
    title.setFont(juce::Font(juce::FontOptions(28.0f, juce::Font::bold)));
    title.setColour(juce::Label::textColourId, juce::Colour(0xffffcc00));
    addAndMakeVisible(title);
    subtitle.setText("CHANNEL STRIP  |  JAYWAY AUDIO", juce::dontSendNotification);
    subtitle.setColour(juce::Label::textColourId, juce::Colour(0xffb8c8dc));
    addAndMakeVisible(subtitle);
    addAndMakeVisible(eqDisplay);
    outputLabel.setText("OUTPUT", juce::dontSendNotification);
    outputLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    addAndMakeVisible(outputLabel);
    outputSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    outputSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 75, 20);
    outputSlider.setRange(-24.0, 12.0, 0.1);
    outputSlider.setValue(0.0);
    outputSlider.setTextValueSuffix(" dB");
    outputSlider.onValueChange = [this] { if (auto* v=processor.parameters.getParameter("output")) v->setValueNotifyingHost(v->convertTo0to1((float)outputSlider.getValue())); };
    addAndMakeVisible(outputSlider);

    const juce::String names[8] = {"Pre-Mic", "FabEQ", "Expander", "3-Band Comp", "Opto Comp", "FET Comp", "SAT / DIST", "Smoother"};
    for (int i=0;i<8;++i) {
        moduleLabels[i].setText(names[i], juce::dontSendNotification);
        moduleLabels[i].setColour(juce::Label::textColourId, juce::Colours::white);
        addAndMakeVisible(moduleLabels[i]);
        blendSliders[i].setSliderStyle(juce::Slider::LinearHorizontal);
        blendSliders[i].setTextBoxStyle(juce::Slider::TextBoxRight, false, 48, 20);
        blendSliders[i].setRange(0.0, 100.0, 1.0);
        blendSliders[i].setValue(100.0);
        blendSliders[i].setTextValueSuffix("%");
        addAndMakeVisible(blendSliders[i]);
        bypassButtons[i].setButtonText("Bypass");
        addAndMakeVisible(bypassButtons[i]);
        blendSliders[i].onValueChange = [this, i] {
            const float value = (float)(blendSliders[i].getValue()/100.0);
            switch(i) {
                case 0: processor.preMic.setBlend(value); break; case 1: processor.eq.setBlend(value); break;
                case 2: processor.expander.setBlend(value); break; case 3: processor.multiband.setBlend(value); break;
                case 4: processor.opto.setBlend(value); break; case 5: processor.fet.setBlend(value); break;
                case 6: processor.saturation.setBlend(value); break; case 7: processor.smoother.setBlend(value); break;
            }
        };
        bypassButtons[i].onClick = [this, i] {
            const bool state = bypassButtons[i].getToggleState();
            switch(i) {
                case 0: processor.preMic.setBypassed(state); break; case 1: processor.eq.setBypassed(state); break;
                case 2: processor.expander.setBypassed(state); break; case 3: processor.multiband.setBypassed(state); break;
                case 4: processor.opto.setBypassed(state); break; case 5: processor.fet.setBypassed(state); break;
                case 6: processor.saturation.setBypassed(state); break; case 7: processor.smoother.setBypassed(state); break;
            }
        };
    }
    meterLabel.setColour(juce::Label::textColourId, juce::Colour(0xffffcc00));
    meterLabel.setText("VU / PEAK MONITOR", juce::dontSendNotification);
    addAndMakeVisible(meterLabel);
    startTimerHz(15);
}
JayWayEditor::~JayWayEditor() { setLookAndFeel(nullptr); stopTimer(); }
void JayWayEditor::paint(juce::Graphics& g) {
    g.fillAll(juce::Colour(0xff0a1931));
    g.setColour(juce::Colour(0xffffcc00)); g.fillRect(0,0,getWidth(),8);
    g.setColour(juce::Colour(0xff263e5b)); g.drawHorizontalLine(92, 20.0f, (float)getWidth()-20.0f);
}
void JayWayEditor::resized() {
    auto area=getLocalBounds().reduced(24); area.removeFromTop(8);
    auto header=area.removeFromTop(82);
    title.setBounds(header.removeFromTop(42));
    subtitle.setBounds(header);
    auto footer=area.removeFromBottom(100);
    outputLabel.setBounds(footer.removeFromLeft(90).removeFromTop(25));
    outputSlider.setBounds(footer.removeFromLeft(115));
    meterLabel.setBounds(footer.removeFromRight(240).reduced(8));
    auto moduleArea=area.removeFromBottom(300);
    const int rowH=moduleArea.getHeight()/8;
    for(int i=0;i<8;++i) {
        auto row=moduleArea.removeFromTop(rowH);
        moduleLabels[i].setBounds(row.removeFromLeft(140));
        bypassButtons[i].setBounds(row.removeFromRight(100).reduced(5,2));
        blendSliders[i].setBounds(row.reduced(6,3));
    }
    eqDisplay.setBounds(area.reduced(0,8));
}
void JayWayEditor::timerCallback() {
    const float peak=processor.visualiser.getLastPeak();
    meterLabel.setText("INPUT PEAK  " + juce::String(juce::Decibels::gainToDecibels(std::max(peak,1.0e-6f)),1) + " dBFS", juce::dontSendNotification);
}
juce::AudioProcessorEditor* JayWayProcessor::createEditor() { return new JayWayEditor(*this); }
