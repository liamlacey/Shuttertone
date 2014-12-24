# Shuttertone #

Shuttertone is an open-source generative music application that uses colour data from images to generate multi-layered sequences of music that are output as a realtime stream of MIDI note messages.

You can download the application from: https://github.com/liamlacey/Shuttertone/releases. <br>
It's currently only available on Mac OS X version 10.9 and above. Windows and OS X pre-10.9 versions coming soon.

This application was originally built for [Music Hackday London 2014](https://www.hackerleague.org/hackathons/music-hack-day-london-2014/hacks/shuttertune). 

Shuttertone has been programmed using the [JUCE](http://www.rawmaterialsoftware.com/juce.php) cross-platform C++ library, and includes an Xcode project for building on Mac OS X. JUCE also easily allows you to create a Visual Studio 2010 project for building on Windows, and makefile to build for Linux.

## How to use ##

The application doesn't output any audio itself; it only outputs MIDI data, so you need to have a Digital Audio Workstation application or MIDI software (e.g. Ableton Live, Logic Pro, Cubase).

The application outputs notes on four channels, each designed for a different layer of music:
+   Channel 1 - Lead/melody
+   Channel 2 - Pads/chords
+   Channel 3 - Bass
+   Channel 4 - Drums

The application will work best if you apply relevant instruments to the corresponding channels/tracks in your DAW. For the pad track, Shuttertone may output chords/multiple notes, so it may not work if you chose an instrument that generates chords based on a single MIDI note. For the drum track, it uses the following four MIDI notes:
+   36 - Kick
+   38 - Snare
+   Percussion 1 - 37
+   Percussion 2 - 43

The above download link includes a demo Ableton Live 9 project to get you started.

## Future plans ##

There are many things on my to-do list for this application:
+   Build a version for Windows
+   Tinker with the mappings of colour data to music parameters to make it more meaningful
+   Add more mappings
+   Tinker with the musical parameter ranges to widen the variety of output
+   Improve the UI so that you can edit MIDI channels, MIDI notes for drum sequence, mappings, image division etc...
+   Add more note scales and chord progressions
+   Allow the sequences to change and evolve over time
+   Allow slideshows of images to be added
+   Add video analysis
+   Use the application as a starting point for creating a C++ library for creating generative music applications

## Technical details ##

The application works in the following way: <br>
1. Each pixel of the chosen image is analysed in terms of its red, green, blue, brightness, perceived brightness, saturation, and hue values. <br>
2. The image is split into four horizontal strips, and the average value of each of the above types of data is worked out, as well as an average of each value for the entire image. <br>
3. A number of these average values are then mapped to a set of musical parameters (see below). Each of the four sections of the image relates to one of the four layers of the generated sequence, with the global values mapped to global sequence parameters. <br>
4. These musical parameter values are used to create a particular sequence of notes. The sequence of notes does not change until you load a different image into the application. <br>

Here is the currently mapping of data:
+   Global - Root note - hue
+   Global - Note scale - blue
+   Global - Tempo - brightness
+   Melody - Number of octaves - red
+   Melody - Average velocity - saturation
+   Melody - Velocity dynamics - brightness
+   Melody - Note length - blue
+   Melody - Note division/frequency of notes - hue
+   Melody - Frequency of rests - green
+   Pads - Chord progression - red
+   Pads - Chord density/number of notes - brightness
+   Pads - Velocity - saturation
+   Pads - Interval of chord notes - hue
+   Bass - Velocity - saturation
+   Drums - Kick pattern - red
+   Drums - Snare pattern - green
+   Drums - Percussion 1 pattern - blue
+   Drums - Percussion 2 pattern - perceived brightness
+   Drums - persussion offset - hue
+   Drums - Average velocity - saturation
+   Drums - Velocity dynamics - brightness

In regards to the sections of the image and their mappings to the layers of music, the top horizontal section is for the melody, the middle-top section for the pads, the middle bottom section for the bass, and the bottom section for the drums.

## License ##

Shuttertone is licensed under the GPLv2. LICENSE.txt for details.

The following libraries are included in the source code:
+  The [JUCE](http://www.rawmaterialsoftware.com/juce.php) library which is licensed under the GPLv2.
