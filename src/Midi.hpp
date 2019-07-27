#pragma once

#include "../JuceLibraryCode/JuceHeader.h"


class VMidiPort : public MidiInputCallback {
public:
    
    VMidiPort(String name);
    ~VMidiPort();
    
    void setInputName(String);
    void setOutputName(String);
    
    String inName() { return m_in_name; }
    String outName() { return m_out_name; }
    
    bool active();
    void start();
    void stop();
    
    void handleIncomingMidiMessage(MidiInput* source, const MidiMessage&) final;
    
private:
    
    bool m_active = false;
    
    String m_in_name;
    String m_out_name;
    
    std::unique_ptr<MidiInput> m_input;
    std::unique_ptr<MidiOutput> m_output;
    
};
