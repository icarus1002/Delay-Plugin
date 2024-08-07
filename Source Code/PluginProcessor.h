/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "StkLite-4.6.2/Delay.h"

//==============================================================================
/**
*/
class ZacharyWP03DelayAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    ZacharyWP03DelayAudioProcessor();
    ~ZacharyWP03DelayAudioProcessor() override;

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

private:
    
    // Functions created:
    void calcAlgorithmParams();
    int calcMsecToSamps(float msec);
    
    // User parameters:
    juce::AudioParameterFloat* mWetDryMixParam;
    juce::AudioParameterFloat* mDelayMsecParam;
    juce::AudioParameterFloat* mFeedbackDbParam;
    
    // Alg Parameters:
    float mFeedbackGainLin;
    float mWetGain, mDryGain;
    
    // Member Objects
    stk::Delay mDelay1L, mDelay1R;  // delay objects
    float mFs; // Sampling Rate
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ZacharyWP03DelayAudioProcessor)
};
