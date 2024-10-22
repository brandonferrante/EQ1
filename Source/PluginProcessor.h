/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.
 
    Author: Brandon Ferrante

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class Project4_TemplateAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    Project4_TemplateAudioProcessor();
    ~Project4_TemplateAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    void updateCoefficients();
    
    void resetDelays();

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Project4_TemplateAudioProcessor)
    
    /*
        Declare Audio Parameters Variables
    */
    juce::AudioParameterInt *frequencyParameter;
    juce::AudioParameterFloat *gainParameter;
    juce::AudioParameterFloat *qParameter;
    
    double fs = 44100;
    
    float a0;
    float a1;
    float a2;
    float b1;
    float b2;
    float c0;
    float d0;
    
    juce::Array<float> a1_delay, a2_delay, b1_delay, b2_delay;
    
    int currentFreq;
    float currentGain;
    float currentQ;
    
    /*
        Declare Effect Classes
    */
    
};
