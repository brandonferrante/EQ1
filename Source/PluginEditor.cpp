/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

    Author: Brandon Ferrante
  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Project4_TemplateAudioProcessorEditor::Project4_TemplateAudioProcessorEditor (Project4_TemplateAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (1000, 500);
    
    /*
        The constructor of the AudioProcessorEditor class is used to define the properties of the GUI components that you want to add in
        These components can include Sliders, Knobs, TextBoxes, SubWindows etc.
    */

    /*
        Step 1 : You have to push your GUI components onto the Component Tree and make them visible
        To do that we use the addAndMakeVisible() function
    */
    addAndMakeVisible(frequencySlider);
    addAndMakeVisible(gainSlider);
    addAndMakeVisible(qSlider);
    
    addAndMakeVisible(frequencyLabel);
    addAndMakeVisible(gainLabel);
    addAndMakeVisible(qLabel);
    
    /*
        Step 2: Use the functions defined for each GUI component class to stylise your components in the way you want it to be
    */
    /****************************************************************************/
    /****************************************************************************/

    frequencySlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    frequencySlider.setRange(20, 20000, 10);
    frequencySlider.setSkewFactorFromMidPoint(2000);
    frequencySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 50);
    frequencySlider.setTextValueSuffix(" Hz");
    frequencySlider.setSliderSnapsToMousePosition(true);
    frequencySlider.setColour(juce::Slider::thumbColourId, juce::Colours::hotpink);
    frequencySlider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::white);
    frequencySlider.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::black);
    frequencySlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    frequencySlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);

    frequencyLabel.setText("Frequency", juce::dontSendNotification);
    frequencyLabel.setFont(juce::Font(24.0f));
    frequencyLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    frequencyLabel.setJustificationType(juce::Justification::centred);
    frequencyLabel.attachToComponent(&frequencySlider, false);

    
    gainSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    gainSlider.setRange(-24, 24, 0.5);
    gainSlider.setSkewFactorFromMidPoint(0);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 50);
    gainSlider.setTextValueSuffix(" dB");
    gainSlider.setSliderSnapsToMousePosition(true);
    gainSlider.setColour(juce::Slider::backgroundColourId, juce::Colours::black);
    gainSlider.setColour(juce::Slider::trackColourId, juce::Colours::white);
    gainSlider.setColour(juce::Slider::thumbColourId, juce::Colours::red);
    gainSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    gainSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    
    gainLabel.setText("Gain", juce::dontSendNotification);
    gainLabel.setFont(juce::Font(24.0f));
    gainLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    gainLabel.setJustificationType(juce::Justification::centred);
    gainLabel.attachToComponent(&gainSlider, false);
    
    
    qSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    qSlider.setRange(0.10f, 100, 0.10f);
    qSlider.setSkewFactor(0.7f, false);
    qSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 100, 50);
    qSlider.setSliderSnapsToMousePosition(true);
    qSlider.setColour(juce::Slider::thumbColourId, juce::Colours::blue);
    qSlider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::white);
    qSlider.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::black);
    qSlider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::white);
    qSlider.setColour(juce::Slider::textBoxBackgroundColourId, juce::Colours::black);
    
    qLabel.setText("Q Factor", juce::dontSendNotification);
    qLabel.setFont(juce::Font(24.0f));
    qLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    qLabel.setJustificationType(juce::Justification::centred);
    qLabel.attachToComponent(&qSlider, false);
    
    /*
        Now for our Sliders and Knobs, we need to set their ranges. These ranges are dependent on the audio parameters we defined in the AudioProcessor Class
        In the AudioProcessorEditor class, we have a pointer to our AudioProcessor class. 
        We will use that pointer to get information of all the parameters declared in the AudioProcessor class
    */
    /*
        Step 3: Use the getParameters() function to get the audio parameter tree
    */
    auto parameterTree = audioProcessor.getParameters();
    
    /*
        Step 4: Use the function getUnchecked() to get the pointer to the specific parameter for your GUIs and we cast it to a AudioParameterFloat
    */
    frequencyParameter = (juce::AudioParameterInt*)parameterTree.getUnchecked(0);
    gainParameter = (juce::AudioParameterFloat*)parameterTree.getUnchecked(1);
    qParameter = (juce::AudioParameterFloat*)parameterTree.getUnchecked(2);
    
    /*
        Step 5: Use the function setRange() to set the Ranges of GUIs
    */
    auto frequencyRange = frequencyParameter->getRange();
    frequencySlider.setRange(frequencyRange.getStart(), frequencyRange.getEnd(), 1);
    
    gainSlider.setRange(gainParameter->range.start, gainParameter->range.end, gainParameter->range.interval);
    qSlider.setRange(qParameter->range.start, qParameter->range.end, qParameter->range.interval);
    
    /*
        Step 6: Set the default Value of the slider and knob
    */
    frequencySlider.setValue(1000, juce::NotificationType::dontSendNotification);
    gainSlider.setValue(0, juce::NotificationType::dontSendNotification);
    qSlider.setValue(1.0f, juce::NotificationType::dontSendNotification);
    
    /*
        Step 7 : Use the callback function to setValue of the audio processor parameters 
    */
    frequencySlider.onValueChange = [this]
    {
        *frequencyParameter = frequencySlider.getValue();
    };
    
    gainSlider.onValueChange = [this]
    {
        *gainParameter = gainSlider.getValue();
    };
    
    qSlider.onValueChange = [this]
    {
        *qParameter = qSlider.getValue();
    };
}


Project4_TemplateAudioProcessorEditor::~Project4_TemplateAudioProcessorEditor()
{
}

//==============================================================================
void Project4_TemplateAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
//    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
//
//    g.setColour (juce::Colours::white);
//    g.setFont (15.0f);
//    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
      g.fillAll(juce::Colours::grey);
}

void Project4_TemplateAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    /*
        Code written in this function is used to position and resize the GUI components on your window
    */

    /*
        Dividing the GUI window into a grid
    */
    auto x = getWidth() / 10;
    auto y = getHeight() / 10;
    
    // auto x = getWidth() / 10;
    // auto y = getHeight() / 10
    
    /*
        Use the setBounds() function to position the GUI Components (x,y) and set the width and height (width,height)
    */
    frequencySlider.setBounds(x, 5 * y, 2 * x, 4 * y);
    gainSlider.setBounds(4 * x, 3 * y, 2 * x, 6 * y);
    qSlider.setBounds(7 * x, 5 * y, 2 * x, 4 * y);
    
    frequencyLabel.setBounds(frequencySlider.getX(), frequencySlider.getY() - 40, frequencySlider.getWidth(), y);
    gainLabel.setBounds(gainSlider.getX(), gainSlider.getY() - 40, gainSlider.getWidth(), y);
    qLabel.setBounds(qSlider.getX(), qSlider.getY() - 40, qSlider.getWidth(), y);

    
    
}
