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
    midiOutputDevice = MidiOutput::createNewDevice("Shuttertone App");
    
    if(midiOutputDevice)
        midiOutputDevice->startBackgroundThread();
    else
        std::cout << "Failed to create a virtual MIDI output device!" << std::endl;
}

MidiGenerator::~MidiGenerator()
{
    if (isThreadRunning())
        stopThread(500);
}

void MidiGenerator::setSequenceData()
{
    //default sequence arrays to hold no notes. This will need to be done everytime I new sequence is applied.
    for (int layer = 0; layer < NUM_OF_LAYERS; layer++)
    {
        for (int note = 0; note < SEQ_MAX_NUM_OF_NOTES; note++)
        {
            noteSequence[layer][note].note_step_num = NO_NOTE;
        }
    }
    
    //create a random number generator for various uses below
    Random randomGen(Time::currentTimeMillis());
    
    //==================================================================================
    //init varibles that determines what notes are stored into noteSequence
    
    //global
    int global_root_note;
    int global_scale_to_use;
    //we have stepInterval too, but that's a class variable
    
    //melody
    int melody_num_of_octaves; //controls note range
    int melody_main_velocity;
    int melody_velocity_offset; //dynamics - a larger offset means a larger range around the main velocity
    int melody_note_length; //in number of steps
    int melody_note_frequency;
    int melody_note_rest_freq;
    
    //pads
    int pads_chord_prog_to_use;
    int pads_note_length; //in number of steps
    int pads_density; //number of notes in chords
    int pads_main_velocity;
    int pads_chord_interval;
    
    //bass
    int bass_main_velocity;
    int bass_note_division; //staccato/legato - how many 'plucks' a note is played as.
    
    //drums/perc
    int drums_kick_pattern_to_use;
    int drums_snare_pattern_to_use;
    int drums_perc1_pattern_to_use;
    int drums_perc2_pattern_to_use;
    int drums_offbeat_value; //value to shift patterns by in steps
    int drums_main_velocity;
    int drums_velocity_offset; //dynamics - a larger offset means a larger range around the main velocity
    
    //==================================================================================
    //Apply global data values
    
    //use the hue value to get a root note
    //FIXME: what range should root note be in?
    global_root_note = scaleValue(averageHue[GLOBAL_SECTION], 0, 1.0, 36, 84);
    std::cout << "Global root note: " << global_root_note << std::endl;
    
    //use blue value to get the scale number to use
    global_scale_to_use = scaleValue(averageBlue[GLOBAL_SECTION], 0, 1.0, 0, NUM_OF_SCALES-1);
    std::cout << "Global scale: " << global_scale_to_use << std::endl;
    
    //use brightness value to set the step interval to ultimately control the tempo
    //FIXME: what range should the step interval be in?
    stepInterval = scaleValue(averageBrightness[GLOBAL_SECTION], 0, 1.0, 70, 120);
    std::cout << "Global time/step interval: " << stepInterval << std::endl;
    
    //==================================================================================
    //Apply melody data values
    
    //use the red value to get the number of octaves to use
    //FIXME: what range should I use here?
    melody_num_of_octaves = scaleValue(averageRed[IMG_SECTION_1], 0, 1.0, 1, 5);
    std::cout << "Melody number of octaves: " << melody_num_of_octaves << std::endl;
    
    //use the saturation value to get the velocity value
    //FIXME: what range should I use here?
    melody_main_velocity = scaleValue(averageSaturation[IMG_SECTION_1], 0, 1.0, 30, 127);
    std::cout << "Melody velocity: " << melody_main_velocity << std::endl;
    
    //use the brightness value to get the velocity offset for dynamic range
    //FIXME: what range should I use here?
    melody_velocity_offset = scaleValue(averageBrightness[IMG_SECTION_1], 0, 1.0, 0, 30);
    std::cout << "Melody velocity offset: " << melody_velocity_offset << std::endl;
    
    //use the blue value to get the note length
    //FIXME: what range should I use here?
    melody_note_length = scaleValue(averageBlue[IMG_SECTION_1], 0, 1.0, 1, 16);
    std::cout << "Melody note length: " << melody_note_length << std::endl;
    
    //use the hue value to get the note frequency
    //FIXME: what range should I use here?
    melody_note_frequency = scaleValue(averageHue[IMG_SECTION_1], 0, 1.0, 0, NUM_OF_NOTE_AMOUNTS-1);
    std::cout << "Melody note frequency: " << melody_note_frequency << std::endl;
    
    //use the green value to get frequency of rest in the melody
    //FIXME: what rage should I use here?
    melody_note_rest_freq = scaleValue(averageGreen[IMG_SECTION_1], 0, 1.0, 1, 16);
    std::cout << "Melody rest frequency: " << melody_note_rest_freq << std::endl;
    
    //==================================================================================
    //Apply pad data values
    
    //use the red value to get the chord progression number to use.
    pads_chord_prog_to_use = scaleValue(averageRed[IMG_SECTION_2], 0, 1.0, 0, NUM_OF_CHORD_PROGRESSIONS-1);
    std::cout << "Pad chord progression: " << pads_chord_prog_to_use << std::endl;
    
    //use the blue value to get the note length
    //FIXME: what range should I use here?
    pads_note_length = scaleValue(averageBlue[IMG_SECTION_2], 0, 1.0, 16, 64);
    std::cout << "Pad note length: " << pads_note_length << std::endl;
    
    //use the brightness value to get the number of chord note to use (density)
    //FIXME: what range should I use here?
    pads_density = scaleValue(averageBrightness[IMG_SECTION_2], 0, 1.0, 1, 4);
    std::cout << "Pad density (chord notes): " << pads_density << std::endl;
    
    //use the saturation value to get the velocity value
    //FIXME: what range should I use here?
    pads_main_velocity = scaleValue(averageSaturation[IMG_SECTION_2], 0, 1.0, 30, 127);
    std::cout << "Pad velocity: " << pads_main_velocity << std::endl;

    //use the hue value to get the chord interval value
    //FIXME: what range should I use here?
    pads_chord_interval = scaleValue(averageHue[IMG_SECTION_2], 0, 1.0, 1, 4);
    std::cout << "Pad chord interval: " << pads_chord_interval << std::endl;
    
    //==================================================================================
    //Apply bass data values
    
    //use the saturation value to get the velocity value
    //FIXME: what range should I use here?
    bass_main_velocity = scaleValue(averageSaturation[IMG_SECTION_3], 0, 1.0, 30, 127);
    std::cout << "Bass velocity: " << bass_main_velocity << std::endl;

    //use the blue value to get the bass note division number
    //FIMXE: what range should I use here?
    bass_note_division = scaleValue(averageBlue[IMG_SECTION_3], 0, 1.0, 1, 8);
    std::cout << "Bass note division: " << bass_note_division << std::endl;
    
    //==================================================================================
    //Apply drum data values
    
    //use the red value to get the kick pattern to use
    drums_kick_pattern_to_use = scaleValue(averageRed[IMG_SECTION_4], 0, 1.0, 0, NUM_OF_DRUM_PATTERNS-1);
    std::cout << "Drums kick pattern: " << drums_kick_pattern_to_use << std::endl;
    
    //use the green value to get the snare pattern to use
    drums_snare_pattern_to_use = scaleValue(averageGreen[IMG_SECTION_4], 0, 1.0, 0, NUM_OF_DRUM_PATTERNS-1);
    std::cout << "Drums snare pattern: " << drums_snare_pattern_to_use << std::endl;
    
    //use the blue value to get the perc1 pattern to use
    drums_perc1_pattern_to_use = scaleValue(averageBlue[IMG_SECTION_4], 0, 1.0, 0, NUM_OF_DRUM_PATTERNS-1);
    std::cout << "Drums perc 1 pattern: " << drums_perc1_pattern_to_use << std::endl;
    
    //use the perceived brightness value to get the perc2 pattern to use
    drums_perc2_pattern_to_use = scaleValue(averagePerceivedBrightness[IMG_SECTION_4], 0, 1.0, 0, NUM_OF_DRUM_PATTERNS-1);
    std::cout << "Drums perc 2 pattern: " << drums_perc2_pattern_to_use << std::endl;
    
    //use the hue value to get an offbeat step number
    //FIXME: what range should I use here
    drums_offbeat_value = scaleValue(averageHue[IMG_SECTION_4], 0, 1.0, 0, 16);
    std::cout << "Drums offbeat value: " << drums_offbeat_value << std::endl;
    
    //use the saturation value to get the velocity value
    //FIXME: what range should I use here?
    drums_main_velocity = scaleValue(averageSaturation[IMG_SECTION_4], 0, 1.0, 30, 127);
    std::cout << "Drums velocity: " << drums_main_velocity << std::endl;
    
    //use the brightness value to get the velocity offset for dynamic range
    //FIXME: what range should I use here?
    drums_velocity_offset = scaleValue(averageBrightness[IMG_SECTION_4], 0, 1.0, 0, 30);
    std::cout << "Drums velocity offset: " << drums_velocity_offset << std::endl;
    
    //==================================================================================
    
    int noteSeqIndex = 0;
    int chord_length = 0;
    
    //==================================================================================
    //Add notes to melody sequence
    
    //TODO: could do with getting a 'number/freq of rests' parameter??
    
    //=====================================
    //create buffer of notes to use based on root note, scale, and num of octaves.
    
    Array<int>mel_note_buffer;
    int mel_note_buffer_size = SCALE_LENGTH * melody_num_of_octaves;
    
    //populate buffer
    for (int oct = 0; oct < melody_num_of_octaves; oct++)
    {
        for (int scale_index = 0; scale_index < SCALE_LENGTH; scale_index++)
        {
            mel_note_buffer.add (global_root_note + Scales::scale[global_scale_to_use][scale_index] + (oct * 12));
        }
    }
    
    noteSeqIndex = 0;
    
    //=====================================
    //add notes to sequence array
    
    int rest_counter = 0;
    
    //iterate for loop by the set noteAmount (so it doesn't just go up in 1's and be placed on every step)
    for (int i = 0; i < SEQ_MAX_NUM_OF_STEPS; i += noteAmount[melody_note_frequency])
    {
        //if we don't want to place a 'rest' here
        //FIXME: ideally 'rest' should be placed in random values rather than in a particular order.
        //This will do for now though!
        if (rest_counter != melody_note_rest_freq)
        {
            //get a random note from note_buffer
            int note_index = randomGen.nextInt(mel_note_buffer_size-1);
            int note_num = mel_note_buffer[note_index];
            
            //generate random velocity value based on main velocity and velocity offset
            int vel = melody_main_velocity + randomGen.nextInt(melody_velocity_offset);
            if (vel > 127)
                vel = 127;
            
            //add notes to sequence
            noteSequence[LAYER_MELODY][noteSeqIndex].note_step_num = i;
            noteSequence[LAYER_MELODY][noteSeqIndex].note_chan = SEQ_MELODY_CHAN;
            noteSequence[LAYER_MELODY][noteSeqIndex].note_num = note_num;
            noteSequence[LAYER_MELODY][noteSeqIndex].note_vel = vel;
            noteSequence[LAYER_MELODY][noteSeqIndex].note_length = melody_note_length;
            noteSeqIndex++;
            rest_counter++;
        }
        else
        {
            rest_counter = 0;
        }
        
        
        
    }
    
    //==================================================================================
    //Add notes to pad sequence
    //Chord are applied in a pattern of four, depend on the chosen chord progression and chosen scale.
    //The step numbers of the four notes depends on the max number of steps.
    
    //note numbers applied here are the global root note + the index of the chosen scale relating to the values of the chosen chord progression.
    
    //TODO: note lengths?
    
    //It might be possible for high pad notes to go out of bounds of the scale.
    //Create a new temp scale here based on the set scale, but which includes a few more octaves (currently 5)
    Array<int>pad_note_buffer;
    //int pad_note_buffer_size = SCALE_LENGTH * 5;
    
    //populate buffer
    for (int oct = 0; oct < 5; oct++)
    {
        for (int scale_index = 0; scale_index < SCALE_LENGTH; scale_index++)
        {
            pad_note_buffer.add (Scales::scale[global_scale_to_use][scale_index] + (oct * 12));
            //note that this note buffer starts from 0, unlike the melody one that starts from root note
        }
    }
    
    chord_length = SEQ_MAX_NUM_OF_STEPS / CHORD_PROG_LENGTH;
    noteSeqIndex = 0; //this needs to be iterated after everytime we add a note, so we store the next note in a new index
    
    for (int div = 0; div < CHORD_PROG_LENGTH; div++)
    {
        for (int chord = 0; chord < pads_density; chord++)
        {
            //set the note to be the global root note
            int note_num = global_root_note;
            //add on the index of the note in the scale relating to current chord progression position.
            note_num += pad_note_buffer[Scales::chordProgression[pads_chord_prog_to_use][div]];
            //add on the note to the chord with the set interval (interval used as scale index, not note number).
            note_num = note_num + (pad_note_buffer[chord * pads_chord_interval]);
            
            noteSequence[LAYER_PAD][noteSeqIndex].note_step_num = div * chord_length;
            noteSequence[LAYER_PAD][noteSeqIndex].note_chan = SEQ_PAD_CHAN;
            noteSequence[LAYER_PAD][noteSeqIndex].note_num = note_num;
            noteSequence[LAYER_PAD][noteSeqIndex].note_vel = pads_main_velocity;
            noteSequence[LAYER_PAD][noteSeqIndex].note_length = chord_length; //FIXME: set this as pads_note_length
            noteSeqIndex++;
            
        }
    }
    
    //==================================================================================
    //Add notes to bass sequence
    //These are pretty much the same as pads
    
    //TODO: implement bass note division
    
    chord_length = SEQ_MAX_NUM_OF_STEPS / CHORD_PROG_LENGTH;
    noteSeqIndex = 0; //this needs to be iterated after everytime we add a note, so we store the next note in a new index
    
    for (int div = 0; div < CHORD_PROG_LENGTH; div++)
    {
        noteSequence[LAYER_BASS][noteSeqIndex].note_step_num = div * chord_length;
        noteSequence[LAYER_BASS][noteSeqIndex].note_chan = SEQ_BASS_CHAN;
        //FIXME: should I make this note a further octave lower?
        noteSequence[LAYER_BASS][noteSeqIndex].note_num = (global_root_note + Scales::scale[global_scale_to_use][Scales::chordProgression[pads_chord_prog_to_use][div]]) - 24;
        noteSequence[LAYER_BASS][noteSeqIndex].note_vel = bass_main_velocity;
        noteSequence[LAYER_BASS][noteSeqIndex].note_length = chord_length; //FIXME: why don't we have bass note length?
        noteSeqIndex++;
    }
    
    //==================================================================================
    //Add notes to drum sequence
    //Drum patterns are apply from the pallete of patterns in Scale&Patterns.h
    
    noteSeqIndex = 0; //this needs to be iterated after everytime we add a note, so we store the next note in a new index
    
    //see if we need to repeat the pattern based on the currently set sequence length
    int repeat_val = SEQ_MAX_NUM_OF_STEPS / DRUM_PATTERN_LENGTH;
    
    for (int step = 0; step < DRUM_PATTERN_LENGTH; step++)
    {
        for (int repeat = 0; repeat < repeat_val; repeat++)
        {
            //add kick pattern
            
            if (DrumPatterns::kickPattern[drums_kick_pattern_to_use][step] == 1)
            {
                //generate random velocity value based on main velocity and velocity offset
                int vel = drums_main_velocity + randomGen.nextInt(drums_velocity_offset);
                
                noteSequence[LAYER_PERC][noteSeqIndex].note_step_num = step + (repeat * DRUM_PATTERN_LENGTH);
                noteSequence[LAYER_PERC][noteSeqIndex].note_chan = SEQ_PERC_CHAN;
                noteSequence[LAYER_PERC][noteSeqIndex].note_num = NOTE_NUM_KICK;
                noteSequence[LAYER_PERC][noteSeqIndex].note_vel = vel;
                noteSequence[LAYER_PERC][noteSeqIndex].note_length = 4; //FIXME: is 4 ok from drums?
                
                noteSeqIndex++;
            }
            
            //add snare pattern
            
            if (DrumPatterns::snarePattern[drums_snare_pattern_to_use][step] == 1)
            {
                //generate random velocity value based on main velocity and velocity offset
                int vel = drums_main_velocity + randomGen.nextInt(drums_velocity_offset);
                if (vel > 127)
                    vel = 127;
                
                noteSequence[LAYER_PERC][noteSeqIndex].note_step_num = step + (repeat * DRUM_PATTERN_LENGTH);
                noteSequence[LAYER_PERC][noteSeqIndex].note_chan = SEQ_PERC_CHAN;
                noteSequence[LAYER_PERC][noteSeqIndex].note_num = NOTE_NUM_SNARE;
                noteSequence[LAYER_PERC][noteSeqIndex].note_vel = vel;
                noteSequence[LAYER_PERC][noteSeqIndex].note_length = 4; //FIXME: is 4 ok from drums?
                
                noteSeqIndex++;
            }
            
            if (DrumPatterns::perc1Pattern[drums_perc1_pattern_to_use][step] == 1)
            {
                //generate random velocity value based on main velocity and velocity offset
                int vel = drums_main_velocity + randomGen.nextInt(drums_velocity_offset);
                if (vel > 127)
                    vel = 127;
                
                //change the step number based on the offbeat value
                int step_num = step + drums_offbeat_value;
                if (step_num >= SEQ_MAX_NUM_OF_STEPS)
                    step -= SEQ_MAX_NUM_OF_STEPS;
                
                noteSequence[LAYER_PERC][noteSeqIndex].note_step_num = step_num + (repeat * DRUM_PATTERN_LENGTH);
                noteSequence[LAYER_PERC][noteSeqIndex].note_chan = SEQ_PERC_CHAN;
                noteSequence[LAYER_PERC][noteSeqIndex].note_num = NOTE_NUM_PERC1;
                noteSequence[LAYER_PERC][noteSeqIndex].note_vel = vel;
                noteSequence[LAYER_PERC][noteSeqIndex].note_length = 4; //FIXME: is 4 ok from drums?
                
                noteSeqIndex++;
            }
            
            if (DrumPatterns::perc2Pattern[drums_perc2_pattern_to_use][step] == 1)
            {
                //generate random velocity value based on main velocity and velocity offset
                int vel = drums_main_velocity + randomGen.nextInt(drums_velocity_offset);
                if (vel > 127)
                    vel = 127;
                
                //change the step number based on the offbeat value
                int step_num = step + drums_offbeat_value;
                if (step_num >= SEQ_MAX_NUM_OF_STEPS)
                    step -= SEQ_MAX_NUM_OF_STEPS;
                
                noteSequence[LAYER_PERC][noteSeqIndex].note_step_num = step_num + (repeat * DRUM_PATTERN_LENGTH);
                noteSequence[LAYER_PERC][noteSeqIndex].note_chan = SEQ_PERC_CHAN;
                noteSequence[LAYER_PERC][noteSeqIndex].note_num = NOTE_NUM_PERC2;
                noteSequence[LAYER_PERC][noteSeqIndex].note_vel = vel;
                noteSequence[LAYER_PERC][noteSeqIndex].note_length = 4; //FIXME: is 4 ok from drums?
                
                noteSeqIndex++;
            }
            
        } //for (int repeat = 0; repeat < repeat_val; repeat++)
        
    } //for (int step = 0; step < DRUM_PATTERN_LENGTH; step++)
    
    //==================================================================================
    //Sort arrays so that they are in ascending NoteMessageData.note_step_num order
    //I'm using the Exchange Sort method - http://mathbits.com/MathBits/CompSci/Arrays/Sorting.htm

    NoteMessageData temp;

    // element to be compared
    for (int i = 0; i < (SEQ_MAX_NUM_OF_NOTES - 1); i++)
    {
         // rest of the elements
        for (int j = (i + 1); j < SEQ_MAX_NUM_OF_NOTES; j++)
        {
            // sort each layer
            for (int layer = 0; layer < NUM_OF_LAYERS; layer++)
            {
                // ascending order
                if (noteSequence[layer][i].note_step_num > noteSequence[layer][j].note_step_num)
                {
                    //swap
                    temp = noteSequence[layer][i];
                    noteSequence[layer][i] = noteSequence[layer][j];
                    noteSequence[layer][j] = temp;
                    
                } //if (noteSequence[LAYER_PERC][i].note_step_num > noteSequence[LAYER_PERC][j].note_step_num)
                
            } //for (int layer = 0; layer < NUM_OF_LAYERS; layer++)
            
        } //for (int j = (i + 1); j < SEQ_MAX_NUM_OF_NOTES; j++)
        
    } //for (int i = 0; i < (SEQ_MAX_NUM_OF_NOTES - 1); i++)

    //==================================================================================
    //get the position of the first actual note in each array (first instance of non -1 (NO_NOTE))
    
    for (int layer = 0; layer < NUM_OF_LAYERS; layer++)
    {
        for (int note = 0; note < SEQ_MAX_NUM_OF_NOTES; note++)
        {
            if (noteSequence[layer][note].note_step_num != NO_NOTE)
            {
                startArrayPos[layer] = note;
                break;
            }
            
        } //for (int note = 0; note < SEQ_MAX_NUM_OF_NOTES; note++)
        
    } //for (int layer = 0; layer < NUM_OF_LAYERS; layer++)
    
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
    
    for (int layer = 0; layer < NUM_OF_LAYERS; layer++)
        currentArrayPos[layer] = startArrayPos[layer];
    
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
             current_time += stepInterval;

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
                    
                    //'remove' this note from the note-off buffer by shuffling down the notes higher in the buffer
                    for (int j = i; j < SIZE_OF_NOTE_OFF_BUF - 1; j++)
                    {
                        note_off_buffer[j] = note_off_buffer[j + 1];
                    }
                    
                    //move i back one incase the moved next note needs turning off too.
                    i--;
                    
                } //if (note_off_buffer[i].note_step_num == step_num)
                
                //if we have reached the end of actual notes in the buffer, exit the loop
                if (note_off_buffer[i].note_step_num == NO_NOTE)
                    break;
                
            } //for (int i = 0; i < SIZE_OF_NOTE_OFF_BUF; i++)
            
            
            //==================================================================================
            //search through the appropriate range of the sequence arrays for notes that need to be turned on on this step
            
            for (int layer = 0; layer < NUM_OF_LAYERS; layer++)
            {
                for (int i = currentArrayPos[layer]; i < SEQ_MAX_NUM_OF_NOTES; i++)
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
                                //may need to wrap round to the beggining for step number
                                int off_step = step_num + noteSequence[layer][i].note_length;
                                
                                if (off_step >= SEQ_MAX_NUM_OF_STEPS)
                                    off_step -= SEQ_MAX_NUM_OF_STEPS;
                                
                                note_off_buffer[j].note_step_num = off_step;
                                
                                note_off_buffer[j].note_chan = noteSequence[layer][i].note_chan;
                                note_off_buffer[j].note_num = noteSequence[layer][i].note_num;
                                break;
                                
                            } //if (note_off_buffer[j].note_step_num == NO_NOTE)
                            
                        } //for (int j = 0; j < SIZE_OF_NOTE_OFF_BUF; j++)
                        
                    }//if (melodySequence[i].note_step_num == step_num)
                    
                    //if the step number doesn't match, we have finished processing all notes for this step...
                    else
                    {
                       //...so store this step number, so for the next step we can start searching at this index in the array.
                        
                        currentArrayPos[layer] = i;
                        //std::cout << currentArrayPos[layer] << std::endl;
                        
                        //break out of searching through this layers array
                        break;
                    }
                    
                } //for (int i = currentArrayPos[layer]; i < SEQ_MAX_NUM_OF_NOTES; i++)
                
            } //for (int layer = 0; layer < NUM_OF_LAYERS; layer++)
            
            //==================================================================================
            //std::cout << step_num << " ";
            
            //increase step_num so we process the next
            step_num++;
            
            //if we have reached the last step in the sequence, reset the needed variables/
            if (step_num >= SEQ_MAX_NUM_OF_STEPS)
            {
                step_num = 0;
                
                for (int layer = 0; layer < NUM_OF_LAYERS; layer++)
                    currentArrayPos[layer] = startArrayPos[layer];
            }
            
        } //if (Time::getMillisecondCounterHiRes() >= current_time)
        
        //without adding a slight pause the CPU level is really high!
        wait(1);
        
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
    if (midiOutputDevice != nullptr)
    {
        midiOutputDevice->sendMessageNow(midiMessage);
        //midiOutputDevice->sendBlockOfMessages(MidiBuffer(midiMessage), Time::getMillisecondCounter(), 44100);
    }
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
