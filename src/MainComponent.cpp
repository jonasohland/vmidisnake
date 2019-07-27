/*
| index | in name | out name | start | stop | delete | duplicate |
*/
#include "MainComponent.hpp"

EditableLabelComponent::EditableLabelComponent(
    std::function<void(String, int, bool)> handler, bool is_input)
{
    setEditable(false, true, false);
    onTextChange = [this, handler, is_input] { handler(get(), row, is_input); };
}

String EditableLabelComponent::get()
{
    return getText();
}

void EditableLabelComponent::set(String name, bool notify)
{
    setText(name, (notify) ? NotificationType::sendNotificationAsync
                           : NotificationType::dontSendNotification);
}

void EditableLabelComponent::setRow(int new_row)
{
    row = new_row;
}

ButtonComponent::ButtonComponent(std::function<void(Type, int)> handler,
                                 Type ty)
{
    switch (ty) {
    case Type::Start:
        m_button.setButtonText("Start");
        break;
    case Type::Stop:
        m_button.setButtonText("Stop");
        break;
    case Type::Delete:
        m_button.setButtonText("Delete");
        break;
    case Type::Duplicate:
        m_button.setButtonText("Duplicate");
        break;
    }

    addAndMakeVisible(&m_button);
    m_button.onClick = [this, ty, handler] { handler(ty, row); };
}

void ButtonComponent::resized()
{
    m_button.setBoundsInset(BorderSize<int>(2));
}

void ButtonComponent::setRow(int new_row)
{
    row = new_row;
}

MidiPortsTableModel::MidiPortsTableModel(std::function<void(bool)> update_fn)
    : update(update_fn)
{
}

int MidiPortsTableModel::getNumRows()
{
    return m_ports.size();
}

void MidiPortsTableModel::paintRowBackground(Graphics& g, int rowNumber,
                                             int width, int height,
                                             bool rowIsSelected)
{
    auto alternateColour =
        LookAndFeel::getDefaultLookAndFeel()
            .findColour(ListBox::backgroundColourId)
            .interpolatedWith(LookAndFeel::getDefaultLookAndFeel().findColour(
                                  ListBox::textColourId),
                              0.03f);
    if (rowNumber % 2)
        g.fillAll(alternateColour);
}

void MidiPortsTableModel::paintCell(Graphics& g, int rowNumber, int columnId,
                                    int width, int height, bool rowIsSelected)
{
    if (columnId == 1) {

        g.setColour(LookAndFeel::getDefaultLookAndFeel().findColour(
            ListBox::textColourId));

        if (ports()[rowNumber]->active()) {
        
            g.setColour(Colours::greenyellow);
            
            g.fillRect(0, 0, width, height);


            g.setColour(LookAndFeel::getDefaultLookAndFeel().findColour(
                ListBox::backgroundColourId));

            g.drawText(std::to_string(rowNumber), 0, 0, width, height,
                       Justification::centred);        }
        else {

            g.setColour(LookAndFeel::getDefaultLookAndFeel().findColour(
                ListBox::textColourId));

            g.drawText(std::to_string(rowNumber), 0, 0, width, height,
                       Justification::centred);
        }
    }
    
    g.setColour (LookAndFeel::getDefaultLookAndFeel().findColour (ListBox::backgroundColourId));
    g.fillRect (width - 1, 0, 1, height);
}

Component* MidiPortsTableModel::refreshComponentForCell(int row, int col,
                                                        bool selected,
                                                        Component* current)
{
    if(col == 2 || col == 3){
        
        auto* label_c  = static_cast<EditableLabelComponent*>(current);

        if (!label_c)
            label_c = new EditableLabelComponent(
                std::bind(&MidiPortsTableModel::handleLabelChange, this,
                          std::placeholders::_1, std::placeholders::_2,
                          std::placeholders::_3), col == 2);

        label_c->set((col == 2) ? ports()[row]->inName()
                                : ports()[row]->outName());

        label_c->setRow(row);

        return label_c;
        
    }
    else if (col > 3) {
        
        auto* button_c = static_cast<ButtonComponent*>(current);

        if (!button_c)
            button_c = new ButtonComponent(
                std::bind(&MidiPortsTableModel::handleButtonPress, this,
                          std::placeholders::_1, std::placeholders::_2),
                static_cast<ButtonComponent::Type>(col - 4));
        
        button_c->setRow(row);

        return button_c;
    } else
        return nullptr;
}

int MidiPortsTableModel::getColumnAutoSizeWidth(int col)
{
    return 0;
}

void MidiPortsTableModel::handleButtonPress(ButtonComponent::Type ty, int row)
{
    DBG("Button pressed: r" << row << " t" << static_cast<int>(ty));
    
    switch (ty) {
        case ButtonComponent::Type::Start:
            startButtonPressed(row);
            break;
        case ButtonComponent::Type::Stop:
            stopButtonPressed(row);
            break;
        case ButtonComponent::Type::Delete:
            deleteButtonPressed(row);
            break;
        case ButtonComponent::Type::Duplicate:
            duplicateButtonPressed(row);
            break;
    }
}

void MidiPortsTableModel::handleLabelChange(String label, int row, bool input)
{
    if(input)
        m_ports[row]->setInputName(label);
    else
        m_ports[row]->setOutputName(label);
}

void MidiPortsTableModel::startButtonPressed(int row)
{
    m_ports[row]->start();
    update(true);
}

void MidiPortsTableModel::stopButtonPressed(int row)
{
    m_ports[row]->stop();
    update(true);
}

void MidiPortsTableModel::deleteButtonPressed(int row)
{
    m_ports[row]->stop();
    m_ports.erase(m_ports.begin() + row);
    
    update(false);
}

void MidiPortsTableModel::duplicateButtonPressed(int row)
{

    m_ports.insert(m_ports.begin() + row + 1,
                   std::make_unique<VMidiPort>(m_ports[row]->inName()));
    
    m_ports[row + 1]->setOutputName(m_ports[row]->outName());
    
    update(false);
}

void MidiPortsTableModel::createNewPort(String inputName, String outputName)
{
    m_ports.push_back(std::make_unique<VMidiPort>(inputName));
    
    m_ports[m_ports.size() - 1]->setOutputName(outputName);
    
    update(false);
}

//==============================================================================
MainComponent::MainComponent()
    : m_tmodel([this](bool repaint) {
        m_table.updateContent();
        if (repaint) m_table.repaint();
    })
{

    auto header = new TableHeaderComponent();

    // clang-format off

    header->addColumn("ID", 1, 30, 30, 30,
        TableHeaderComponent::resizable + TableHeaderComponent::visible);
    header->addColumn("Input Name", 2, 100, 50, -1,
        TableHeaderComponent::resizable + TableHeaderComponent::visible);
    header->addColumn("Output Name", 3, 100, 50, -1,
        TableHeaderComponent::resizable + TableHeaderComponent::visible);
    
    header->addColumn("Start", 4, 45, 45, 45,
        TableHeaderComponent::visible);
    header->addColumn("Stop", 5, 45, 45, 45,
        TableHeaderComponent::visible);
    header->addColumn("Delete", 6, 60, 60, 60,
        TableHeaderComponent::visible);
    header->addColumn("Duplicate", 7, 60, 60, 60,
        TableHeaderComponent::visible);

    // clang-format on

    header->setStretchToFitActive(true);

    m_table.setHeader(header);

    m_table.setModel(&m_tmodel);

    addAndMakeVisible(&m_table);
    
    m_create_button.setButtonText("create");

    m_create_button.onClick = std::bind(&MainComponent::do_create, this);

    m_new_input_name_box.setMultiLine(false);
    m_new_output_name_box.setMultiLine(false);
    
    addAndMakeVisible(&m_create_button);
    addAndMakeVisible(&m_new_input_name_box);
    addAndMakeVisible(&m_new_output_name_box);

    setSize(600, 400);
}

MainComponent::~MainComponent()
{
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    int footer_h = 40;
    int footer_h_padding = 7;
    int footer_w_padding = 7;

    footer = getBounds();

    footer.setWidth(footer.getWidth() - footer_w_padding * 2);
    footer.setX(footer.getX() + footer_w_padding);

    footer.setHeight(footer_h - footer_h_padding * 2);
    footer.setY(getBounds().getHeight() - footer_h + footer_h_padding);

    // clang-format off
    
    m_new_input_name_box.setBounds(footer.getX(),
                                   footer.getY(),
                                   footer.getWidth() * 0.3,
                                   footer.getHeight());

    m_new_output_name_box.setBounds(
        footer.getX()
            + m_new_input_name_box.getWidth()
            + footer.getWidth() * 0.05,
        footer.getY(),
        footer.getWidth() * 0.3,
        footer.getHeight());

    m_create_button.setBounds(
        m_new_output_name_box.getX()
            + m_new_output_name_box.getWidth()
            + footer.getWidth() * 0.05,
        footer.getY(),
        footer.getWidth() * 0.3,
        footer.getHeight());

    m_table.setBounds(
        0, 0, getBounds().getWidth(),
        getBounds().getHeight() - footer_h);

    // clang-format on
    
    repaint();
}


void MainComponent::do_create()
{
    m_tmodel.createNewPort(
        m_new_input_name_box.getText(), m_new_output_name_box.getText());
}
