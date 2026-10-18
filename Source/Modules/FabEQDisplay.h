#pragma once
#include <JuceHeader.h>
#include "FabEQModule.h"

namespace jayway {
class FabEQDisplay : public juce::Component {
public:
    explicit FabEQDisplay(FabEQModule& eq) : module(eq) { setMouseCursor(juce::MouseCursor::CrosshairCursor); }
    void paint(juce::Graphics& g) override {
        auto r = getLocalBounds().toFloat();
        g.fillAll(juce::Colour(0xff071326));
        g.setColour(juce::Colour(0xff21334c));
        for (int i = 0; i <= 8; ++i) {
            const float y = r.getY() + r.getHeight() * i / 8.0f;
            g.drawHorizontalLine(juce::roundToInt(y), r.getX(), r.getRight());
        }
        for (int i = 0; i <= 10; ++i) {
            const float x = r.getX() + r.getWidth() * i / 10.0f;
            g.drawVerticalLine(juce::roundToInt(x), r.getY(), r.getBottom());
        }
        juce::Path curve;
        for (int x = 0; x < getWidth(); ++x) {
            const double f = freqAtX(static_cast<float>(x));
            const double mag = module.getMagnitudeAtFreq(f);
            const float db = static_cast<float>(juce::Decibels::gainToDecibels(mag));
            const float y = dbToY(db);
            if (x == 0) curve.startNewSubPath((float)x, y); else curve.lineTo((float)x, y);
        }
        g.setColour(juce::Colour(0xffffcc00));
        g.strokePath(curve, juce::PathStrokeType(2.2f));
        for (int i = 0; i < FabEQModule::bandCount; ++i) {
            const auto& b = module.band(i);
            if (!b.enabled) continue;
            const auto p = juce::Point<float>(xAtFreq(b.frequency), dbToY(b.gainDb));
            g.setColour(juce::Colour(0xffffcc00)); g.fillEllipse(p.x - 5.0f, p.y - 5.0f, 10.0f, 10.0f);
            g.setColour(juce::Colours::black); g.drawEllipse(p.x - 5.0f, p.y - 5.0f, 10.0f, 10.0f, 1.0f);
        }
        g.setColour(juce::Colour(0xffaab8ca));
        g.setFont(11.0f);
        g.drawText("20 Hz", 4, getHeight() - 18, 48, 14, juce::Justification::left);
        g.drawText("1 kHz", getWidth()/2 - 20, getHeight() - 18, 48, 14, juce::Justification::centred);
        g.drawText("20 kHz", getWidth() - 52, getHeight() - 18, 48, 14, juce::Justification::right);
    }
    void mouseDoubleClick(const juce::MouseEvent& e) override {
        int index = nearestBand(e.position);
        auto& b = module.band(index);
        b.enabled = true; b.shape = FabEQModule::Shape::Bell;
        b.frequency = static_cast<float>(freqAtX(e.position.x));
        b.gainDb = juce::jlimit(-18.0f, 18.0f, yToDb(e.position.y));
        b.q = 0.8f; module.updateFilters(); repaint();
    }
    void mouseDown(const juce::MouseEvent& e) override { selected = nearestEnabledBand(e.position); lastPos = e.position; }
    void mouseDrag(const juce::MouseEvent& e) override {
        if (selected < 0) return;
        auto& b = module.band(selected);
        if (e.mods.isShiftDown()) b.q = juce::jlimit(0.1f, 10.0f, b.q + (lastPos.y - e.position.y) * 0.02f);
        else { b.frequency = static_cast<float>(freqAtX(e.position.x)); b.gainDb = juce::jlimit(-24.0f, 24.0f, yToDb(e.position.y)); }
        lastPos = e.position; module.updateFilters(); repaint();
    }
private:
    double freqAtX(float x) const { const double t = juce::jlimit(0.0, 1.0, (double)x / (double)juce::jmax(1, getWidth())); return 20.0 * std::pow(1000.0, t); }
    float xAtFreq(float f) const { return (float)(std::log(std::max(20.0f, f) / 20.0) / std::log(1000.0) * getWidth()); }
    float dbToY(float db) const { return juce::jmap(juce::jlimit(-24.0f, 24.0f, db), 24.0f, -24.0f, 0.0f, (float)getHeight()); }
    float yToDb(float y) const { return juce::jmap(y, 0.0f, (float)getHeight(), 24.0f, -24.0f); }
    int nearestBand(juce::Point<float> p) const {
        int best = 0; float dist = 1.0e9f;
        for (int i=0;i<FabEQModule::bandCount;++i) { auto& b=module.band(i); float d=std::abs(xAtFreq(b.frequency)-p.x)+std::abs(dbToY(b.gainDb)-p.y); if (d<dist) {dist=d;best=i;} }
        return best;
    }
    int nearestEnabledBand(juce::Point<float> p) const {
        int best=-1; float dist=18.0f;
        for (int i=0;i<FabEQModule::bandCount;++i) { auto& b=module.band(i); if (!b.enabled) continue; float d=std::hypot(xAtFreq(b.frequency)-p.x,dbToY(b.gainDb)-p.y); if(d<dist){dist=d;best=i;} }
        return best;
    }
    FabEQModule& module;
    int selected{-1};
    juce::Point<float> lastPos{};
};
}
