/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.
 
    Author: Brandon Ferrante

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class Project4_TemplateAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    Project4_TemplateAudioProcessorEditor (Project4_TemplateAudioProcessor&);
    ~Project4_TemplateAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;


private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    Project4_TemplateAudioProcessor& audioProcessor;
    
    juce::Slider frequencySlider;
    juce::Slider gainSlider;
    juce::Slider qSlider;
    
    juce::Label frequencyLabel;
    juce::Label gainLabel;
    juce::Label qLabel;
    
    juce::AudioParameterInt *frequencyParameter;
    juce::AudioParameterFloat  *gainParameter;
    juce::AudioParameterFloat *qParameter;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Project4_TemplateAudioProcessorEditor)
};
