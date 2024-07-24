/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ZacharyWP03DelayAudioProcessorEditor::ZacharyWP03DelayAudioProcessorEditor (ZacharyWP03DelayAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    
    auto& params = processor.getParameters();
        
    juce::AudioParameterFloat* dryWetParameter = (juce::AudioParameterFloat*)params.getUnchecked(1);
        
    mDryWetSlider.setBounds(0, 125, 100, 100);
    mDryWetSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mDryWetSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 25);
    mDryWetSlider.setColour(mDryWetSlider.textBoxTextColourId, juce::Colours::white);
    mDryWetSlider.setRange(dryWetParameter->range.start, dryWetParameter->range.end, 0.01);
    mDryWetSlider.setValue(*dryWetParameter);
    addAndMakeVisible(mDryWetSlider);
        
    mDryWetLabel.setText("Dry/Wet", juce::dontSendNotification);
    mDryWetLabel.attachToComponent(&mDryWetSlider, false);
    mDryWetLabel.setJustificationType(juce::Justification::centredTop);
    mDryWetLabel.setColour(mDryWetLabel.textColourId, juce::Colours::white);
    addAndMakeVisible(mDryWetLabel);
        
    mDryWetSlider.onValueChange = [this, dryWetParameter] { *dryWetParameter = mDryWetSlider.getValue();};
    mDryWetSlider.onDragStart = [dryWetParameter] { dryWetParameter->beginChangeGesture(); };
    mDryWetSlider.onDragEnd = [dryWetParameter] { dryWetParameter->endChangeGesture(); };
        
    juce::AudioParameterFloat* feedbackParameter = (juce::AudioParameterFloat*)params.getUnchecked(0);
    
    mFeedbackSlider.setBounds(300, 125, 100, 100);
    mFeedbackSlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mFeedbackSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 25);
    mFeedbackSlider.setColour(mFeedbackSlider.textBoxTextColourId, juce::Colours::white);
    mFeedbackSlider.setRange(feedbackParameter->range.start, feedbackParameter->range.end, 0.01);
    mFeedbackSlider.setValue(*feedbackParameter);
    addAndMakeVisible(mFeedbackSlider);
        
    mFeedbackLabel.setText("Feedback", juce::dontSendNotification);
    mFeedbackLabel.attachToComponent(&mFeedbackSlider, false);
    mFeedbackLabel.setJustificationType(juce::Justification::centredTop);
    mFeedbackLabel.setColour(mFeedbackLabel.textColourId, juce::Colours::white);
    addAndMakeVisible(mFeedbackLabel);
        
    mFeedbackSlider.onValueChange = [this, feedbackParameter] { *feedbackParameter = mFeedbackSlider.getValue(); };
    mFeedbackSlider.onDragStart = [feedbackParameter] { feedbackParameter->beginChangeGesture(); };
    mFeedbackSlider.onDragEnd = [feedbackParameter] { feedbackParameter->endChangeGesture(); };
    
    juce::AudioParameterFloat* delayParameter = (juce::AudioParameterFloat*)params.getUnchecked(2);
    
    mDelaySlider.setBounds(150, 30, 100, 100);
    mDelaySlider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    mDelaySlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 25);
    mDelaySlider.setColour(mDelaySlider.textBoxTextColourId, juce::Colours::white);
    mDelaySlider.setRange(delayParameter->range.start, delayParameter->range.end, 0.01);
    mDelaySlider.setValue(*delayParameter);
    addAndMakeVisible(mDelaySlider);
        
    mDelayLabel.setText("Delay", juce::dontSendNotification);
    mDelayLabel.attachToComponent(&mDelaySlider, false);
    mDelayLabel.setJustificationType(juce::Justification::centredTop);
    mDelayLabel.setColour(mDelayLabel.textColourId, juce::Colours::white);
    addAndMakeVisible(mDelayLabel);
        
    mDelaySlider.onValueChange = [this, delayParameter] { *delayParameter = mDelaySlider.getValue(); };
    mDelaySlider.onDragStart = [delayParameter] { delayParameter->beginChangeGesture(); };
    mDelaySlider.onDragEnd = [delayParameter] { delayParameter->endChangeGesture(); };
    
    
    
}

ZacharyWP03DelayAudioProcessorEditor::~ZacharyWP03DelayAudioProcessorEditor()
{
}

//==============================================================================
void ZacharyWP03DelayAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Delay", getLocalBounds(), juce::Justification::centredBottom, 1);
}

void ZacharyWP03DelayAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
