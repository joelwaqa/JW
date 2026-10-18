#pragma once
#include <cmath>
#include <complex>
#include <algorithm>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace jayway {
struct Biquad {
    double b0{1.0}, b1{0.0}, b2{0.0}, a1{0.0}, a2{0.0};
    double z1{0.0}, z2{0.0};

    void reset() noexcept { z1 = z2 = 0.0; }

    float process(float input) noexcept {
        const double x = static_cast<double>(input);
        const double y = b0 * x + z1;
        z1 = b1 * x - a1 * y + z2;
        z2 = b2 * x - a2 * y;
        return static_cast<float>(y);
    }

    void makePeak(double sampleRate, double frequency, double gainDb, double q) {
        const double f = std::clamp(frequency, 10.0, sampleRate * 0.49);
        const double safeQ = std::max(0.1, q);
        const double A = std::pow(10.0, gainDb / 40.0);
        const double w0 = 2.0 * M_PI * f / sampleRate;
        const double cosw0 = std::cos(w0);
        const double alpha = std::sin(w0) / (2.0 * safeQ);
        const double a0 = 1.0 + alpha / A;
        b0 = (1.0 + alpha * A) / a0;
        b1 = (-2.0 * cosw0) / a0;
        b2 = (1.0 - alpha * A) / a0;
        a1 = (-2.0 * cosw0) / a0;
        a2 = (1.0 - alpha / A) / a0;
    }

    void makeLowShelf(double sampleRate, double frequency, double gainDb, double slope = 1.0) {
        const double A = std::pow(10.0, gainDb / 40.0);
        const double w0 = 2.0 * M_PI * std::clamp(frequency, 10.0, sampleRate * 0.49) / sampleRate;
        const double cosw0 = std::cos(w0);
        const double sinw0 = std::sin(w0);
        const double alpha = sinw0 * 0.5 * std::sqrt((A + 1.0 / A) * (1.0 / std::max(0.1, slope) - 1.0) + 2.0);
        const double twoRootAAlpha = 2.0 * std::sqrt(A) * alpha;
        const double a0 = (A + 1.0) + (A - 1.0) * cosw0 + twoRootAAlpha;
        b0 = A * ((A + 1.0) - (A - 1.0) * cosw0 + twoRootAAlpha) / a0;
        b1 = 2.0 * A * ((A - 1.0) - (A + 1.0) * cosw0) / a0;
        b2 = A * ((A + 1.0) - (A - 1.0) * cosw0 - twoRootAAlpha) / a0;
        const double a1n = -2.0 * ((A - 1.0) + (A + 1.0) * cosw0);
        const double a2n = (A + 1.0) + (A - 1.0) * cosw0 - twoRootAAlpha;
        a1 = a1n / a0; a2 = a2n / a0;
    }

    void makeHighShelf(double sampleRate, double frequency, double gainDb, double slope = 1.0) {
        const double A = std::pow(10.0, gainDb / 40.0);
        const double w0 = 2.0 * M_PI * std::clamp(frequency, 10.0, sampleRate * 0.49) / sampleRate;
        const double cosw0 = std::cos(w0);
        const double sinw0 = std::sin(w0);
        const double alpha = sinw0 * 0.5 * std::sqrt((A + 1.0 / A) * (1.0 / std::max(0.1, slope) - 1.0) + 2.0);
        const double twoRootAAlpha = 2.0 * std::sqrt(A) * alpha;
        const double a0 = (A + 1.0) - (A - 1.0) * cosw0 + twoRootAAlpha;
        b0 = A * ((A + 1.0) + (A - 1.0) * cosw0 + twoRootAAlpha) / a0;
        b1 = -2.0 * A * ((A - 1.0) + (A + 1.0) * cosw0) / a0;
        b2 = A * ((A + 1.0) + (A - 1.0) * cosw0 - twoRootAAlpha) / a0;
        a1 = 2.0 * ((A - 1.0) - (A + 1.0) * cosw0) / a0;
        a2 = ((A + 1.0) - (A - 1.0) * cosw0 - twoRootAAlpha) / a0;
    }

    double magnitudeResponse(double frequency, double sampleRate) const {
        const double w = 2.0 * M_PI * frequency / sampleRate;
        const std::complex<double> z1 = std::polar(1.0, -w);
        const std::complex<double> z2 = z1 * z1;
        const std::complex<double> numerator =
            std::complex<double>(b0, 0.0) + std::complex<double>(b1, 0.0) * z1 +
            std::complex<double>(b2, 0.0) * z2;
        const std::complex<double> denominator =
            std::complex<double>(1.0, 0.0) + std::complex<double>(a1, 0.0) * z1 +
            std::complex<double>(a2, 0.0) * z2;
        const double den = std::abs(denominator);
        return den > 1.0e-12 ? std::abs(numerator) / den : 1.0;
    }
};
} // namespace jayway
