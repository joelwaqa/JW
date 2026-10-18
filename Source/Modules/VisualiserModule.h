#pragma once
#include "ModuleBase.h"
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
namespace jayway {
class VisualiserModule : public ModuleBase {
public:
    const char* getName() const noexcept override { return "Visualiser"; }
    void process(juce::AudioBuffer<float>& b) override {
        if (bypassed || b.getNumSamples()==0) return;
        const auto* p=b.getReadPointer(0); float peak=0.0f;
        for(int j=0;j<b.getNumSamples();++j) peak=std::max(peak,std::abs(p[j]));
        lastPeak=peak;
    }
    float getLastPeak() const noexcept { return lastPeak; }
private: float lastPeak{0.0f};
};
}
