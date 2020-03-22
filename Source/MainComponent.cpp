/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"


//==============================================================================
MainContentComponent::MainContentComponent()
{
    midiGenerator = std::make_unique<MidiGenerator>();
    
    addAndMakeVisible(&imageComponent);
    
    addAndMakeVisible(&playButton);
    playButton.setClickingTogglesState(true);
    playButton.setButtonText("Play");
    playButton.addListener(this);
    playButton.setColour(TextButton::buttonColourId, Colours::wheat);
    playButton.setColour(TextButton::buttonOnColourId, Colours::red);
    playButton.setEnabled(false);
    playButton.setAlpha(0.5);
    
    addAndMakeVisible(&loadImageButton);
    loadImageButton.setButtonText("Load Image");
    loadImageButton.addListener(this);
    loadImageButton.setColour(TextButton::buttonColourId, Colours::wheat);
    
    addAndMakeVisible(&fileNameLabel);
    fileNameLabel.setColour(Label::textColourId, Colours::wheat);
    fileNameLabel.setJustificationType(Justification::centred);
    
    addAndMakeVisible(&midiOutputComboBox);
    midiOutputComboBox.addListener(this);
    
    auto midiOutputDeviceListNames = midiGenerator->getMidiOutputListNames();
    
    for (int i = 0; i < midiOutputDeviceListNames.size(); i++)
    {
#if JUCE_MAC || JUCE_LINUX || JUCE_IOS
        if (i == midiOutputDeviceListNames.size() - 1)
            midiOutputComboBox.addSeparator();
#endif
        midiOutputComboBox.addItem (midiOutputDeviceListNames[i], i + 1);
    }
    
#if JUCE_MAC || JUCE_LINUX || JUCE_IOS
    midiOutputComboBox.setSelectedItemIndex (midiOutputComboBox.getNumItems() - 1, sendNotification);
#else
    midiOutputComboBox.setSelectedItemIndex (0, sendNotification);
#endif
    
    addAndMakeVisible(&midiOutputLabel);
    midiOutputLabel.setText("MIDI Ouput:", dontSendNotification);
    midiOutputLabel.setColour(Label::textColourId, Colours::wheat);
    midiOutputLabel.setJustificationType(Justification::centred);
    
    setSize (800, 560);
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
    imageComponent.setBounds(0, 0, getWidth(), getHeight() - 60);
    
    loadImageButton.setBounds((getWidth()/2) - 110, getHeight() - 50, 100, 40);
    playButton.setBounds((getWidth()/2) +10, getHeight() - 50, 100, 40);
    fileNameLabel.setBounds(0, getHeight()-90, getWidth(), 20);
    
    midiOutputComboBox.setBounds (getWidth() - 160, getHeight() - 45, 150, 20);
    midiOutputLabel.setBounds (getWidth() - 260, getHeight() - 45, 100, 20);
}

void MainContentComponent::buttonClicked (Button *button)
{
    if (button == &playButton)
    {
        if (button->getToggleState())
        {
            midiGenerator->startThread();
            
            button->setButtonText("Stop");
        }
        else
        {
            midiGenerator->stopThread(500);
            
            button->setButtonText("Play");
            
        }
        
    } //if (button == playButton)
    
    else if (button == &loadImageButton)
    {
        playButton.setEnabled(false);
        playButton.setAlpha(0.5);
        
        FileChooser myChooser ("Please select an image to load...",
                               File::getSpecialLocation(File::userPicturesDirectory),
                               "*.jpeg;*.jpg;*.png");
        
        if (myChooser.browseForFileToOpen())
        {
            image = ImageCache::getFromFile(myChooser.getResult());
            imageComponent.setImage(image);
            fileNameLabel.setText(myChooser.getResult().getFullPathName(), dontSendNotification);
            
            
            //stop the currently playing sequence
            midiGenerator->stopThread(500);
            playButton.setToggleState(false, dontSendNotification);
            playButton.setButtonText("Play");
            
            //get colour data from new image and generate sequence data from it
            analyseImage();
        }
        
        if (!image.isNull())
        {
            playButton.setEnabled(true);
            playButton.setAlpha(1.0);
        }
    }
}

void MainContentComponent::comboBoxChanged (ComboBox* comboBox)
{
    if (comboBox == &midiOutputComboBox)
    {
        midiGenerator->setMidiOutputDevice (comboBox->getSelectedItemIndex());
    }
}

void MainContentComponent::analyseImage()
{
    imageWidth = image.getWidth();
    imageHeight = image.getHeight();
    
    std::cout << "Image Width: " << imageWidth << std::endl;
    std::cout << "Image Height: " << image.getHeight() << std::endl;
    std::cout << "Is image RGB?: " << image.isRGB() << std::endl;
    std::cout << "Is image ARGB?: " << image.isARGB() << std::endl;
    
    std::cout << std::endl;
    
    //work out the average colour value of each of the 4 sections...
    
    int sectionStartX[NUM_IMG_SECTIONS];
    int sectionStartY[NUM_IMG_SECTIONS];
    int sectionEndX[NUM_IMG_SECTIONS];
    int sectionEndY[NUM_IMG_SECTIONS];
    
    //positions for dividing image into horizontal strips
    //section 1 (top strip)
    sectionStartX[0] = 0;
    sectionStartY[0] = 0;
    sectionEndX[0] = imageWidth;
    sectionEndY[0] = imageHeight / 4;
    
    //section 2 (middle-top strip)
    sectionStartX[1] = 0;
    sectionStartY[1] = imageHeight / 4;
    sectionEndX[1] = imageWidth;
    sectionEndY[1] = imageHeight / 2;
    
    //section 3 (middle-bottom strip)
    sectionStartX[2] = 0;
    sectionStartY[2] = imageHeight / 2;
    sectionEndX[2] = imageWidth;
    sectionEndY[2] = (imageHeight / 4) * 3;
    
    //section 4 (bottom strip)
    sectionStartX[3] = 0;
    sectionStartY[3] = (imageHeight / 4) * 3;
    sectionEndX[3] = imageWidth;
    sectionEndY[3] = imageHeight;
    
    //get the colour values of each section
    for (int section = 0; section < NUM_IMG_SECTIONS; section++)
    {
        std::cout << "Image Section: " << section << std::endl;
        
        float accRed = 0;
        float accGreen = 0;
        float accBlue = 0;
        float accBrightness = 0;
        float accPerceivedBrightness = 0;
        float accHue = 0;
        float accSaturation = 0;
        
        int  pixelCounter = 0;
        
        Colour pixelColour;
        
        //get the colour values of each pixel of this section
        for (int pixelX = sectionStartX[section]; pixelX < sectionEndX[section]; pixelX++)
        {
            for (int pixelY = sectionStartY[section]; pixelY < sectionEndY[section]; pixelY++)
            {
                pixelColour = image.getPixelAt(pixelX, pixelY);
                
                accRed += pixelColour.getFloatRed();
                accGreen += pixelColour.getFloatGreen();
                accBlue += pixelColour.getFloatBlue();
                accBrightness += pixelColour.getBrightness();
                accPerceivedBrightness += pixelColour.getPerceivedBrightness();
                accHue += pixelColour.getHue();
                accSaturation += pixelColour.getSaturation();
                
                pixelCounter++;
            }
        }
        
        std::cout << "Pixel Count: " << pixelCounter << std::endl;
        
        //set the average of each colour value
        midiGenerator->setAverageRed (section, accRed / (float)pixelCounter);
        midiGenerator->setAverageGreen (section, accGreen / (float)pixelCounter);
        midiGenerator->setAverageBlue (section, accBlue / (float)pixelCounter);
        midiGenerator->setAverageBrightness (section, accBrightness / (float)pixelCounter);
        midiGenerator->setAveragePerceivedBrightness (section, accPerceivedBrightness / (float)pixelCounter);
        midiGenerator->setAverageHue (section, accHue / (float)pixelCounter);
        midiGenerator->setAverageSaturation (section, accSaturation / (float)pixelCounter);
        
        std::cout << "Red: " << midiGenerator->getAverageRed(section) << " Blue: " << midiGenerator->getAverageBlue(section) << " Green:" << midiGenerator->getAverageGreen(section) << " Brightness: " << midiGenerator->getAverageBrightness(section) << " Perceived Brightness: " << midiGenerator->getAveragePerceivedBrightness(section) << " Hue: " << midiGenerator->getAverageHue(section) << " Saturation: " << midiGenerator->getAverageSaturation(section) << std::endl;
        std::cout << std::endl;
    }
    
    //get the colour values of the whole image, based on the values of the 4 sections
    
    float accRed = 0;
    float accGreen = 0;
    float accBlue = 0;
    float accBrightness = 0;
    float accPerceivedBrightness = 0;
    float accHue = 0;
    float accSaturation = 0;
    
    for (int section = 0; section < NUM_IMG_SECTIONS; section++)
    {
        accRed += midiGenerator->getAverageRed(section);
        accGreen += midiGenerator->getAverageGreen(section);
        accBlue += midiGenerator->getAverageBlue(section);
        accBrightness += midiGenerator->getAverageBrightness(section);
        accPerceivedBrightness += midiGenerator->getAveragePerceivedBrightness(section);
        accHue += midiGenerator->getAverageHue(section);
        accSaturation += midiGenerator->getAverageSaturation(section);
    }
    
    midiGenerator->setAverageRed (GLOBAL_SECTION, accRed / (float)NUM_IMG_SECTIONS);
    midiGenerator->setAverageGreen (GLOBAL_SECTION, accGreen / (float)NUM_IMG_SECTIONS);
    midiGenerator->setAverageBlue (GLOBAL_SECTION, accBlue / (float)NUM_IMG_SECTIONS);
    midiGenerator->setAverageBrightness (GLOBAL_SECTION, accBrightness / (float)NUM_IMG_SECTIONS);
    midiGenerator->setAveragePerceivedBrightness (GLOBAL_SECTION, accPerceivedBrightness / (float)NUM_IMG_SECTIONS);
    midiGenerator->setAverageHue (GLOBAL_SECTION, accHue / (float)NUM_IMG_SECTIONS);
    midiGenerator->setAverageSaturation (GLOBAL_SECTION, accSaturation / (float)NUM_IMG_SECTIONS);
    
    std::cout << "Whole Image" << std::endl;
    std::cout << "Red: " << midiGenerator->getAverageRed(GLOBAL_SECTION) << " Blue: " << midiGenerator->getAverageBlue(GLOBAL_SECTION) << " Green:" << midiGenerator->getAverageGreen(GLOBAL_SECTION) << " Brightness: " << midiGenerator->getAverageBrightness(GLOBAL_SECTION) << " Perceived Brightness: " << midiGenerator->getAveragePerceivedBrightness(GLOBAL_SECTION) << " Hue: " << midiGenerator->getAverageHue(GLOBAL_SECTION) << " Saturation: " << midiGenerator->getAverageSaturation(GLOBAL_SECTION) << std::endl;
    std::cout << std::endl;
    
    //generate sequence data
    midiGenerator->setSequenceData();
}
