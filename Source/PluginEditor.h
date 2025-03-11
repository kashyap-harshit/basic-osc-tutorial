/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class BasicOscAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    BasicOscAudioProcessorEditor (BasicOscAudioProcessor&);
    ~BasicOscAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    
    
    BasicOscAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BasicOscAudioProcessorEditor)
};
