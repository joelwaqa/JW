# JAYWAY Series Channel Strip

A JUCE 8.0.6 / CMake VST3 project for the JAYWAY Audio channel-strip concept.

## Build on GitHub Actions

1. Create a new GitHub repository.
2. Upload the **contents** of this ZIP, including the hidden `.github` folder.
3. Push to `main` or `master`, or open the **Actions** tab and run **Build JAYWAY VST3**.
4. Download the generated artifact named `JAYWAY-Series-Channel-Strip-VST3-Windows`.

The first configure downloads JUCE 8.0.6 from the official JUCE Git repository, so the runner needs network access.

## Current implementation status

This repository is a starter implementation designed to provide a complete source layout and a build workflow. It includes a working JUCE plug-in processor/editor, a six-band EQ display with draggable points, basic DSP modules and module blend/bypass controls. Some named modules are deliberately simplified starter DSP and are **not** exact emulations of commercial products such as CLA-2A/CLA-76, nor is the spectrum display a calibrated mastering analyser. Review and test the result in your DAW before production use.

## Requirements

- CMake 3.22+
- A C++17 compiler
- JUCE 8.0.6 (fetched automatically by CMake)
- Windows Visual Studio build tools for the GitHub Actions target

## Plug-in identity

- Product: JAYWAY Series Channel Strip
- Company: JAYWAY Audio
- Manufacturer code: `Jayw`
- Plug-in code: `Jay4`
- Bundle ID: `com.jaywayaudio.fabeq.v4`
- Format: VST3
