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
class ZacharyWP03DelayAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    ZacharyWP03DelayAudioProcessorEditor (ZacharyWP03DelayAudioProcessor&);
    ~ZacharyWP03DelayAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ZacharyWP03DelayAudioProcessor& audioProcessor;
    
    juce::Slider mDryWetSlider;
    juce::Slider mFeedbackSlider;
    juce::Slider mDelaySlider;
    
    juce::Label mDryWetLabel;
    juce::Label mFeedbackLabel;
    juce::Label mDelayLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ZacharyWP03DelayAudioProcessorEditor)
};
