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
class OneShotSmplerAudioProcessorEditor  : public juce::AudioProcessorEditor, private juce::Button::Listener
{
public:
    OneShotSmplerAudioProcessorEditor (OneShotSmplerAudioProcessor&);
    ~OneShotSmplerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void buttonClicked(juce::Button* button) override;
    
    juce::TextButton sineWaveButton;
    juce::TextButton sampleSelectionButton;
    juce::MidiKeyboardComponent keyboardComponent;
    
    OneShotSmplerAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OneShotSmplerAudioProcessorEditor)
};
