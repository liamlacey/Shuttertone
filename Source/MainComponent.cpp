/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"


//==============================================================================
MainContentComponent::MainContentComponent()
{
    //Create a virtual MIDI output device
    midiOutputDevice = MidiOutput::createNewDevice("Generative MIDI App");
    
    if(midiOutputDevice)
        midiOutputDevice->startBackgroundThread();
    else
        std::cout << "Failed to create a virtual MIDI output device!" << std::endl;
    
    
    image = ImageCache::getFromFile (File ("/Users/Liam/Pictures/2-tree-and-sunset.jpg"));
    addAndMakeVisible(imageComponent = new ImageComponent());
    imageComponent->setImage (image);
    
    
    addAndMakeVisible(playButton = new TextButton());
    playButton->setClickingTogglesState(true);
    playButton->setButtonText("Play");
    playButton->addListener(this);
    
    
    setSize (600, 400);
}

MainContentComponent::~MainContentComponent()
{
}

void MainContentComponent::paint (Graphics& g)
{
    g.fillAll (Colour (0xff001F36));
}

void MainContentComponent::resized()
{
    imageComponent->setBounds(0, 0, getWidth(), getHeight() - 60);
    playButton->setBounds((getWidth()/2) - 50, getHeight() - 50, 100, 40);
}

void MainContentComponent::buttonClicked (Button *button)
{
    if (button == playButton)
    {
        if (button->getToggleState())
            button->setButtonText("Stop");
        else
            button->setButtonText("Play");
    }
}
