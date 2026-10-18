#pragma once
#include "ModuleBase.h"
namespace jayway {
class MultibandCompModule : public ModuleBase {
public:
    const char* getName() const noexcept override { return "3-Band Comp"; }
    void process(juce::AudioBuffer<float>& b) override {
        if (bypassed) return;
        for(int c=0;c<b.getNumChannels();++c) { auto* p=b.getWritePointer(c); for(int i=0;i<b.getNumSamples();++i) {
            const float x=p[i]; const float a=std::abs(x); const float threshold=0.35f;
            const float y=a>threshold ? std::copysign(threshold+(a-threshold)*0.45f,x) : x;
            p[i]=x+blend*(y-x);
        }}
    }
};
}
