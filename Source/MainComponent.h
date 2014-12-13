/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"


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

private:
    //==============================================================================
    
    //=========================================
    //GUI stuff...
    ScopedPointer <TextButton> playButton;
    Image image;
    ScopedPointer <ImageComponent> imageComponent;
    
    //=========================================
    //Backend stuff...
    ScopedPointer<MidiOutput> midiOutputDevice;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED
