
#include "PluginEditor.h"

namespace
{
    const juce::Colour backgroundColour = juce::Colour(18, 18, 20);
    const juce::Colour panelColour = juce::Colour(29, 29, 33);
    const juce::Colour redColour = juce::Colour(225, 35, 48);
    const juce::Colour textColour = juce::Colour(240, 240, 240);
    const juce::Colour mutedColour = juce::Colour(145, 145, 150);
}

Klutch11DoublerAudioProcessorEditor::
Klutch11DoublerAudioProcessorEditor(
    Klutch11DoublerAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p)
{
    setSize(620, 430);

    setupKnob(doublingKnob, doublingLabel, "DOUBLING");
    setupKnob(widthKnob, widthLabel, "WIDTH");
    setupKnob(detuneKnob, detuneLabel, "DETUNE");
    setupKnob(delayKnob, delayLabel, "DELAY");
    setupKnob(mixKnob, mixLabel, "MIX");
    setupKnob(outputKnob, outputLabel, "OUTPUT");

    doublingAttachment =
        std::make_unique<SliderAttachment>(
            p.apvts, "doubling", doublingKnob);

    widthAttachment =
        std::make_unique<SliderAttachment>(
            p.apvts, "width", widthKnob);

    detuneAttachment =
        std::make_unique<SliderAttachment>(
            p.apvts, "detune", detuneKnob);

    delayAttachment =
        std::make_unique<SliderAttachment>(
            p.apvts, "delay", delayKnob);

    mixAttachment =
        std::make_unique<SliderAttachment>(
            p.apvts, "mix", mixKnob);

    outputAttachment =
        std::make_unique<SliderAttachment>(
            p.apvts, "output", outputKnob);
}

void Klutch11DoublerAudioProcessorEditor::setupKnob(
    juce::Slider& slider,
    juce::Label& label,
    const juce::String& name)
{
    addAndMakeVisible(slider);
    addAndMakeVisible(label);

    slider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(
        juce::Slider::TextBoxBelow, false, 65, 20);

    slider.setColour(
        juce::Slider::rotarySliderFillColourId, redColour);

    slider.setColour(
        juce::Slider::rotarySliderOutlineColourId,
        juce::Colour(65, 65, 70));

    slider.setColour(
        juce::Slider::thumbColourId, textColour);

    slider.setColour(
        juce::Slider::textBoxTextColourId, textColour);

    slider.setColour(
        juce::Slider::textBoxBackgroundColourId, panelColour);

    slider.setColour(
        juce::Slider::textBoxOutlineColourId,
        juce::Colours::transparentBlack);

    label.setText(name, juce::dontSendNotification);
    label.setFont(juce::Font(12.0f, juce::Font::bold));
    label.setColour(juce::Label::textColourId, mutedColour);
    label.setJustificationType(juce::Justification::centred);
}

void Klutch11DoublerAudioProcessorEditor::paint(
    juce::Graphics& g)
{
    g.fillAll(backgroundColour);

    auto area = getLocalBounds().toFloat();

    // Main panel
    g.setColour(panelColour);
    g.fillRoundedRectangle(
        area.reduced(12.0f), 14.0f);

    // Branding
    g.setColour(redColour);
    g.setFont(juce::Font(25.0f, juce::Font::bold));
    g.drawText(
        "KLUTCH 11",
        25, 22, 260, 35,
        juce::Justification::centredLeft);

    g.setColour(textColour);
    g.setFont(juce::Font(12.0f, juce::Font::bold));
    g.drawText(
        "DOUBLER",
        27, 58, 200, 20,
        juce::Justification::centredLeft);

    // Red divider
    g.setColour(redColour);
    g.fillRect(27, 88, getWidth() - 54, 2);

    // Main knob background
    g.setColour(juce::Colour(35, 35, 40));
    g.fillRoundedRectangle(
        190.0f, 105.0f, 240.0f, 220.0f, 12.0f);

    // Main knob heading
    g.setColour(textColour);
    g.setFont(juce::Font(15.0f, juce::Font::bold));
    g.drawText(
        "DOUBLING INTENSITY",
        200, 110, 220, 25,
        juce::Justification::centred);

    // Intensity scale
    g.setColour(mutedColour);
    g.setFont(juce::Font(10.0f));
    g.drawText(
        "DRY", 205, 295, 55, 16,
        juce::Justification::centredLeft);

    g.drawText(
        "NATURAL", 278, 295, 90, 16,
        juce::Justification::centred);

    g.drawText(
        "WIDE", 365, 295, 55, 16,
        juce::Justification::centredRight);

    // Footer
    g.setColour(redColour);
    g.fillRect(27, 365, getWidth() - 54, 1);

    g.setColour(mutedColour);
    g.setFont(juce::Font(10.0f));
    g.drawText(
        "STEREO VOCAL PROCESSOR  |  VERSION 1.0",
        30, 374, getWidth() - 60, 20,
        juce::Justification::centred);
}

void Klutch11DoublerAudioProcessorEditor::resized()
{
    // Main doubling control
    doublingKnob.setBounds(245, 140, 130, 145);
    doublingLabel.setBounds(245, 270, 130, 20);

    // Supporting controls
    widthKnob.setBounds(35, 130, 95, 95);
    widthLabel.setBounds(35, 215, 95, 20);

    detuneKnob.setBounds(35, 250, 95, 95);
    detuneLabel.setBounds(35, 335, 95, 20);

    delayKnob.setBounds(490, 130, 95, 95);
    delayLabel.setBounds(490, 215, 95, 20);

    mixKnob.setBounds(490, 250, 95, 95);
    mixLabel.setBounds(490, 335, 95, 20);

    outputKnob.setBounds(265, 325, 90, 75);
    outputLabel.setBounds(265, 390, 90, 18);
}
