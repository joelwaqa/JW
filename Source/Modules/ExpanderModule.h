#pragma once
#include "ModuleBase.h"
namespace jayway {
class ExpanderModule : public ModuleBase {
public:
    const char* getName() const noexcept override { return "Expander"; }
    void process(juce::AudioBuffer<float>& b) override {
        if (bypassed) return;
        for(int c=0;c<b.getNumChannels();++c) { auto* p=b.getWritePointer(c); for(int i=0;i<b.getNumSamples();++i) {
            const float x=p[i]; const float a=std::abs(x); const float gain=a<0.025f ? a/0.025f : 1.0f;
            const float y=x*gain; p[i]=x+blend*(y-x);
        }}
    }
};
}
