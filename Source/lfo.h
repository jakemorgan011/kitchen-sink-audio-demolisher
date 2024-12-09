/*
  ==============================================================================

    lfo.h
    Created: 2 Dec 2024 3:47:37pm
    Author:  Jake Morgan

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

namespace termite {
class sin_lfo {
public:
    sin_lfo(){
    }
    ~sin_lfo(){
    }
    
    void prepareToPlay(float inSampleRate){
        sampleRate = inSampleRate;
    }
    float returnSample(){
        float sine_out = std::sin(phase * 2 * juce::MathConstants<float>::pi);
        phase += (frequency / sampleRate);
        if(phase > 1.f){
            phase -= 1.f;
        }
        return sine_out;
    }
    void setFrequency(float inFrequency){
        frequency = inFrequency;
    }
    /// returns positive side only.
    float returnPolarSample(){
        return (returnSample()/2) + 0.5;
    }
    
private:
    float sampleRate = 44100.f;
    float phase = 0;
    float frequency = 1.f;
    
};

class triangle_lfo {
public:
    triangle_lfo(){
        
    }
    ~triangle_lfo(){
        
    }
    
private:
};

}
