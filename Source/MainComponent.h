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
                                public Button::Listener,
                                public ComboBox::Listener
{
public:
    //==============================================================================
    MainContentComponent();
    ~MainContentComponent();

    //=========================================
    //GUI stuff...
    
    void paint (Graphics&) override;
    void resized() override;
    
    void buttonClicked (Button *button) override;
    void comboBoxChanged (ComboBox* comboBox) override;
    
    //=========================================
    //Backend stuff...
    void analyseImage();

private:
    //==============================================================================
    
    //=========================================
    //GUI/Image stuff...
    TextButton playButton;
    TextButton loadImageButton;
    
    Image image;
    int imageWidth, imageHeight;
    
    ImageComponent imageComponent;
    
    Label fileNameLabel;
    
    ComboBox midiOutputComboBox;
    Label midiOutputLabel;
    
    //=========================================
    //Backend/MIDI stuff...
    std::unique_ptr<MidiGenerator> midiGenerator;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};

#endif  // MAINCOMPONENT_H_INCLUDED
