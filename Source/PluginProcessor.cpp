/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BasicOscAudioProcessor::BasicOscAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), theValueTree(*this, nullptr, "Parameters", createParams())
#endif
{
    synth.addSound(new SynthSound());
    synth.addVoice(new SynthVoice());


}

BasicOscAudioProcessor::~BasicOscAudioProcessor()
{
}

//==============================================================================
const juce::String BasicOscAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BasicOscAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool BasicOscAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool BasicOscAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double BasicOscAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BasicOscAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int BasicOscAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BasicOscAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String BasicOscAudioProcessor::getProgramName (int index)
{
    return {};
}

void BasicOscAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void BasicOscAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate(sampleRate);
    for (int i = 0;i < synth.getNumVoices(); i++) {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i))) {
            voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels());
        }
    }

}

void BasicOscAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool BasicOscAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void BasicOscAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());
    for (int i = 0; i < synth.getNumVoices(); ++i) {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i))) {
            auto& waveformType = *theValueTree.getRawParameterValue("OSC");

            voice->changeOsc(waveformType);

            auto& attack = *theValueTree.getRawParameterValue("ATTACK");
            auto& decay = *theValueTree.getRawParameterValue("DECAY");
            auto& sustain = *theValueTree.getRawParameterValue("SUSTAIN");
            auto& release = *theValueTree.getRawParameterValue("RELEASE");
            voice->updateADSR(attack, decay, sustain, release);

        }
    }
    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());


}

//==============================================================================
bool BasicOscAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* BasicOscAudioProcessor::createEditor()
{
    return new BasicOscAudioProcessorEditor (*this);
}

//==============================================================================
void BasicOscAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void BasicOscAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BasicOscAudioProcessor();
}

bool SynthVoice::canPlaySound(juce::SynthesiserSound* sound)
{
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}

void SynthVoice::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{
    osc.reset();
    osc.setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber));
    adsr.noteOn();


}

void SynthVoice::stopNote(float velocity, bool allowTailOff) {
    adsr.noteOff();
}
void SynthVoice::controllerMoved(int controllerNumber, int newControllerValue) {
}


void SynthVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels) {
    adsr.setSampleRate(sampleRate);
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = outputChannels;
    osc.prepare(spec);

}

void SynthVoice::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) {
    juce::dsp::AudioBlock<float> audioBlock{ outputBuffer };
    osc.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));

    adsr.applyEnvelopeToBuffer(outputBuffer, startSample, numSamples);
}

void SynthVoice::updateADSR(float attack, float decay, float sustain, float release) {
    adsrParams.attack = attack;
    adsrParams.decay = decay;
    adsrParams.sustain = sustain;
    adsrParams.release = release;
    adsr.setParameters(adsrParams);
}
void SynthVoice::changeOsc(int oscNum) {
    if (oscNum == 0) {
        osc.initialise([](float x) { return std::sin(x); }, 128); //sine
    }
    else if (oscNum == 1) {
        osc.initialise([](float x) { return x / juce::MathConstants<float>::pi; }, 128); //saw
    }
    else if (oscNum == 2) {
        osc.initialise([](float x) { return x < 0.0f ? -1.0f : 1.0f; }, 128); //sqaure
    }
    else if (oscNum == 3) {
        osc.initialise([](float x) { return x < 0.0f ? -1.0f + (4.0f * (x + 0.5f)) : 1.0f - (4.0f * (x - 0.5f)); }, 128); //triangle
    }
}



juce::AudioProcessorValueTreeState::ParameterLayout BasicOscAudioProcessor::createParams() {
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    params.push_back(std::make_unique<juce::AudioParameterChoice>("OSC", "Oscillator", juce::StringArray{ "Sine", "Saw", "Square", "Triangle" }, 0));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("ATTACK", "Attack", juce::NormalisableRange{ 0.0f, 1.0f }, 0.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>("DECAY", "Decay", juce::NormalisableRange{ 0.0f, 1.0f }, 0.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>("SUSTAIN", "Sustain", juce::NormalisableRange{ 0.0f, 1.0f }, 1.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>("RELEASE", "Release", juce::NormalisableRange{ 0.0f, 3.0f }, 0.1f));

    return { params.begin(), params.end() };
}
