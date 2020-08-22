#include "MidiCommandManager.h"

namespace app_services {


    MidiCommandManager::MidiCommandManager(tracktion_engine::Engine &e)
            : engine(e) {

        // need  to listen to midi events to pass to the midi command manager
        // to do this we need to call the addMidiInputDeviceCallback method
        // on the JUCE deviceManager (not the tracktion wrapper)
        // also we will enable the device if its disabled
        auto &juceDeviceManager = engine.getDeviceManager().deviceManager;
        auto list = juce::MidiInput::getAvailableDevices();
        for (const auto &midiDevice : list) {

            DBG("enabling juce midi device: " + midiDevice.name);
            juceDeviceManager.setMidiInputDeviceEnabled(midiDevice.identifier, true);

            DBG("adding callback for juce midi device: " + midiDevice.name);
            juceDeviceManager.addMidiInputDeviceCallback(midiDevice.identifier, this);
        }

    }

    void MidiCommandManager::handleIncomingMidiMessage(juce::MidiInput *source, const juce::MidiMessage &message) {

        (new IncomingMessageCallback(*this, message, source->getName()))->post();

    }

    void MidiCommandManager::midiMessageReceived(const juce::MidiMessage &message, const juce::String &source) {

        DBG(getMidiMessageDescription(message));

        if (message.isController()) {
            listeners.call([message](Listener &l) {
                l.controllerEventReceived(message.getControllerNumber(), message.getControllerValue());
            });

            switch (message.getControllerNumber()) {

                case 1:

                    // Encoder 1
                    if (message.getControllerValue() == 1)
                    {
                        listeners.call([](Listener &l) { l.encoder1Increased(); });
                    }


                    if (message.getControllerValue() == 127)
                        listeners.call([](Listener &l) { l.encoder1Decreased(); });

                    break;

                case 2:

                    // Encoder 2
                    if (message.getControllerValue() == 1)
                        listeners.call([](Listener &l) { l.encoder2Increased(); });

                    if (message.getControllerValue() == 127)
                        listeners.call([](Listener &l) { l.encoder2Decreased(); });

                    break;

                case 3:

                    // Encoder 3
                    if (message.getControllerValue() == 1)
                        listeners.call([](Listener &l) { l.encoder3Increased(); });

                    if (message.getControllerValue() == 127)
                        listeners.call([](Listener &l) { l.encoder3Decreased(); });

                    break;

                case 4:

                    // Encoder 4
                    if (message.getControllerValue() == 1)
                        listeners.call([](Listener &l) { l.encoder4Increased(); });

                    if (message.getControllerValue() == 127)
                        listeners.call([](Listener &l) { l.encoder4Decreased(); });

                    break;

                case 64:

                    // Encoder 1 Button
                    if (message.getControllerValue() == 127)
                        listeners.call([](Listener &l) { l.encoder1ButtonPressed(); });

                    if (message.getControllerValue() == 0)
                        listeners.call([](Listener &l) { l.encoder1ButtonReleased(); });
                    break;

                case 65:

                    // Encoder 2 Button
                    if (message.getControllerValue() == 127)
                        listeners.call([](Listener &l) { l.encoder2ButtonPressed(); });

                    if (message.getControllerValue() == 0)
                        listeners.call([](Listener &l) { l.encoder2ButtonReleased(); });
                    break;

                case 66:

                    // Encoder 3 Button
                    if (message.getControllerValue() == 127)
                        listeners.call([](Listener &l) { l.encoder3ButtonPressed(); });

                    if (message.getControllerValue() == 0)
                        listeners.call([](Listener &l) { l.encoder3ButtonReleased(); });
                    break;

                case 67:

                    // Encoder 4 Button
                    if (message.getControllerValue() == 127)
                        listeners.call([](Listener &l) { l.encoder4ButtonPressed(); });

                    if (message.getControllerValue() == 0)
                        listeners.call([](Listener &l) { l.encoder4ButtonReleased(); });

                    break;

                case 10:

                    // Tracks button
                    if (message.getControllerValue() == 127)
                        listeners.call([](Listener &l) { l.tracksButtonPressed(); });

                    if (message.getControllerValue() == 0)
                        listeners.call([](Listener &l) { l.tracksButtonReleased(); });

                    break;

                case 7:

                    // Instrument Plugins button
                    if (message.getControllerValue() == 127)
                        listeners.call([](Listener &l) { l.pluginsButtonPressed(); });

                    if (message.getControllerValue() == 0)
                        listeners.call([](Listener &l) { l.pluginsButtonReleased(); });

                    break;

                case 9:

                    // Current track plugin list button
                    if (message.getControllerValue() == 127)
                        listeners.call([](Listener &l) { l.currentTrackPluginsButtonPressed(); });

                    if (message.getControllerValue() == 0)
                        listeners.call([](Listener &l) { l.currentTrackPluginsButtonReleased(); });

                    break;

                case 5:

                    // Settings button
                    if (message.getControllerValue() == 127)
                        listeners.call([](Listener &l) { l.settingsButtonPressed(); });

                    if (message.getControllerValue() == 0)
                        listeners.call([](Listener &l) { l.settingsButtonReleased(); });

                    break;

                case 38:

                    // Record button
                    if (message.getControllerValue() == 127)
                        listeners.call([](Listener &l) { l.recordButtonPressed(); });

                    if (message.getControllerValue() == 0)
                        listeners.call([](Listener &l) { l.recordButtonReleased(); });

                    break;

                case 39:

                    // Play button
                    if (message.getControllerValue() == 127)
                        listeners.call([](Listener &l) { l.playButtonPressed(); });

                    if (message.getControllerValue() == 0)
                        listeners.call([](Listener &l) { l.playButtonReleased(); });

                    break;

                case 40:

                    // Stop Button
                    if (message.getControllerValue() == 127)
                        listeners.call([](Listener &l) { l.stopButtonPressed(); });

                    if (message.getControllerValue() == 0)
                        listeners.call([](Listener &l) { l.stopButtonReleased(); });

                    break;

                default:
                    break;
            }

        }

    }

    juce::String MidiCommandManager::getMidiMessageDescription(const juce::MidiMessage &m) {

        if (m.isNoteOn()) return "Note on " + juce::MidiMessage::getMidiNoteName(m.getNoteNumber(), true, true, 3);
        if (m.isNoteOff()) return "Note off " + juce::MidiMessage::getMidiNoteName(m.getNoteNumber(), true, true, 3);
        if (m.isProgramChange()) return "Program change " + juce::String(m.getProgramChangeNumber());
        if (m.isPitchWheel()) return "Pitch wheel " + juce::String(m.getPitchWheelValue());
        if (m.isAftertouch())
            return "After touch " + juce::MidiMessage::getMidiNoteName(m.getNoteNumber(), true, true, 3) + ": " +
                   juce::String(m.getAfterTouchValue());
        if (m.isChannelPressure()) return "Channel pressure " + juce::String(m.getChannelPressureValue());
        if (m.isAllNotesOff()) return "All notes off";
        if (m.isAllSoundOff()) return "All sound off";
        if (m.isMetaEvent()) return "Meta event";

        if (m.isController()) {
            juce::String name = "[" + juce::String(m.getControllerNumber()) + "]";

            return "Controller " + name + ": " + juce::String(m.getControllerValue());
        }

        return juce::String::toHexString(m.getRawData(), m.getRawDataSize());

    }

    void MidiCommandManager::addListener(MidiCommandManager::Listener *l) {

        listeners.add(l);

    }

    void MidiCommandManager::removeListener(MidiCommandManager::Listener *l) {

        listeners.remove(l);
    }

}