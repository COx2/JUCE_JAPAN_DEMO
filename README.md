[![Github All Releases](https://img.shields.io/github/downloads/COx2/JUCE_JAPAN_DEMO/total.svg)]()  [![GitHub release](https://img.shields.io/github/release/COx2/JUCE_JAPAN_DEMO.svg)](https://github.com/COx2/JUCE_JAPAN_DEMO/releases)

## JUCE JAPAN DEMO ##
Demo projects for Juce Japan books.

Project Name : JUCE JAPAN DEMO

License : GPL v3


## What's the JUCE JAPAN DEMO? ##

The JUCE JAPAN DEMO is demo projects regarding to the book named "JUCE JAPAN".

JUCE JAPAN is a textbook for learning audio plugin development with JUCE.

[JUCE JAPAN / Outlaw Faction](http://oufac.com/ "Outlaw Faction")


## Building ##

I have built under Windows and Mac OS X.

### Windows Build Instructions ###

1. Clone this repository.
2. Open a directory with the name regarding to the book numbering.
3. Run "Projucer.exe" executable included in "Juce" directory.
4. Open juce project file "xxx.jucer"
   - Make any changes to the configure and build options.
   - Save juce project if modified
5. Hit "Save Project"
6. Open Visual Studio solution file named "xxx.sln" in your Visual Studio.
7. Select the build settings: "Release - x64" and set platform to x64(64bit). Otherwise, "Release - Win32" and set platform to x86(32bit).
8. Build and deploy to plugin folder.

### Mac OS X Build Instructions ###

1. Clone this repository.
2. Open a directory with the name regarding to the book numbering.
3. Run "Projucer.app" executable included in "Juce" directory.
4. Open juce project file "xxx.jucer"
   - Make any changes to the configure and build options.
   - Save juce project if modified
5. Hit "Save Project"
6. Open Xcode project file named "xxx.xcodeproj" in your Xcode.
7. Select the architecture x64(64bit) or x86(32bit).
8. On default setting, When build succeed and automatically deploy to plugin directory.


### Technologies Used ###
  * C++ for the language
  * JUCE for the framework/library
  * Steinberg VST SDK
  * Visual Studio for the IDE
