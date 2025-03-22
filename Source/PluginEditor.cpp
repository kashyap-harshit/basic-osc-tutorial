/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BasicOscAudioProcessorEditor::BasicOscAudioProcessorEditor (BasicOscAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);

    attackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.theValueTree, "ATTACK", attackSlider);

    decayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.theValueTree, "DECAY", decaySlider);

    sustainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.theValueTree, "SUSTAIN", sustainSlider);

    releaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.theValueTree, "RELEASE", releaseSlider);

    oscSelectAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.theValueTree, "OSC", oscSelector);


    attackSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    attackSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    addAndMakeVisible(attackSlider);

    decaySlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    decaySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    addAndMakeVisible(decaySlider);

    sustainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    sustainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    addAndMakeVisible(sustainSlider);

    releaseSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    releaseSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 50, 25);
    addAndMakeVisible(releaseSlider);

    oscSelector.addItem("Sine", 1);
    oscSelector.addItem("Saw", 2);
    oscSelector.addItem("Square", 3);
    oscSelector.addItem("Triangle", 4);

    oscSelector.setSelectedId(audioProcessor.theValueTree.getRawParameterValue("OSC")->load() + 1);

    oscSelector.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(oscSelector);


}

BasicOscAudioProcessorEditor::~BasicOscAudioProcessorEditor()
{
}

//==============================================================================
void BasicOscAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colour(251, 133, 0));
}

void BasicOscAudioProcessorEditor::resized()
{
    const auto padding = 12;
    const auto bounds = getLocalBounds().reduced(padding);
    const auto sliderWidth = bounds.getWidth() / 4 - padding;
    const auto sliderHeight = bounds.getWidth() / 2;
    const auto sliderStartX = 18;
    const auto sliderStartY = bounds.getHeight() / 2 - (sliderHeight / 4);


    attackSlider.setBounds(sliderStartX, sliderStartY, sliderWidth, sliderHeight);
    decaySlider.setBounds(attackSlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);

    sustainSlider.setBounds(decaySlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);

    releaseSlider.setBounds(sustainSlider.getRight() + padding, sliderStartY, sliderWidth, sliderHeight);

    oscSelector.setBounds(bounds.getX(), bounds.getY(), bounds.getWidth(), 30);
}
