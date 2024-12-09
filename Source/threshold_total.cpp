/*
  ==============================================================================

    threshold_total.cpp
    Created: 12 Nov 2024 11:24:01pm
    Author:  Jake Morgan

  ==============================================================================
*/

#include "threshold_total.h"

threshold::threshold(){
    
}
threshold::~threshold(){
    
}

void threshold::prepareToPlay(float inSampleRate){
    sampleRate = inSampleRate;
    smoothedThreshold.reset(sampleRate,0.01f);
    smoothedStutterWidth.reset(sampleRate,0.01f);
}

void threshold::processBlock(juce::AudioBuffer<float>& inBuffer, stutter& stutter){
    
    float* left_channel = inBuffer.getWritePointer(0);
    float* right_channel = inBuffer.getWritePointer(1);
    juce::AudioBuffer<float> copyS;
    juce::AudioBuffer<float> copyR;
    juce::AudioBuffer<float> copyD;
    copyS.makeCopyOf(inBuffer);
    copyR.makeCopyOf(inBuffer);
    copyD.makeCopyOf(inBuffer);
    
    // at the moment this creates a lot of artifacts.
    // whats next is to change this from of buffer by buffer system to a sample by sample system.
    // probably going to change stutter delay to a sample and hold thing. idk
    // have to make another prototype for the sample and hold thing then.
    stutter.processBlock(copyS);
    ringMod.processBlock(copyR);
    ampMod.processBlock(copyD);
    
    float* s_copy_left = copyS.getWritePointer(0);
    float* s_copy_right = copyS.getWritePointer(1);
    float* r_copy_left = copyR.getWritePointer(0);
    float* r_copy_right = copyR.getWritePointer(1);
    float* d_copy_left = copyD.getWritePointer(0);
    float* d_copy_right = copyD.getWritePointer(1);
    
    int num_samples = inBuffer.getNumSamples();
    for(int i = 0; i< num_samples; i++){
        
        float abs_left = left_channel[i];
        if (abs_left < 0){abs_left *= -1;}
        float abs_right = right_channel[i];
        if (abs_right < 0){abs_right *= -1;}
        
        float topThresh = smoothedTopThresh.getNextValue();
        float botThresh = smoothedBotThresh.getNextValue();
        float ringTop = smoothedRingTopThresh.getNextValue();
        float ringBot = smoothedRingBotThresh.getNextValue();
        float ampTop = smoothedAmpTopThresh.getNextValue();
        float ampBot = smoothedAmpBotThresh.getNextValue();
        
        // stutter
        if(abs_left < topThresh && abs_left > botThresh){
            left_channel[i] = s_copy_left[i];
        }else{left_channel[i] = left_channel[i];}
        if(abs_right < topThresh && abs_right > botThresh){
            right_channel[i] = s_copy_right[i];
        }else{right_channel[i] = right_channel[i];}
        //
        //ring
        if(abs_left < ringTop && abs_left > ringBot){
            left_channel[i] = r_copy_left[i];
        }else{left_channel[i] = left_channel[i];}
        if(abs_right < ringTop && abs_left > ringBot){
            right_channel[i] = r_copy_right[i];
        }else{right_channel[i] = right_channel[i];}
        //
        //amp
        if(abs_left < ampTop && abs_left > ampBot){
            left_channel[i] = d_copy_left[i];
        }else{left_channel[i] = left_channel[i];}
        if(abs_right < ampTop && abs_right > ampBot){
            right_channel[i] = d_copy_right[i];
        }else{right_channel[i] = right_channel[i];}
        //
        
        //idek
    }
}

void threshold::setParameters(float inStutterWidth, float inStutterHeight, float inRingWidth, float inRingHeight, float inAmpWidth, float inAmpHeight){
    
    smoothedThreshold.setTargetValue(inStutterHeight);
    smoothedTopThresh.setTargetValue(inStutterHeight+(inStutterWidth*0.5f));
    smoothedBotThresh.setTargetValue(inStutterHeight-(inStutterWidth*0.5f));
    smoothedRingTopThresh.setTargetValue(inRingHeight+(inRingWidth*0.5f));
    smoothedRingBotThresh.setTargetValue(inRingHeight-(inRingWidth*0.5f));
    smoothedAmpBotThresh.setTargetValue(inAmpHeight+(inAmpWidth*0.5f));
    smoothedAmpTopThresh.setTargetValue(inAmpHeight-(inAmpWidth*0.5));
    
}
