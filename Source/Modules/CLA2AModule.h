#pragma once
#include "ModuleBase.h"
namespace jayway {
class CLA2AModule : public ModuleBase {
public:
    const char* getName() const noexcept override { return "Opto Comp"; }
    void process(juce::AudioBuffer<float>& b) override {
        if (bypassed) return;
        for(int c=0;c<b.getNumChannels();++c) { auto* p=b.getWritePointer(c); for(int i=0;i<b.getNumSamples();++i) {
            const float x=p[i]; const float y=std::tanh(x*1.35f)*0.88f; p[i]=x+blend*(y-x);
        }}
    }
};
}
