
#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class Klutch11DoublerAudioProcessorEditor
    : public juce::AudioProcessorEditor
{
public:
    explicit Klutch11DoublerAudioProcessorEditor(
        Klutch11DoublerAudioProcessor&);
    ~Klutch11DoublerAudioProcessorEditor() override = default;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    using SliderAttachment =
        juce::AudioProcessorValueTreeState::SliderAttachment;

    Klutch11DoublerAudioProcessor& audioProcessor;

    juce::Slider doublingKnob;
    juce::Slider widthKnob;
    juce::Slider detuneKnob;
    juce::Slider delayKnob;
    juce::Slider mixKnob;
    juce::Slider outputKnob;

    juce::Label doublingLabel;
    juce::Label widthLabel;
    juce::Label detuneLabel;
    juce::Label delayLabel;
    juce::Label mixLabel;
    juce::Label outputLabel;

    std::unique_ptr<SliderAttachment> doublingAttachment;
    std::unique_ptr<SliderAttachment> widthAttachment;
    std::unique_ptr<SliderAttachment> detuneAttachment;
    std::unique_ptr<SliderAttachment> delayAttachment;
    std::unique_ptr<SliderAttachment> mixAttachment;
    std::unique_ptr<SliderAttachment> outputAttachment;

    void setupKnob(juce::Slider&, juce::Label&,
                   const juce::String&);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(
        Klutch11DoublerAudioProcessorEditor
    )
};
