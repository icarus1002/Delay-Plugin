/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ZacharyWP03DelayAudioProcessor::ZacharyWP03DelayAudioProcessor()
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
    // Instantiating New Parameters
    
    // Feedback Gain Param
    addParameter(mFeedbackDbParam = new juce::AudioParameterFloat(
                                                                  "Feedback Gain (Db)", // paramID,
                                                                  "FeedbackDb", // param name,
                                                                  -20.0f, // min value,
                                                                  20.0f, // max value,
                                                                  0.0f // default value,
                                                                        ));
    // Wet Dry Mix Param
    addParameter(mWetDryMixParam = new juce::AudioParameterFloat(
                                                                 "Wet and Dry Mix", // paramID,
                                                                 "WetDryMix", // param name,
                                                                 0.0f, // min value,
                                                                 100.0f, // max value,
                                                                 50.0f // default value,
                                                                       ));
    // Msec Delay Param
    addParameter(mDelayMsecParam = new juce::AudioParameterFloat(
                                                                 "Delay Msec", // paramID,
                                                                 "DelayMsec", // param name,
                                                                 5.0f, // min value,
                                                                 10000.0f, // max value,
                                                                 500.0f // default value,
                                                                       ));
}

ZacharyWP03DelayAudioProcessor::~ZacharyWP03DelayAudioProcessor()
{
}

//==============================================================================
const juce::String ZacharyWP03DelayAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ZacharyWP03DelayAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ZacharyWP03DelayAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ZacharyWP03DelayAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ZacharyWP03DelayAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ZacharyWP03DelayAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ZacharyWP03DelayAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ZacharyWP03DelayAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ZacharyWP03DelayAudioProcessor::getProgramName (int index)
{
    return {};
}

void ZacharyWP03DelayAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ZacharyWP03DelayAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Get system sampling rate
    mFs = sampleRate;
    
    // Initialize Delay objects w/ maximum length
    int maxSamps = calcMsecToSamps(10000);
    mDelay1L.setMaximumDelay(maxSamps);
    mDelay1R.setMaximumDelay(maxSamps);
}

void ZacharyWP03DelayAudioProcessor::calcAlgorithmParams()
{
    
    // Wet and Dry gains
    mWetGain = mWetDryMixParam->get()/100.0;
    mDryGain = 1.0 - mWetGain;
    
    // Set Delay times
    int samps = calcMsecToSamps(mDelayMsecParam->get());
    mDelay1L.setDelay(samps);
    mDelay1R.setDelay(samps);
    
    // set feedback gain
    mFeedbackGainLin = powf(10.0, mFeedbackDbParam->get()*0.05);
    
}

int ZacharyWP03DelayAudioProcessor::calcMsecToSamps(float msec)
{
    // msec = mDelayMsecParam->get();
    float numSamps = (msec / 1000) * mFs;
    return numSamps;
}

void ZacharyWP03DelayAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ZacharyWP03DelayAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void ZacharyWP03DelayAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    // Update Param Values
    calcAlgorithmParams();
    
    // Process Audio Samples
    float* channelDataLeft = buffer.getWritePointer(0);
    float* channelDataRight = buffer.getWritePointer(1);
    
    // temp variables
    float varL, varR;

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    
    
    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    
    const int numSamples = buffer.getNumSamples();
    
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int i = 0; i < numSamples; ++i)
    {
        // get output of delay
            // varL = mDelay1L.tick(channelDataLeft[samp]);
            // varR = mDelay1R.tick(channelDataRight[samp]);
        
        // get next out
        varL = mDelay1L.nextOut();
        varR = mDelay1R.nextOut();
        
        // mix wet and dry into output
            //outL += mWetGain * hearL;
            //outR =+ mWetGain * hearR;
        
        // compute next input to filter with feedback and tick
        mDelay1L.tick(mWetGain*channelDataLeft[i] + mFeedbackGainLin);
        mDelay1R.tick(mWetGain*channelDataRight[i] + mFeedbackGainLin);
        
        // send out
            //channelDataLeft[samp] = outL;
            //channelDataRight[samp] = outR;
        
        // mix wet and dry and send out
        channelDataLeft[i] = mDryGain * channelDataLeft[i] + mWetGain * varL;
        channelDataRight[i] = mDryGain * channelDataRight[i] + mWetGain * varR;
        
        
    }
    
    
    
}

//==============================================================================
bool ZacharyWP03DelayAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ZacharyWP03DelayAudioProcessor::createEditor()
{
    return new ZacharyWP03DelayAudioProcessorEditor (*this);
}

//==============================================================================
void ZacharyWP03DelayAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void ZacharyWP03DelayAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ZacharyWP03DelayAudioProcessor();
}
