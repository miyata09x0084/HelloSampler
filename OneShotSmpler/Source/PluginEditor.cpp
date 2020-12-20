/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
OneShotSmplerAudioProcessorEditor::OneShotSmplerAudioProcessorEditor (OneShotSmplerAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
    , keyboardComponent(p.getMidiKeyboardState(), juce::MidiKeyboardComponent::horizontalKeyboard)
{
    sineWaveButton.setButtonText("Sine Wave");
    sineWaveButton.addListener(this);
    sampleSelectionButton.setButtonText("Sample Select");
    sampleSelectionButton.addListener(this);
    
    addAndMakeVisible(sineWaveButton);
    addAndMakeVisible(sampleSelectionButton);
    addAndMakeVisible(keyboardComponent);
    
    setSize(800, 600);
}

OneShotSmplerAudioProcessorEditor::~OneShotSmplerAudioProcessorEditor()
{
}

//==============================================================================
void OneShotSmplerAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void OneShotSmplerAudioProcessorEditor::resized()
{
    sineWaveButton.setBoundsRelative(0.2, 0.2, 0.2, 0.2);
    sampleSelectionButton.setBoundsRelative(0.6, 0.2, 0.2, 0.2);
    keyboardComponent.setBoundsRelative(0.0, 0.7, 1.0, 0.3);
}

void OneShotSmplerAudioProcessorEditor::buttonClicked(juce::Button* button)
{
    if (button == &sineWaveButton) {
        processor.loadSineWave();
    }
    else if (button == &sampleSelectionButton) {
        processor.loadSampleFile();
    }

}
