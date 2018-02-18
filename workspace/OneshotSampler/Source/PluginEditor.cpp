/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
OneshotSamplerAudioProcessorEditor::OneshotSamplerAudioProcessorEditor (OneshotSamplerAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
    ,keyboardComponent(*p.getMidiKeyboardState(), MidiKeyboardComponent::horizontalKeyboard)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    
    // MIDIKeyboadComponentクラスのインスタンスを作成. 引数には入力するMIDIデータを保持するオブジェクトを与える
    // MIDIKeyboadComponentを子オブジェクトとして配置・表示する
    addAndMakeVisible(keyboardComponent);
    
    addAndMakeVisible(sampleSelectButton);
    sampleSelectButton.setButtonText("Sample Select");
    sampleSelectButton.addListener(this);
    
    setSize (800, 600);
}

OneshotSamplerAudioProcessorEditor::~OneshotSamplerAudioProcessorEditor()
{
}

//==============================================================================
void OneshotSamplerAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

}

void OneshotSamplerAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    sampleSelectButton.setBoundsRelative(0.6, 0.2, 0.2, 0.2);
    
    keyboardComponent.setBoundsRelative(0.0, 0.7, 1.0, 0.3);
    
}

// ボタンコンポーネントがクリックされたときのイベントハンドラ（コールバック関数）
void OneshotSamplerAudioProcessorEditor::buttonClicked(Button* button)
{
    if (button == &sampleSelectButton)
        setupSampler();
}

void OneshotSamplerAudioProcessorEditor::setupSampler()
{
    AudioFormatManager formatManager;
    
    // サポートするファイルフォーマットを登録(JUCE標準対応)
    formatManager.registerBasicFormats();
    
    FileChooser chooser("Select a audio file to play...",
                        File::nonexistent, formatManager.getWildcardForAllFormats());
    
    if (chooser.browseForFileToOpen())
    {
        File file(chooser.getResult());
        
        AudioFormatReader* reader = formatManager.createReaderFor(file);
        
        if (reader != nullptr)
        {
            processor.setupSampler(reader);
        }
    }

}
