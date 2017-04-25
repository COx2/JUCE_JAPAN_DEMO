[![Github All Releases](https://img.shields.io/github/downloads/COx2/JUCE_JAPAN_DEMO/total.svg)]()  [![GitHub release](https://img.shields.io/github/release/COx2/JUCE_JAPAN_DEMO.svg)](https://github.com/COx2/JUCE_JAPAN_DEMO/releases)

## JUCE JAPAN DEMO ##
Demo projects of Juce Japan.

Project Name : JUCE JAPAN DEMO

Product Name : None

Current Version : V1.0.0

License : GPL v3

[JUCE JAPAN / Outlaw Faction](http://oufac.com/ "Outlaw Faction")

## What's the JUCE JAPAN DEMO? ##

The JUCE JAPAN DEMO is demo projects correspondig to the book "JUCE JAPAN".

## Building ##

So I've built under Windows and Mac OS X.

### Windows Build Instructions ###

1. Download Juce (http://www.juce.com/)
2. Download the VST SDK (http://www.steinberg.net/en/company/developers.html)
3. Run "The Projucer" executable included in Juce.
4. Open juce project file "xxx.jucer"
   - Make any changes to the configure and build options.
   - Save juce project if modified
5. Hit "Save Project and Open in Visual Studio". I use Visual Studio 2015.
6. Select the build: "Release - x64" and set platform to x64(64bit). Otherwise, "Release - Win32" and set platform to x86(32bit).
7. Build and deploy to plugin folder.

### Mac OS X Build Instructions ###

1. Download Juce (http://www.juce.com/)
2. Download the VST SDK (http://www.steinberg.net/en/company/developers.html)
3. Run "The Projucer" executable included in Juce.
4. Open juce project file "xxx.jucer"
   - Make any changes to the configure and build options.
   - Save juce project if modified
5. Hit "Save Project and Open in Xcode". I use Xcode 7.
6. Select the architecture x64(64bit) or x86(32bit).
7. On default setting, When build succeed and automatically deploy to plugin directory.


### Technologies Used ###
  * C++ for the language
  * JUCE for the framework/library
  * Steinberg VST SDK
  * Visual Studio for the IDE
