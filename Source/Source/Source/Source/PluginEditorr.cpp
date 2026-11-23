
#include "PluginEditor.h"

Klutch11DoublerAudioProcessorEditor::
Klutch11DoublerAudioProcessorEditor(
    Klutch11DoublerAudioProcessor& processor)
    : AudioProcessorEditor(&processor),
      audioProcessor(processor)
{
    setSize(760, 500);

    setupKnob(doublingKnob, doublingLabel, "DOUBLING");
    setupKnob(widthKnob, widthLabel, "WIDTH");
    setupKnob(detuneKnob, detuneLabel, "DETUNE");
    setupKnob(delayKnob, delayLabel, "DELAY");
    setupKnob(mixKnob, mixLabel, "MIX");
    setupKnob(outputKnob, outputLabel, "OUTPUT");

    doublingAttachment = std::make_unique<SliderAttachment>(
        audioProcessor.apvts, "doubling", doublingKnob);

    widthAttachment = std::make_unique<SliderAttachment>(
        audioProcessor.apvts, "width", widthKnob);

    detuneAttachment = std::make_unique<SliderAttachment>(
        audioProcessor.apvts, "detune", detuneKnob);

    delayAttachment = std::make_unique<SliderAttachment>(
        audioProcessor.apvts, "delay", delayKnob);

    mixAttachment = std::make_unique<SliderAttachment>(
        audioProcessor.apvts, "mix", mixKnob);

    outputAttachment = std::make_unique<SliderAttachment>(
        audioProcessor.apvts, "output", outputKnob);
}

void Klutch11DoublerAudioProcessorEditor::setupKnob(
    juce::Slider& slider,
    juce::Label& label,
    const juce::String& name)
{
    slider.setSliderStyle(
        juce::Slider::RotaryHorizontalVerticalDrag);

    slider.setTextBoxStyle(
        juce::Slider::TextBoxBelow, false, 65, 20);

    slider.setColour(
        juce::Slider::rotarySliderFillColourId,
        juce::Colour(0xffe50924));

    slider.setColour(
        juce::Slider::rotarySliderOutlineColourId,
        juce::Colour(0xff444444));

    slider.setColour(
        juce::Slider::thumbColourId,
        juce::Colour(0xffff263f));

    slider.setColour(
        juce::Slider::textBoxTextColourId,
        juce::Colours::white);

    slider.setColour(
        juce::Slider::textBoxBackgroundColourId,
        juce::Colour(0xff171717));

    slider.setColour(
        juce::Slider::textBoxOutlineColourId,
        juce::Colour(0xff333333));

    label.setText(name, juce::dontSendNotification);
    label.setJustificationType(juce::Justification::centred);
    label.setColour(
        juce::Label::textColourId,
        juce::Colours::white);

    label.setFont(juce::Font(13.0f, juce::Font::bold));

    addAndMakeVisible(slider);
    addAndMakeVisible(label);
}

void Klutch11DoublerAudioProcessorEditor::paint(
    juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xff090909));

    g.setColour(juce::Colour(0xffe50924));
    g.fillRect(0, 0, getWidth(), 4);

    g.setColour(juce::Colours::white);
    g.setFont(juce::Font(27.0f, juce::Font::bold));
    g.drawText(
        "KLUTCH 11",
        25, 17, 300, 38,
        juce::Justification::centredLeft);

    g.setColour(juce::Colour(0xffe50924));
    g.setFont(juce::Font(15.0f, juce::Font::bold));
    g.drawText(
        "DOUBLER",
        27, 51, 200, 25,
        juce::Justification::centredLeft);

    g.setColour(juce::Colour(0xff777777));
    g.setFont(juce::Font(11.0f));
    g.drawText(
        "VOCAL THICKENER  /  STEREO WIDENER",
        420, 28, 310, 25,
        juce::Justification::centredRight);

    g.setColour(juce::Colour(0xff292929));
    g.drawLine(25.0f, 85.0f,
               735.0f, 85.0f, 1.0f);

    g.setColour(juce::Colour(0xff171717));
    g.fillRoundedRectangle(
        260.0f, 105.0f, 240.0f, 225.0f,
        14.0f);

    g.setColour(juce::Colour(0xffe50924));
    g.drawRoundedRectangle(
        260.0f, 105.0f, 240.0f, 225.0f,
        14.0f, 1.2f);

    g.setColour(juce::Colour(0xff777777));
    g.setFont(juce::Font(10.0f));
    g.drawText(
        "KLUTCH 11 AUDIO",
        0, 465, getWidth(), 20,
        juce::Justification::centred);
}

void Klutch11DoublerAudioProcessorEditor::resized()
{
    doublingLabel.setBounds(310, 115, 140, 25);
    doublingKnob.setBounds(285, 140, 190, 175);

    const int knobWidth = 110;
    const int knobHeight = 115;
    const int startX = 20;
    const int gap = 13;
    const int y = 350;

    juce::Slider* knobs[] = {
        &widthKnob,
        &detuneKnob,
        &delayKnob,
        &mixKnob,
        &outputKnob
    };

    juce::Label* labels[] = {
        &widthLabel,
        &detuneLabel,
        &delayLabel,
        &mixLabel,
        &outputLabel
    };

    for (int i = 0; i < 5; ++i)
    {
        const int x = startX + i * (knobWidth + gap);

        labels[i]->setBounds(x, y - 25, knobWidth, 22);
        knobs[i]->setBounds(x, y, knobWidth, knobHeight);
    }
}
