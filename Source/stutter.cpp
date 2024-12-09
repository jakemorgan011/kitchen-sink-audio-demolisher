/*
  ==============================================================================

    stutter.cpp
    Created: 12 Nov 2024 11:27:24pm
    Author:  Jake Morgan

  ==============================================================================
*/

#include "stutter.h"

stutter::stutter(){
    
}

stutter::~stutter(){
    
}

void stutter::prepareToPlay(float inSampleRate){
    sampleRate = inSampleRate;
    smoothedStutterTime.reset(sampleRate, 0.01);
    circularBuffer.setSize(2, max_delay_in_samples*2);
}

void stutter::processBlock(juce::AudioBuffer<float>& inBuffer){
    
    float* in_channel_left = inBuffer.getWritePointer(0);
    float* in_channel_right = inBuffer.getWritePointer(1);
    
    float* circular_channel_left = circularBuffer.getWritePointer(0);
    float* circular_channel_right = circularBuffer.getWritePointer(1);
    
    int numSamples = inBuffer.getNumSamples();
    float stutterTime = smoothedStutterTime.getNextValue();
    int delayTime = max_delay_in_samples * stutterTime;
    
    for(int i = 0; i<numSamples;i++){
        float input_left = in_channel_left[i];
        float input_right = in_channel_right[i];
        
        in_channel_left[i] = 0.f;
        in_channel_right[i] = 0.f;
        
        circular_channel_left[writehead] = input_left + feedbackLeft;
        circular_channel_right[writehead] = input_right + feedbackRight;
        
        readhead = writehead - delayTime;
        if(readhead <0){
            readhead = circularBuffer.getNumSamples() + readhead;
        }
        
        float delay_left = circular_channel_left[readhead];
        float delay_right = circular_channel_right[readhead];
        
        feedbackLeft = delay_left * feedback_amount;
        feedbackRight = delay_right * feedback_amount;
        
        in_channel_left[i] = delay_left;
        in_channel_left[i] = delay_right;
        in_channel_left[i] = bandpass.processSample(in_channel_left[i], 1000, 60);
        in_channel_right[i] = bandpass.processSample(in_channel_right[i], 1000, 60);
        writehead++;
        if(writehead >= circularBuffer.getNumSamples()){
            writehead = 0;
        }
    }
    
}

void stutter::setParameters(float inTime){
    smoothedStutterTime.setTargetValue(inTime);
}
