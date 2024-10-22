/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.
 
    Author: Brandon Ferrante

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Project4_TemplateAudioProcessor::Project4_TemplateAudioProcessor()
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
    /*
        Initialize Audio Parameters Variables
    */
    frequencyParameter = new juce::AudioParameterInt(juce::ParameterID("1", 1), "Frequency Knob", 20, 20000, 1000);
    gainParameter = new juce::AudioParameterFloat(juce::ParameterID("2", 2), "Gain Slider", -24.0f, 24.0f, 0.f);
    qParameter = new juce::AudioParameterFloat(juce::ParameterID("3", 3), "Q Knob", 0.10f, 100.0f, 1.0f);
    
    /*
        Add Audio Parameters to Parameter Tree
    */
    addParameter(frequencyParameter);
    addParameter(gainParameter);
    addParameter(qParameter);
    
    // Set current variables to use for checking
    currentFreq = *frequencyParameter;
    currentGain = *gainParameter;
    currentQ = *qParameter;
    
    // update/compute coefficients
    updateCoefficients();
    
    // initialize arrays with zeros
    auto numInputChannels = getTotalNumInputChannels();
    
    for (int i = 0; i < numInputChannels; i++)
    {
        a1_delay.set(i, 0.0f);
        a2_delay.set(i, 0.0f);
        b1_delay.set(i, 0.0f);
        b2_delay.set(i, 0.0f);
    }
    
    /*
        Initialize any properties of effect classes
    */
}

Project4_TemplateAudioProcessor::~Project4_TemplateAudioProcessor()
{
}

//==============================================================================
const juce::String Project4_TemplateAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Project4_TemplateAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool Project4_TemplateAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool Project4_TemplateAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double Project4_TemplateAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Project4_TemplateAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int Project4_TemplateAudioProcessor::getCurrentProgram()
{
    return 0;
}

void Project4_TemplateAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Project4_TemplateAudioProcessor::getProgramName (int index)
{
    return {};
}

void Project4_TemplateAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void Project4_TemplateAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    
    /*
        Initialize any variables or effect class properties
    */
    fs = sampleRate;
    resetDelays();
}

void Project4_TemplateAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Project4_TemplateAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void Project4_TemplateAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    
    
    // Check to see if parameters have changed
    if (currentFreq != *frequencyParameter || currentGain != *gainParameter || currentQ != *qParameter)
    {
        updateCoefficients();
        
        currentFreq = *frequencyParameter;
        currentGain = *gainParameter;
        currentQ = *qParameter;
    }
        

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    {
        buffer.clear (i, 0, buffer.getNumSamples());
    }

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < buffer.getNumChannels(); channel++)
    {
        auto *channelData = buffer.getWritePointer(channel);
        
        /*
            Place the difference equation here
        */
        for (int n = 0; n < buffer.getNumSamples(); n++)
        {
            float x = channelData[n];
            float z = (a0 * x) + (a1 * a1_delay[channel] +
                                  a2 * a2_delay[channel] -
                                  b1 * b1_delay[channel] -
                                  b2 * b2_delay[channel] );
            
            // Update delays
            a2_delay.set(channel, a1_delay[channel]);
            a1_delay.set(channel, x);
            b2_delay.set(channel, b1_delay[channel]);
            b1_delay.set(channel, z);
            
            channelData[n] = z * c0 + x * d0;
        }
    }
}

//==============================================================================
bool Project4_TemplateAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Project4_TemplateAudioProcessor::createEditor()
{
    return new Project4_TemplateAudioProcessorEditor (*this);
}

//==============================================================================
void Project4_TemplateAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Project4_TemplateAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Project4_TemplateAudioProcessor();
}


void Project4_TemplateAudioProcessor::updateCoefficients()
{
    int frequencyValue = *frequencyParameter;
    float gainValue = *gainParameter;
    float qValue = *qParameter;
    
    float theta = 2 * M_PI * frequencyValue / fs;
    float mu = std::pow(10.0f, (gainValue / 20.0f));
    float xi = 4 / (1 + mu);
    float beta = 0.5 * ((1 - xi * tan(theta / (2 * qValue))) / (1 + xi * tan(theta / (2 * qValue))));
    float gamma = (0.5 + beta) * cos(theta);
    
    a0 = 0.5 - beta;
    a1 = 0.0;
    a2 = -1 * (0.5 - beta);
    b1 = -2 * gamma;
    b2 = 2 * beta;
    c0 = mu - 1.0;
    d0 = 1.0;
    
}

void Project4_TemplateAudioProcessor::resetDelays()
{
    auto numInputChannels = getTotalNumInputChannels();
    
    for (int i = 0; i < numInputChannels; i++)
    {
        a1_delay.set(i, 0.0f);
        a2_delay.set(i, 0.0f);
        b1_delay.set(i, 0.0f);
        b2_delay.set(i, 0.0f);
    }
    
}
