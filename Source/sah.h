/*
  ==============================================================================

    sah.h
    Created: 2 Dec 2024 4:44:29pm
    Author:  Jake Morgan

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

class sah {
public:
    sah(){}
    ~sah(){}
    
private:
    
    juce::LinearSmoothedValue<float> sampleSize = 0;
    
    
};
