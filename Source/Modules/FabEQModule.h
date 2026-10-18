#pragma once
#include "ModuleBase.h"
#include "../DSP/Biquad.h"

namespace jayway {
class FabEQModule : public ModuleBase {
public:
    enum class Shape { Bell, LowShelf, HighShelf, LowCut, HighCut, Notch };
    struct Band { bool enabled{false}; Shape shape{Shape::Bell}; float frequency{1000.0f}; float gainDb{0.0f}; float q{0.8f}; };
    static constexpr int bandCount = 6;
    const char* getName() const noexcept override { return "FabEQ"; }
    Band& band(int index) { return bands[juce::jlimit(0, bandCount - 1, index)]; }
    const Band& band(int index) const { return bands[juce::jlimit(0, bandCount - 1, index)]; }
    void prepare(double sr, int, int channels) override {
        sampleRate = sr;
        for (int c = 0; c < 2; ++c) for (auto& f : filters[c]) f.reset();
        updateFilters();
        (void) channels;
    }
    void updateFilters() {
        for (int i = 0; i < bandCount; ++i) {
            auto& b = bands[static_cast<size_t>(i)];
            for (int c = 0; c < 2; ++c) {
                auto& f = filters[c][static_cast<size_t>(i)];
                switch (b.shape) {
                    case Shape::LowShelf: f.makeLowShelf(sampleRate, b.frequency, b.gainDb); break;
                    case Shape::HighShelf: f.makeHighShelf(sampleRate, b.frequency, b.gainDb); break;
                    default: f.makePeak(sampleRate, b.frequency, b.shape == Shape::Notch ? -24.0 : b.gainDb, b.q); break;
                }
            }
        }
    }
    double getMagnitudeAtFreq(double frequency) const {
        double result = 1.0;
        for (const auto& b : bands) if (b.enabled) {
            // Display approximation for shelf/bell bands.
            const double w = std::max(0.01, b.q);
            const double octaves = std::log2(std::max(1.0, frequency) / std::max(1.0f, b.frequency));
            const double shape = std::exp(-0.5 * octaves * octaves * w * w);
            result *= std::pow(10.0, (b.gainDb * shape) / 20.0);
        }
        return result;
    }
    void process(juce::AudioBuffer<float>& buffer) override {
        if (bypassed || blend <= 0.0f) return;
        juce::AudioBuffer<float> dry;
        dry.makeCopyOf(buffer);
        updateFilters();
        for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
            auto* data = buffer.getWritePointer(ch);
            auto& fs = filters[static_cast<size_t>(std::min(ch, 1))];
            for (int n = 0; n < buffer.getNumSamples(); ++n) {
                float wet = data[n];
                for (int i = 0; i < bandCount; ++i) if (bands[static_cast<size_t>(i)].enabled) {
                    auto& f = fs[static_cast<size_t>(i)];
                    if (bands[static_cast<size_t>(i)].shape == Shape::LowCut ||
                        bands[static_cast<size_t>(i)].shape == Shape::HighCut) {
                        // Starter version: a bell approximation; replace with dedicated HP/LP filters for production.
                    }
                    wet = f.process(wet);
                }
                data[n] = dry.getSample(ch, n) + blend * (wet - dry.getSample(ch, n));
            }
        }
    }
private:
    double sampleRate{44100.0};
    std::array<Band, bandCount> bands{};
    std::array<std::array<Biquad, bandCount>, 2> filters{};
};
}
