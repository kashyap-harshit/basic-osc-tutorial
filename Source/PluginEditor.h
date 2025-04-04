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
    juce::Slider attackSlider;
    juce::Slider decaySlider;
    juce::Slider sustainSlider;
    juce::Slider releaseSlider;
    juce::ComboBox oscSelector;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackAttachment;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> decayAttachment;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sustainAttachment;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> releaseAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> oscSelectAttachment;

    
    
    BasicOscAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BasicOscAudioProcessorEditor)
};
