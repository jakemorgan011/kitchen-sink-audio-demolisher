/*
  ==============================================================================

    threshold_total.h
    Created: 12 Nov 2024 11:24:01pm
    Author:  Jake Morgan

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"
#include "stutter.h"
#include "lfo.h"

class ampMod{
public:
    ampMod(){}
    ~ampMod(){}
    
    void prepareToPlay(float inSampleRate){
        sampleRate = inSampleRate;
    }
    
    void processSample(float& sampleToProcess){
        sampleToProcess *= lfo.returnSample();
    }
    
    void processBlock(juce::AudioBuffer<float>& inBuffer){
        int num_samples = inBuffer.getNumSamples();
        float* left = inBuffer.getWritePointer(0);
        float* right = inBuffer.getWritePointer(1);
        
        for (int i = 0; i<num_samples; i++){
            processSample(left[i]);
            processSample(right[i]);
        }
    }
    
    void setFrequency(float inNorm){
        float freq = normToFreq(inNorm);
        lfo.setFrequency(freq);
    }
    float normToFreq(float inNorm){
        return juce::jlimit(0.001f, 21950.f, 10 * powf(2200.f, inNorm));
    }
    
private:
    float sampleRate;
    
    termite::sin_lfo lfo;
    
};

class ringMod {
public:
    ringMod(){
        
    }
    ~ringMod(){
        
    }
    
    void prepareToPlay(float inSampleRate){
        sampleRate = inSampleRate;
    }
    void processSample(float& sampleToProcess){
        sampleToProcess *= lfo.returnPolarSample();
    }
    void processBlock(juce::AudioBuffer<float>& inBuffer){
        int num_samples = inBuffer.getNumSamples();
        float* left = inBuffer.getWritePointer(0);
        float* right = inBuffer.getWritePointer(1);
        for(int i = 0; i<num_samples; i++){
            processSample(left[i]);
            processSample(right[i]);
        }
    }
    void setFrequency(float inNorm){
        float freq = normToFreq(inNorm);
        smoothedFrequency.setTargetValue(freq);
        lfo.setFrequency(freq);
    }
    float normToFreq(float inNorm){
        return juce::jlimit(0.001f, 21950.f, 10 * powf(2200.f, inNorm));
    }
    
private:
    
    termite::sin_lfo lfo;
    
    juce::LinearSmoothedValue<float> smoothedFrequency;
    
    float sampleRate = 44100.f;
    
};

class threshold {
public:
    
    threshold();
    ~threshold();
    
    void prepareToPlay(float inSampleRate);
    void processBlock(juce::AudioBuffer<float> &inBuffer, stutter& stutter);
    void setParameters(float inStutterWidth, float inStutterHeight, float inRingWidth, float inRingHeight, float inAmpWidth, float inAmpHeight);
    void setRingFreq(float inFreq){
        ringMod.setFrequency(inFreq);
    }
    void setAmpFreq(float inFreq){
        ampMod.setFrequency(inFreq);
    }
    
    
private:
    
    ringMod ringMod;
    ampMod ampMod;
    
    float sampleRate = 44100.f;
    
    juce::LinearSmoothedValue<float> smoothedTopThresh;
    juce::LinearSmoothedValue<float> smoothedBotThresh;
    juce::LinearSmoothedValue<float> smoothedRingTopThresh;
    juce::LinearSmoothedValue<float> smoothedRingBotThresh;
    juce::LinearSmoothedValue<float> smoothedAmpBotThresh;
    juce::LinearSmoothedValue<float> smoothedAmpTopThresh;
    
    juce::LinearSmoothedValue<float> smoothedThreshold;
    juce::LinearSmoothedValue<float> smoothedStutterWidth;
    juce::LinearSmoothedValue<float> smoothedRingWidth;
    juce::LinearSmoothedValue<float> smoothedAmpWidth;
    
    
};


