/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "MidiGenerator.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   :  public Component,
                                public Button::Listener
{
public:
    //==============================================================================
    MainContentComponent();
    ~MainContentComponent();

    //=========================================
    //GUI stuff...
    
    void paint (Graphics&);
    void resized();
    
    void buttonClicked (Button *button);
    
    //=========================================
    //Backend stuff...
    void analyseImage();
    

private:
    //==============================================================================
    
    //=========================================
    //GUI/Image stuff...
    ScopedPointer <TextButton> playButton;
    ScopedPointer<TextButton> loadImageButton;
    
    Image image;
    int imageWidth, imageHeight;
    
    ScopedPointer <ImageComponent> imageComponent;
    
    //=========================================
    //Backend/MIDI stuff...
    ScopedPointer<MidiGenerator> midiGenerator;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED
