/*
  ==============================================================================

    MidiGenerator.cpp
    Created: 13 Dec 2014 2:39:09pm
    Author:  Liam Lacey

  ==============================================================================
*/

#include "MidiGenerator.h"

MidiGenerator::MidiGenerator()
            : Thread("MIDI output Generator")
{
    //Create a virtual MIDI output device
    midiOutputDevice = MidiOutput::createNewDevice("Generative MIDI App");
    
    if(midiOutputDevice)
        midiOutputDevice->startBackgroundThread();
    else
        std::cout << "Failed to create a virtual MIDI output device!" << std::endl;
    
    //default sequence arrays to hold no notes. This will need to be done everytime I new sequence is applied.
    for (int layer = 0; layer < NUM_OF_LAYERS; layer++)
    {
        for (int note = 0; note < SEQ_MAX_NUM_OF_NOTES; note++)
        {
            noteSequence[layer][note].note_step_num = NO_NOTE;
        }
    }
    
//    noteSequence[LAYER_MELODY][0].note_step_num = 10;
//    noteSequence[LAYER_MELODY][0].note_chan = 1;
//    noteSequence[LAYER_MELODY][0].note_num = 48;
//    noteSequence[LAYER_MELODY][0].note_vel = 110;
//    noteSequence[LAYER_MELODY][0].note_length = 25;
//    
//    noteSequence[LAYER_MELODY][1].note_step_num = 25;
//    noteSequence[LAYER_MELODY][1].note_chan = 1;
//    noteSequence[LAYER_MELODY][1].note_num = 50;
//    noteSequence[LAYER_MELODY][1].note_vel = 70;
//    noteSequence[LAYER_MELODY][1].note_length = 60;
}

MidiGenerator::~MidiGenerator()
{
    
}


void MidiGenerator::run()
{
    //This thread runs using Time::getMillisecondCounterHiRes() and a set time interval.
    //It uses a current step number and searches through all the note sequence arrays
    //to determine if a note needs to be turned on or off at this particular step in time.
    
    //==================================================================================
    //setup
    
    int current_time = Time::getMillisecondCounterHiRes();
    int step_num = 0;
    
    //With this array of NoteMessageData we don't use the length or vel params
    NoteMessageData note_off_buffer[SIZE_OF_NOTE_OFF_BUF];
    for (int i = 0; i < SIZE_OF_NOTE_OFF_BUF; i++)
        note_off_buffer[i].note_step_num = NO_NOTE;
    
    //==================================================================================
    //main loop
    
    while (!threadShouldExit())
    {
        //if we have reach the next 'step' in time, process the sequence
        if (Time::getMillisecondCounterHiRes() >= current_time)
        {
            //increase currentTime
             current_time += SEQ_STEP_INTERVAL;
            
            
            //==================================================================================
            //search through the note-off buffer for notes that need to be turned off on this step
            
            for (int i = 0; i < SIZE_OF_NOTE_OFF_BUF; i++)
            {
                //if the notes step number matches the current step number
                if (note_off_buffer[i].note_step_num == step_num)
                {
                    //turn off the note
                    MidiMessage midiMessage = MidiMessage::noteOff(note_off_buffer[i].note_chan, note_off_buffer[i].note_num);
                    sendMidiMessage(midiMessage);
                    
                    //'remove' this note from the note-off buffer
                    note_off_buffer[i].note_step_num = NO_NOTE;
                    
                } //if (note_off_buffer[i].note_step_num == step_num)
                
            } //for (int i = 0; i < SIZE_OF_NOTE_OFF_BUF; i++)
            
            //==================================================================================
            //search through sequence arrays for notes that need to be turned on on this step
            
            for (int i = 0; i < SEQ_MAX_NUM_OF_NOTES; i++)
            {
                for (int layer = 0; layer < NUM_OF_LAYERS; layer++)
                {
                //if this object/note has a matching step number
                if (noteSequence[layer][i].note_step_num == step_num)
                {
                    //play the note
                    MidiMessage midiMessage = MidiMessage::noteOn(noteSequence[layer][i].note_chan,
                                                                  noteSequence[layer][i].note_num,
                                                                  (uint8)noteSequence[layer][i].note_vel);
                    sendMidiMessage(midiMessage);
                    
                    //add this note to the note-off buffer
                    for (int j = 0; j < SIZE_OF_NOTE_OFF_BUF; j++)
                    {
                        //if this index of note_off_buffer is currently not being used
                        if (note_off_buffer[j].note_step_num == NO_NOTE)
                        {
                            //set the note_step_num to be the current step number + the notes length
                            //<ay need to wrap round to the beggining for step number
                            int off_step = step_num + noteSequence[layer][i].note_length;
                            
                            if (off_step >= SEQ_MAX_NUM_OF_STEPS)
                                off_step -= SEQ_MAX_NUM_OF_STEPS;
                            
                            note_off_buffer[j].note_step_num = off_step;
                            
                            note_off_buffer[j].note_chan = noteSequence[layer][i].note_chan;
                            note_off_buffer[j].note_num = noteSequence[layer][i].note_num;
                            break;
                            
                        } //if (note_off_buffer[j].note_step_num == NO_NOTE)
                        
                    } //for (int j = 0; j < SIZE_OF_NOTE_OFF_BUF; j++)
                    
                } //if (melodySequence[i].note_step_num == step_num)
                    
                } //for (int layer = 0; layer < NUM_OF_LAYERS; layer++)
                
            } //for (int i = 0; i < SEQ_MAX_NUM_OF_NOTES; i++)
            
            //==================================================================================
            std::cout << step_num << " ";
            
            //increase step_num so we process the next
            step_num++;
            
            if (step_num >= SEQ_MAX_NUM_OF_STEPS)
                step_num = 0;
            
        } //if (Time::getMillisecondCounterHiRes() >= current_time)
        
        //should I do this?
        //wait(1);
        
    } //while (!threadShouldExit())
    
    //==================================================================================
    //turn off any 'hanging' notes
    
    for (int i = 0; i < SIZE_OF_NOTE_OFF_BUF; i++)
    {
        //if this index is a note
        if (note_off_buffer[i].note_step_num != NO_NOTE)
        {
            //turn off the note
            MidiMessage midiMessage = MidiMessage::noteOff(note_off_buffer[i].note_chan, note_off_buffer[i].note_num);
            sendMidiMessage(midiMessage);
            
            //'remove' this note from the note-off buffer
            note_off_buffer[i].note_step_num = NO_NOTE;
            
        } //if (note_off_buffer[i].note_step_num == step_num)
        
    } //for (int i = 0; i < SIZE_OF_NOTE_OFF_BUF; i++)
}

void MidiGenerator::sendMidiMessage (MidiMessage midiMessage)
{
    midiOutputDevice->sendBlockOfMessages(MidiBuffer(midiMessage), Time::getMillisecondCounter(), 44100);
}


void MidiGenerator::setAverageRed (int section_num, float value)
{
    averageRed[section_num] = value;
}

void MidiGenerator::setAverageGreen (int section_num, float value)
{
    averageGreen[section_num] = value;
}

void MidiGenerator::setAverageBlue (int section_num, float value)
{
    averageBlue[section_num] = value;
}

void MidiGenerator::setAverageBrightness (int section_num, float value)
{
    averageBrightness[section_num] = value;
}

void MidiGenerator::setAveragePerceivedBrightness (int section_num, float value)
{
    averagePerceivedBrightness[section_num] = value;
}

void MidiGenerator::setAverageHue (int section_num, float value)
{
    averageHue[section_num] = value;
}

void MidiGenerator::setAverageSaturation (int section_num, float value)
{
    averageSaturation[section_num] = value;
}

float MidiGenerator::getAverageRed (int section_num)
{
    return averageRed[section_num];
}

float MidiGenerator::getAverageGreen (int section_num)
{
    return averageGreen[section_num];
}

float MidiGenerator::getAverageBlue (int section_num)
{
    return averageBlue[section_num];
}

float MidiGenerator::getAverageBrightness (int section_num)
{
    return averageBrightness[section_num];
}

float MidiGenerator::getAveragePerceivedBrightness (int section_num)
{
    return averagePerceivedBrightness[section_num];
}

float MidiGenerator::getAverageHue (int section_num)
{
    return averageHue[section_num];
}

float MidiGenerator::getAverageSaturation (int section_num)
{
    return averageSaturation[section_num];
}