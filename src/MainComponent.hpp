/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Midi.hpp"
#include <list>

class EditableLabelComponent : public Label {
public:
    EditableLabelComponent(std::function<void(String, int, bool)>, bool input);
    
    String get();
    void set(String, bool notify = false);
    void setRow(int);

private:
    int row;
};

class ButtonComponent : public Component {
    
public:
    enum Type {
        Start = 0,
        Stop,
        Delete,
        Duplicate
    };
    
    ButtonComponent(std::function<void(Type, int)>, Type);
    void resized() final;
    void setRow(int);
    
private:
    TextButton m_button;
    int row;
};

class MidiPortsTableModel : public TableListBoxModel {
  public:
    
    MidiPortsTableModel(std::function<void(bool)>);
    
    int getNumRows() final;

    void paintRowBackground(Graphics&, int rowNumber, int width, int height,
                            bool rowIsSelected) final;

    void paintCell(Graphics& g, int rowNumber, int columnId, int width,
                   int height, bool rowIsSelected) final;

    Component* refreshComponentForCell(int row, int col, bool selected,
                                       Component* current) final;

    int getColumnAutoSizeWidth(int) final;

    std::vector<std::unique_ptr<VMidiPort>>& ports()
    {
        return m_ports;
    }

    void handleLabelChange(String, int, bool);
    void handleButtonPress(ButtonComponent::Type, int);

    void startButtonPressed(int);
    void stopButtonPressed(int);
    void deleteButtonPressed(int);
    void duplicateButtonPressed(int);
    
    void createNewPort(String, String);
    
    std::function<void(bool)> update;

  private:
    std::vector<std::unique_ptr<VMidiPort>> m_ports;
    
};

class MainComponent   : public Component
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    
    void do_create();
    
    MidiPortsTableModel& tmodel() { return m_tmodel; }

private:

    MidiPortsTableModel m_tmodel;
    TableListBox m_table;
    
    TextButton m_create_button;
    TextEditor m_new_input_name_box;
    TextEditor m_new_output_name_box;
    
    Rectangle<int> footer;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
