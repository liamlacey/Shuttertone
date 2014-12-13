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
    
    //image = ImageCache::getFromFile (File ("/Users/Liam/Pictures/300 Nature and City Full HD Wallpapers 1920 X 1080/Wallpapers/(11).jpg"));
    image = ImageCache::getFromFile (File ("/Users/Liam/Pictures/300 Nature and City Full HD Wallpapers 1920 X 1080/Wallpapers/(24).jpg"));
    
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
    
    //section 1 (top left)
    sectionStartX[0] = 0;
    sectionStartY[0] = 0;
    sectionEndX[0] = imageWidth / 2;
    sectionEndY[0] = imageHeight / 2;
    
    //section 2 (top right)
    sectionStartX[1] = imageWidth / 2;
    sectionStartY[1] = 0;
    sectionEndX[1] = imageWidth;
    sectionEndY[1] = imageHeight / 2;
    
    //section 3 (bottom left)
    sectionStartX[2] = 0;
    sectionStartY[2] = imageHeight / 2;
    sectionEndX[2] = imageWidth / 2;
    sectionEndY[2] = imageHeight;
    
    //section 4 (bottom right)
    sectionStartX[3] = imageWidth / 2;
    sectionStartY[3] = imageHeight / 2;
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
        
        //work out the average of each colour value
        averageRed[section] = accRed / (float)pixelCounter;
        averageGreen[section] = accGreen / (float)pixelCounter;
        averageBlue[section] = accBlue / (float)pixelCounter;
        averageBrightness[section] = accBrightness / (float)pixelCounter;
        averagePerceivedBrightness[section] = accPerceivedBrightness / (float)pixelCounter;
        averageHue[section] = accHue / (float)pixelCounter;
        averageSaturation[section] = accSaturation / (float)pixelCounter;
        
        std::cout << "Red: " << averageRed[section] << " Blue: " << averageBlue[section] << " Green:" << averageGreen[section] << " Brightness: " << averageBrightness[section] << " Perceived Brightness: " << averagePerceivedBrightness[section] << " Hue: " << averageHue[section] << " Saturation: " << averageSaturation[section] << std::endl;
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
        accRed += averageRed[section];
        accGreen += averageGreen[section];
        accBlue += averageBlue[section];
        accBrightness += averageBrightness[section];
        accPerceivedBrightness += averagePerceivedBrightness[section];
        accHue += averageHue[section];
        accSaturation += averageSaturation[section];
    }
    
    averageRed[GLOBAL_SECTION] = accRed / (float)NUM_IMG_SECTIONS;
    averageGreen[GLOBAL_SECTION] = accGreen / (float)NUM_IMG_SECTIONS;
    averageBlue[GLOBAL_SECTION] = accBlue / (float)NUM_IMG_SECTIONS;
    averageBrightness[GLOBAL_SECTION] = accBrightness / (float)NUM_IMG_SECTIONS;
    averagePerceivedBrightness[GLOBAL_SECTION] = accPerceivedBrightness / (float)NUM_IMG_SECTIONS;
    averageHue[GLOBAL_SECTION] = accHue / (float)NUM_IMG_SECTIONS;
    averageSaturation[GLOBAL_SECTION] = accSaturation / (float)NUM_IMG_SECTIONS;
    
    std::cout << "Whole Image" << std::endl;
    std::cout << "Red: " << averageRed[GLOBAL_SECTION] << " Blue: " << averageBlue[GLOBAL_SECTION] << " Green:" << averageGreen[GLOBAL_SECTION] << " Brightness: " << averageBrightness[GLOBAL_SECTION] << " Perceived Brightness: " << averagePerceivedBrightness[GLOBAL_SECTION] << " Hue: " << averageHue[GLOBAL_SECTION] << " Saturation: " << averageSaturation[GLOBAL_SECTION] << std::endl;
    std::cout << std::endl;

    
    

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
