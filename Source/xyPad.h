/*
  ==============================================================================

    xyPad.h
    Created: 13 Nov 2024 8:20:15pm
    Author:  Jake Morgan

  ==============================================================================
*/

#pragma once
#include "JuceHeader.h"

class xyHandle : public juce::Component {
public:
    
    void setColour(juce::Colour c){
        colourSelect = &c;
    }
    
    void paint(juce::Graphics& g) override;
    
    void mouseEnter(const juce::MouseEvent& e) override;
    
    void mouseDown(const juce::MouseEvent& e) override;
    
    void mouseDrag(const juce::MouseEvent& e) override;
    
    void mouseExit(const juce::MouseEvent& e) override;
    
    std::function<void()> onHandleDragged = nullptr;
    
    void deviate(std::string pos);
    
private:
    juce::Colour* colourSelect = nullptr;
    
    juce::ComponentDragger dragger;
    
    float deviation = 0.f;
};
class xyBar : public juce::Component {
public:
    void paint(juce::Graphics& g) override;
};
class xyPad : public juce::Component {
public:
    
    //static const int handleSize = 10;
    static constexpr int handleSize = 10;
    
    xyPad();
    ~xyPad();
    
    void paint(juce::Graphics& g) override;
    void resized() override;
    
    float getYValue(){
        return juce::jmap((float)xyHandle.getBounds().getY(),0.f,(float)getHeight(),0.f,1.f);
    }
    float getYValueB(){
        return juce::jmap((float)xyHandleB.getBounds().getY(),0.f,(float)getHeight(),0.f,1.f);
    }
    float getYValueR(){
        return juce::jmap((float)xyHandleR.getBounds().getY(),0.f,(float)getHeight(),0.f,1.f);
    }
    
    void setOnHandleDragged(std::function<void()> inFunction){
        xyHandle.onHandleDragged = inFunction;
    }
    
    
private:
    
    xyBar xyBar;
    xyHandle xyHandle;
    class xyHandle xyHandleB;
    class xyHandle xyHandleR;
    
//    const juce::Colours::blue blue;
//    const juce::Colours::red red;
//    const juce::Colours::lime green;
};
