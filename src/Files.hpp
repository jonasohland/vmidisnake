#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <nlohmann/json.hpp>
#include "MainComponent.hpp"

namespace nl = nlohmann;

struct VMidiPortListEntry {
    
    VMidiPortListEntry(String, String, bool);
    VMidiPortListEntry(nl::json);
    
    String inName;
    String outName;
    bool active;
    
    nl::json toJson();
    
    String toJsonString();
};

using VMidiPortList = std::vector<VMidiPortListEntry>;

class VMidiMenuModel : public MenuBarModel {

  public:
    void setMainComponent(MainComponent* mcp)
    {
        m_mcp = mcp;
    }

    StringArray getMenuBarNames() final;
    PopupMenu getMenuForIndex(int, const String&) final;
    void menuItemSelected(int, int) final;

    void saveToFile();
    void loadFromFile();
    void addFromFile();

  private:
    MainComponent* m_mcp;
};
