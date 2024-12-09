/*
  ==============================================================================

    bandpass_filter.h
    Created: 2 Dec 2024 3:47:56pm
    Author:  Jake Morgan

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

namespace termite {
class bandpass{
public:
    bandpass(){
    }
    ~bandpass(){
    }
    
    void prepareToPlay(float inSampleRate){
        sampleRate = inSampleRate;
    }
    /// this q takes values from 1-100
    float processSample(float inSample, float inFrequency, float inQ){
        float x = inSample;
        float cf = inFrequency;
        float q = inQ;
        float two_pi_over_sr = (2*juce::MathConstants<float>::pi) / sampleRate;
        float bw = cf/q;
        float r = (-bw*two_pi_over_sr);
        float c1 = 2*r*(cos(cf*two_pi_over_sr));
        float c2 = -r*r;
        
        float output = (1-r) * (x - r*x2) + (c1*y1) + (c2*y2);
        y2 = y1;
        y1 = output;
        x2 = x1;
        x1 = x;
        return output;
    }
private:
    
    float sampleRate = 44100.f;
    float frequency = 0.f;
    float q = 1.f;
    int single_sample_delay = 0;
    float x1 = 0;
    float x2 = 0;
    float y1 = 0;
    float y2 = 0;
    
};
}
