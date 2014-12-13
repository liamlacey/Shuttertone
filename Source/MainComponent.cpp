/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"


//==============================================================================
MainContentComponent::MainContentComponent()
                        
{
    midiGenerator = new MidiGenerator();
    
    //image = ImageCache::getFromFile (File ("/Users/Liam/Pictures/300 Nature and City Full HD Wallpapers 1920 X 1080/Wallpapers/(11).jpg"));
    image = ImageCache::getFromFile (File ("/Users/Liam/Pictures/300 Nature and City Full HD Wallpapers 1920 X 1080/Wallpapers/(24).jpg"));
    //image = ImageCache::getFromFile (File ("/Users/Liam/Pictures/300 Nature and City Full HD Wallpapers 1920 X 1080/Wallpapers/(27).jpg"));
    //image = ImageCache::getFromFile (File ("/Users/Liam/Pictures/300 Nature and City Full HD Wallpapers 1920 X 1080/Wallpapers/(55).jpg"));
    //image = ImageCache::getFromFile (File ("/Users/Liam/Pictures/300 Nature and City Full HD Wallpapers 1920 X 1080/Wallpapers/(135).jpg"));
    
    
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
    
//    //positions for dividing image into corners
//    //section 1 (top left)
//    sectionStartX[0] = 0;
//    sectionStartY[0] = 0;
//    sectionEndX[0] = imageWidth / 2;
//    sectionEndY[0] = imageHeight / 2;
//    
//    //section 2 (top right)
//    sectionStartX[1] = imageWidth / 2;
//    sectionStartY[1] = 0;
//    sectionEndX[1] = imageWidth;
//    sectionEndY[1] = imageHeight / 2;
//    
//    //section 3 (bottom left)
//    sectionStartX[2] = 0;
//    sectionStartY[2] = imageHeight / 2;
//    sectionEndX[2] = imageWidth / 2;
//    sectionEndY[2] = imageHeight;
//    
//    //section 4 (bottom right)
//    sectionStartX[3] = imageWidth / 2;
//    sectionStartY[3] = imageHeight / 2;
//    sectionEndX[3] = imageWidth;
//    sectionEndY[3] = imageHeight;
    
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
    
    midiGenerator->setSequenceData();

    addAndMakeVisible(imageComponent = new ImageComponent());
    imageComponent->setImage (image);
    
    addAndMakeVisible(playButton = new TextButton());
    playButton->setClickingTogglesState(true);
    playButton->setButtonText("Play");
    playButton->addListener(this);
    
    
    setSize (1000, 700);
    
    
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
}
