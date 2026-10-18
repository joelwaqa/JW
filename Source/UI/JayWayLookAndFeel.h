#pragma once
#include <JuceHeader.h>
namespace jayway {
class JayWayLookAndFeel : public juce::LookAndFeel_V4 {
public:
    JayWayLookAndFeel() {
        setColour(juce::ResizableWindow::backgroundColourId, juce::Colour(0xff0a1931));
        setColour(juce::Slider::thumbColourId, juce::Colour(0xffffcc00));
        setColour(juce::Slider::trackColourId, juce::Colour(0xff426080));
        setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
        setColour(juce::Slider::textBoxBackgroundColourId, juce::Colour(0xff132641));
        setColour(juce::ComboBox::backgroundColourId, juce::Colour(0xff132641));
        setColour(juce::ComboBox::textColourId, juce::Colours::white);
    }
    void drawButtonBackground(juce::Graphics& g, juce::Button& b, const juce::Colour& bg, bool hover, bool down) override {
        auto r=b.getLocalBounds().toFloat();
        g.setColour(down ? juce::Colour(0xff354b65) : (hover ? juce::Colour(0xff1b3555) : bg));
        g.fillRoundedRectangle(r, 4.0f);
        g.setColour(juce::Colour(0xffffcc00)); g.drawRoundedRectangle(r,4.0f,1.0f);
    }
};
}
