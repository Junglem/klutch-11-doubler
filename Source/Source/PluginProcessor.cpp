
#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <cmath>

Klutch11DoublerAudioProcessor::Klutch11DoublerAudioProcessor()
    : AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      apvts(*this, nullptr, "Parameters", createParameterLayout())
{
}

juce::AudioProcessorValueTreeState::ParameterLayout
Klutch11DoublerAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"doubling", 1}, "Doubling",
        juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), 0.0f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"width", 1}, "Width",
        0.0f, 100.0f, 80.0f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"detune", 1}, "Detune",
        0.0f, 100.0f, 50.0f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"delay", 1}, "Delay",
        8.0f, 35.0f, 18.0f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"mix", 1}, "Mix",
        0.0f, 100.0f, 100.0f));

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{"output", 1}, "Output",
        -12.0f, 12.0f, 0.0f));

    return layout;
}

void Klutch11DoublerAudioProcessor::prepareToPlay(
    double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;

    // Four-second circular buffer
    int bufferLength = static_cast<int>(sampleRate * 4.0);

    delayBuffer.setSize(2, bufferLength);
    delayBuffer.clear();

    delayWritePosition = 0;
}

void Klutch11DoublerAudioProcessor::releaseResources()
{
    delayBuffer.setSize(0, 0);
}

void Klutch11DoublerAudioProcessor::processBlock(
    juce::AudioBuffer<float>& buffer,
    juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;

    const int channels = buffer.getNumChannels();
    const int samples = buffer.getNumSamples();
    const int bufferLength = delayBuffer.getNumSamples();

    if (bufferLength == 0)
        return;

    const float intensity =
        apvts.getRawParameterValue("doubling")->load() / 100.0f;

    const float width =
        apvts.getRawParameterValue("width")->load() / 100.0f;

    const float detune =
        apvts.getRawParameterValue("detune")->load() / 100.0f;

    const float delayMs =
        apvts.getRawParameterValue("delay")->load();

    const float mix =
        apvts.getRawParameterValue("mix")->load() / 100.0f;

    const float outputGain = juce::Decibels::decibelsToGain(
        apvts.getRawParameterValue("output")->load());

    const float baseDelay = delayMs *
        static_cast<float>(currentSampleRate) / 1000.0f;

    const float modulationDepth = detune *
        static_cast<float>(currentSampleRate) * 0.003f;

    auto readDelay = [&](int channel, float delaySamples)
    {
        float position = static_cast<float>(delayWritePosition)
                       - delaySamples;

        while (position < 0.0f)
            position += static_cast<float>(bufferLength);

        const int index1 = static_cast<int>(position);
        const int index2 = (index1 + 1) % bufferLength;

        const float fraction = position - static_cast<float>(index1);

        const float a = delayBuffer.getSample(channel, index1);
        const float b = delayBuffer.getSample(channel, index2);

        return a + fraction * (b - a);
    };

    for (int i = 0; i < samples; ++i)
    {
        const float left = buffer.getSample(0, i);
        const float right = channels > 1
            ? buffer.getSample(1, i) : left;

        delayBuffer.setSample(0, delayWritePosition, left);
        delayBuffer.setSample(1, delayWritePosition, right);

        const double phase = juce::MathConstants<double>::twoPi
            * static_cast<double>(delayWritePosition)
            / (currentSampleRate * 4.0);

        const float leftMod = std::sin(static_cast<float>(phase));
        const float rightMod = std::sin(
            static_cast<float>(phase + juce::MathConstants<double>::pi));

        const float leftDelay = juce::jlimit(
            1.0f, static_cast<float>(bufferLength - 2),
            baseDelay + leftMod * modulationDepth);

        const float rightDelay = juce::jlimit(
            1.0f, static_cast<float>(bufferLength - 2),
            baseDelay + rightMod * modulationDepth);

        float delayedL = readDelay(0, leftDelay);
        float delayedR = readDelay(1, rightDelay);

        // Stereo width control
        const float mid = (delayedL + delayedR) * 0.5f;
        const float side = (delayedL - delayedR) * 0.5f * width;

        delayedL = mid + side;
        delayedR = mid - side;

        // Main doubling intensity
        const float doubledL =
            (left + delayedL * intensity * 0.65f)
            / (1.0f + intensity * 0.65f);

        const float doubledR =
            (right + delayedR * intensity * 0.65f)
            / (1.0f + intensity * 0.65f);

        // Dry/wet blend
        const float outputL =
            left * (1.0f - mix) + doubledL * mix;

        const float outputR =
            right * (1.0f - mix) + doubledR * mix;

        buffer.setSample(0, i, outputL * outputGain);

        if (channels > 1)
            buffer.setSample(1, i, outputR * outputGain);

        delayWritePosition++;

        if (delayWritePosition >= bufferLength)
            delayWritePosition = 0;
    }
}

juce::AudioProcessorEditor*
Klutch11DoublerAudioProcessor::createEditor()
{
    return new Klutch11DoublerAudioProcessorEditor(*this);
}

void Klutch11DoublerAudioProcessor::getStateInformation(
    juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void Klutch11DoublerAudioProcessor::setStateInformation(
    const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml(
        getXmlFromBinary(data, sizeInBytes));

    if (xml && xml->hasTagName(apvts.state.getType()))
        apvts.replaceState(juce::ValueTree::fromXml(*xml));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Klutch11DoublerAudioProcessor();
}
