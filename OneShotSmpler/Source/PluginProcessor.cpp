/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
OneShotSmplerAudioProcessor::OneShotSmplerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

OneShotSmplerAudioProcessor::~OneShotSmplerAudioProcessor()
{
}

//==============================================================================
const juce::String OneShotSmplerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool OneShotSmplerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool OneShotSmplerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool OneShotSmplerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double OneShotSmplerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int OneShotSmplerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int OneShotSmplerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void OneShotSmplerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String OneShotSmplerAudioProcessor::getProgramName (int index)
{
    return {};
}

void OneShotSmplerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void OneShotSmplerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate(sampleRate);

    keyboardState.reset();
}

void OneShotSmplerAudioProcessor::releaseResources()
{
    keyboardState.allNotesOff(0);

    keyboardState.reset();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool OneShotSmplerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
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

void OneShotSmplerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    if(isChanging) {return;}
    
    juce::ScopedNoDenormals noDenormals;
    int totalNumInputChannels  = getTotalNumInputChannels();
    int totalNumOutputChannels = getTotalNumOutputChannels();
    
    keyboardState.processNextMidiBuffer(midiMessages, 0, buffer.getNumSamples(), true);

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool OneShotSmplerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* OneShotSmplerAudioProcessor::createEditor()
{
    return new OneShotSmplerAudioProcessorEditor (*this);
}

//==============================================================================
void OneShotSmplerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void OneShotSmplerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}


//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new OneShotSmplerAudioProcessor();
}

void OneShotSmplerAudioProcessor::setupSampler(juce::AudioFormatReader& newReader)
{
    isChanging = true;
    
    synth.clearSounds();
    synth.clearVoices();
    
    juce::BigInteger allNotes;
    allNotes.setRange(0, 128, true);
    
    synth.addSound(new juce::SamplerSound("default", newReader, allNotes, 60, 0, 0.1, 10.0));
    
    for (int i = 0; i < 128; i++) {
        synth.addVoice(new juce::SamplerVoice());
    }
    
    isChanging = false;
}

void OneShotSmplerAudioProcessor::loadSineWave()
{
    juce::AudioFormatManager formatManager;
    formatManager.registerBasicFormats();

    juce::MemoryInputStream* inputStream = new juce::MemoryInputStream(BinaryData::sine_wav, BinaryData::sine_wavSize, true);

//    juce::AudioFormatReader* reader = formatManager.createReaderFor(inputStream);
//
//    if (reader != nullptr)
//    {
//        setupSampler(*reader);
//        delete reader;
//    }
}


void OneShotSmplerAudioProcessor::loadSampleFile()
{
    juce::AudioFormatManager formatManager;
    formatManager.registerBasicFormats();

    juce::FileChooser chooser("Open audio file to play.", formatManager.getWildcardForAllFormats());

    if (chooser.browseForFileToOpen())
    {
        juce::File file(chooser.getResult());
        juce::AudioFormatReader* reader = formatManager.createReaderFor(chooser.getResult());

        if (reader != nullptr)
        {
            setupSampler(*reader);

            delete reader;
        }
    }
}
