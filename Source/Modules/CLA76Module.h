#pragma once
#include "ModuleBase.h"
namespace jayway {
class CLA76Module : public ModuleBase {
public:
    const char* getName() const noexcept override { return "FET Comp"; }
    void process(juce::AudioBuffer<float>& b) override {
        if (bypassed) return;
        for(int c=0;c<b.getNumChannels();++c) { auto* p=b.getWritePointer(c); for(int i=0;i<b.getNumSamples();++i) {
            const float x=p[i]; const float a=std::abs(x); const float y=a>0.22f ? std::copysign(0.22f+(a-0.22f)*0.25f,x) : x;
            p[i]=x+blend*(y-x);
        }}
    }
};
}
