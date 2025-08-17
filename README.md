# Basic OSC Tutorial

A simple open-source VST synth project made with JUCE. This repository serves as an introductory tutorial for building audio plugins from scratch and understanding the basics of synthesizer (synth) development.

## Overview

This project is inspired by the idea of learning by doing—building a functional audio synthesizer plugin (VST) in a single week. It is perfect for beginners looking to get started with:

- Audio programming using C++ and JUCE
- Designing synthesizer signal flow
- Basic sound synthesis concepts such as oscillators, envelopes, and filters
- Connecting digital signal processing with real plugin user interfaces

You’ll find all the source code for a basic oscillator-based synth (OSC = oscillator), as well as the project files and instructions to get running quickly.

## Features

- **Basic oscillator(s):** Fundamental sound source for synthesis (sine, square, saw, etc.)
- **Simple envelope generator:** Shape the amplitude of each note
- **Minimal UI:** Just enough to tweak and hear your results
- **Well-commented code:** Learn as you read and experiment


## Getting Started

### Prerequisites

- You should have a working C++ environment.
- Install [JUCE Framework](https://juce.com/).
- Compatible with major DAWs that support VST3 plugins.


### Building the Plugin

1. Clone this repository:

```sh
git clone https://github.com/kashyap-harshit/basic-osc-tutorial.git
```

2. Open the `basicOsc.jucer` file in the Projucer.
3. Set up your desired exporter (Visual Studio, Xcode, etc.).
4. Build the project using your IDE.
5. Copy or link the built VST3 to your DAW’s plugin folder, then scan for new plugins.

## How It Works

This project follows the typical architecture of a subtractive synth in software:

- **Oscillator Section:** Generates a periodic waveform (tone) at a controllable frequency.
- **Envelope Section:** Applies an ADSR envelope to modulate amplitude.
- **Processor:** Handles synthesizer voice allocation and output mixing.

All elements are simplified for clarity and ease of experimentation, making this a great learning project.

## Learning Resources

For more theory and inspiration, read:

- \#Synth-Quest on Hashnode: Insights and motivations behind building a synth plugin in a short time.
- **Create a Synth VST in One Week:** A blog documenting the week-long learning and building journey.
