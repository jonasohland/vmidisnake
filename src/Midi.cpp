#include "Midi.hpp"

VMidiPort::VMidiPort(String name)
{
    m_input.reset(MidiInput::createNewDevice(name, this));
    m_output.reset(MidiOutput::createNewDevice(name));
    
    m_in_name = name;
    m_out_name = name;

    m_input->start();
    
    m_active = true;
}

VMidiPort::~VMidiPort()
{
    m_input->stop();
}

void VMidiPort::setInputName(String name)
{
    m_in_name = name;
    
    if(m_input){
        if(active())
            m_input->stop();
        
        m_input.reset(MidiInput::createNewDevice(m_in_name, this));
    }
    
}

void VMidiPort::setOutputName(String name)
{
    m_out_name = name;
    m_output.reset(MidiOutput::createNewDevice(m_out_name));
}

bool VMidiPort::active()
{
    return m_active;
}

void VMidiPort::start()
{
    if (!active()){
        
        if(!m_input)
            m_input.reset(MidiInput::createNewDevice(m_in_name, this));
        
        m_input->start();
        m_active = true;
    }
}

void VMidiPort::stop()
{
    if(active() && m_input){
        m_input->stop();
        m_active = false;
    }
}

void VMidiPort::handleIncomingMidiMessage(MidiInput* source,
                                          const MidiMessage& msg)
{
    MessageManager::callAsync([=]() { m_output->sendMessageNow(msg); });
}
