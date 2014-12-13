/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

enum ImageSections
{
    IMG_SECTION_1 = 0,
    IMG_SECTION_2,
    IMG_SECTION_3,
    IMG_SECTION_4,
    
    NUM_IMG_SECTIONS
};

#define GLOBAL_SECTION NUM_IMG_SECTIONS

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
    //GUI/Image stuff...
    ScopedPointer <TextButton> playButton;
    
    Image image;
    int imageWidth, imageHeight;
    
    //varialbles to store the average colour values for each image section (including a whole-image average)
    float averageRed[NUM_IMG_SECTIONS + 1];
    float averageGreen[NUM_IMG_SECTIONS + 1];
    float averageBlue[NUM_IMG_SECTIONS + 1];
    float averageBrightness[NUM_IMG_SECTIONS + 1];
    float averagePerceivedBrightness[NUM_IMG_SECTIONS + 1];
    float averageHue[NUM_IMG_SECTIONS + 1];
    float averageSaturation[NUM_IMG_SECTIONS + 1];
    
    ScopedPointer <ImageComponent> imageComponent;
    
    //=========================================
    //Backend/MIDI stuff...
    ScopedPointer<MidiOutput> midiOutputDevice;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED
