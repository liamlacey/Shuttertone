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
}

MidiGenerator::~MidiGenerator()
{
    
}


void MidiGenerator::run()
{
    while (!threadShouldExit())
    {
        wait(1);
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