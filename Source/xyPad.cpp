/*
  ==============================================================================

    xyPad.cpp
    Created: 13 Nov 2024 8:20:15pm
    Author:  Jake Morgan

  ==============================================================================
*/

#include "xyPad.h"

// bar

void xyBar::paint(juce::Graphics& g){
    juce::Path p;
    // erm i guess
    auto barRect = getLocalBounds().removeFromBottom(120).removeFromTop(120).toFloat();
    p.addRectangle(barRect);
    g.setColour(juce::Colours::lime);
    g.fillPath(p);
}



// handle

void xyHandle::paint(juce::Graphics& g){
    juce::Path p;
    g.setColour(colourSelect);
    //g.fillPath(p);
    g.fillEllipse(250, 125, 20, 20);
    g.fillEllipse(getLocalBounds().toFloat());
}

void xyHandle::mouseDown(const juce::MouseEvent& e){
    dragger.startDraggingComponent(this, e);
    setMouseCursor(juce::MouseCursor::NoCursor);
}

void xyHandle::mouseEnter(const juce::MouseEvent& e){
    setMouseCursor(juce::MouseCursor::DraggingHandCursor);
}

void xyHandle::mouseDrag(const juce::MouseEvent& e){
    dragger.dragComponent(this, e, nullptr);
    auto parent_component = getParentComponent();
    
    auto x = juce::jlimit(0, parent_component->getWidth(), getBounds().getCentreX());
    auto y = juce::jlimit(0, parent_component->getHeight(), getBounds().getCentreY());
    
    setCentrePosition(x, y);
    
    juce::NullCheckedInvocation::invoke(onHandleDragged);
    
    parent_component->repaint();
}



void xyHandle::mouseExit(const juce::MouseEvent& e){
    setMouseCursor(juce::MouseCursor::NormalCursor);
}

void xyHandle::deviate(std::string pos){
    // theres actually no reason to use string.
    // idk why i did.
    // make not not to do it next time.
    int left = pos.compare("left");
    int right = pos.compare("right");
    if(left == 0){
        deviation = -100;
    }if(right  == 0){
        deviation = 100;
    }else{
        deviation = 0;
    }
}


// pad

xyPad::xyPad(){
//    xyHandleB.setColour(juce::Colours::blue);
//    xyHandleR.setColour(juce::Colours::red);
//    xyHandle.setColour(juce::Colours::lime);
    xyHandle.colourSelect = juce::Colours::lime;
    xyHandleR.colourSelect = juce::Colours::red;
    xyHandleB.colourSelect = juce::Colours::blue;
    xyHandle.deviate("none");
    xyHandleR.deviate("left");
    xyHandleB.deviate("right");
    addAndMakeVisible(xyHandle);
    addAndMakeVisible(xyHandleB);
    addAndMakeVisible(xyHandleR);
    addAndMakeVisible(xyBar);
}

xyPad::~xyPad(){
}

void xyPad::paint(juce::Graphics& g){
    xyHandle.setColour(juce::Colours::blue);
    xyHandleB.setColour(juce::Colours::lime);
    xyHandleR.setColour(juce::Colours::red);
}

void xyPad::resized(){
    xyHandle.setBounds(getWidth()/2 - handleSize/2, getHeight()/2 - handleSize/2, handleSize, handleSize);
    xyHandleB.setBounds(((getWidth()/4 - handleSize/2) -50), getHeight()/2 - handleSize/2, handleSize, handleSize);
    xyHandleR.setBounds(((getWidth()/2 - handleSize/2) -50), getHeight()/2 - handleSize/2, handleSize, handleSize);
}
