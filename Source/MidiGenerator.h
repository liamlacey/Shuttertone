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
#include "Scales.h"

//This is my first attempt at a struct for storing note data so we can play a sequence of notes.
//When searching through arrays of this struct, it should only process the object/note
//if the current time/step matches the step number. This method could prove to be CPU extensive
//as for each step in the global seqeuence it will need to check all array objects for matching
//values. Struggling to think of a better way though right now though!
//We could optimse this method by sorting the arrays by note_step_num, and then this will make
//it possible to only have to search through a particular range of the array, not the whole thing.

struct NoteMessageData
{
    int note_step_num; //timestamp
    int note_chan;
    int note_num;
    int note_vel;
    int note_length; //in step numbers
};

#define NUM_OF_LAYERS NUM_IMG_SECTIONS

enum SequencerLayers
{
    LAYER_MELODY = 0,
    LAYER_PAD,
    LAYER_BASS,
    LAYER_PERC
};

//FIXME: should this be a variable instead that is used to change the tempo?
#define SEQ_STEP_INTERVAL 100

#define SEQ_MAX_NUM_OF_STEPS 64
#define SEQ_MAX_NUM_OF_NOTES 512
#define SIZE_OF_NOTE_OFF_BUF 64

#define SEQ_MELODY_CHAN 1
#define SEQ_PAD_CHAN 2
#define SEQ_BASS_CHAN 3
#define SEQ_PERC_CHAN 4

#define NO_NOTE -1

class MidiGenerator : public Thread
{
    
public:
    //==============================================================================
    
    MidiGenerator();
    ~MidiGenerator();
    
    //Thread callback function
    void run();
    
    void sendMidiMessage (MidiMessage midiMessage);
    
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
    
    //virtual MIDI output device
    ScopedPointer<MidiOutput> midiOutputDevice;
    
    //variables to store the average colour values for each image section (including a whole-image average)
    float averageRed[NUM_IMG_SECTIONS + 1];
    float averageGreen[NUM_IMG_SECTIONS + 1];
    float averageBlue[NUM_IMG_SECTIONS + 1];
    float averageBrightness[NUM_IMG_SECTIONS + 1];
    float averagePerceivedBrightness[NUM_IMG_SECTIONS + 1];
    float averageHue[NUM_IMG_SECTIONS + 1];
    float averageSaturation[NUM_IMG_SECTIONS + 1];
    
    //arrays to store sequences of note data
    NoteMessageData noteSequence[NUM_OF_LAYERS][SEQ_MAX_NUM_OF_NOTES];
    
};



#endif  // MIDIGENERATOR_H_INCLUDED
