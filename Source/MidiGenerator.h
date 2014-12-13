/*
  ==============================================================================

    MidiGenerator.h
    Created: 13 Dec 2014 2:39:09pm
    Author:  Liam Lacey

  ==============================================================================
*/

#ifndef MIDIGENERATOR_H_INCLUDED
#define MIDIGENERATOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class MidiGenerator : public Thread
{
    
public:
    //==============================================================================
    
    MidiGenerator();
    ~MidiGenerator();
    
    //Thread callback function
    void run();
    
    void setAverageRed (int section_num, float value);
    void setAverageGreen (int section_num, float value);
    void setAverageBlue (int section_num, float value);
    void setAverageBrightness (int section_num, float value);
    void setAveragePerceivedBrightness (int section_num, float value);
    void setAverageHue (int section_num, float value);
    void setAverageSaturation (int section_num, float value);
    
    float getAverageRed (int section_num);
    float getAverageGreen (int section_num);
    float getAverageBlue (int section_num);
    float getAverageBrightness (int section_num);
    float getAveragePerceivedBrightness (int section_num);
    float getAverageHue (int section_num);
    float getAverageSaturation (int section_num);
    
private:
    //==============================================================================
    
    ScopedPointer<MidiOutput> midiOutputDevice;
    
    //variables to store the average colour values for each image section (including a whole-image average)
    float averageRed[NUM_IMG_SECTIONS + 1];
    float averageGreen[NUM_IMG_SECTIONS + 1];
    float averageBlue[NUM_IMG_SECTIONS + 1];
    float averageBrightness[NUM_IMG_SECTIONS + 1];
    float averagePerceivedBrightness[NUM_IMG_SECTIONS + 1];
    float averageHue[NUM_IMG_SECTIONS + 1];
    float averageSaturation[NUM_IMG_SECTIONS + 1];
};



#endif  // MIDIGENERATOR_H_INCLUDED
