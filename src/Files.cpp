#include "Files.hpp"

VMidiPortListEntry::VMidiPortListEntry(String iname, String oname, bool actv)
    : inName(iname), outName(oname), active(actv)
{
}

VMidiPortListEntry::VMidiPortListEntry(nl::json jsin)
    : inName(jsin["in_name"].template get<std::string>())
    , outName(jsin["out_name"].template get<std::string>())
    , active(jsin["active"].template get<bool>())
{
}

nl::json VMidiPortListEntry::toJson()
{
    return {{"in_name", inName.toStdString()},
            {"out_name", outName.toStdString()},
            {"active", active}};
}

String VMidiPortListEntry::toJsonString()
{
    return toJson().dump();
}

StringArray VMidiMenuModel::getMenuBarNames()
{
    return {"File"};
}

PopupMenu VMidiMenuModel::getMenuForIndex(int id, const String& name)
{
    PopupMenu menu;

    menu.addItem(1, "Save to File  ");
    menu.addItem(2, "Load from File");
    menu.addItem(2, "Add from File");

    return menu;
}

void VMidiMenuModel::menuItemSelected(int menuId, int)
{
    DBG("MenuID: " << menuId);
    switch (menuId) {
    case 1:
        saveToFile();
        break;
    case 2:
        loadFromFile();
        break;
    case 3:
        addFromFile();
        break;
    }
}

void VMidiMenuModel::saveToFile()
{
    nl::json out_js;

    for (auto& p : m_mcp->tmodel().ports())
        out_js.push_back(
            VMidiPortListEntry(p->inName(), p->outName(), p->active())
                .toJson());

    FileChooser chs{"Save to File...",
                    File::getSpecialLocation(File::userHomeDirectory),
                    "*.json"};

    if (chs.browseForFileToSave(true)) {

        auto fs = chs.getResult().createOutputStream();

        fs->writeString(out_js.dump());

        fs->flush();

        delete fs;
    }
}

void VMidiMenuModel::loadFromFile()
{
    FileChooser chs{"Load from File...",
                    File::getSpecialLocation(File::userHomeDirectory),
                    "*.json"};

    if (chs.browseForFileToOpen()) {

        auto fs = chs.getResult().createInputStream();

        auto js = nl::json::parse(fs->readString().toStdString());

        delete fs;

        m_mcp->tmodel().ports().clear();

        for (auto& port : js) {
            m_mcp->tmodel().ports().push_back(std::make_unique<VMidiPort>(
                port["in_name"].get<std::string>()));

            m_mcp->tmodel()
                .ports()[m_mcp->tmodel().ports().size() - 1]
                ->setOutputName(port["out_name"].get<std::string>());
        }

        m_mcp->tmodel().update(false);
    }
}

void VMidiMenuModel::addFromFile()
{
    FileChooser chs{"Add from File...",
                    File::getSpecialLocation(File::userHomeDirectory),
                    "*.json"};

    if (chs.browseForFileToOpen()) {

        auto fs = chs.getResult().createInputStream();

        auto js = nl::json::parse(fs->readString().toStdString());

        delete fs;

        for (auto& port : js) {
            m_mcp->tmodel().ports().push_back(std::make_unique<VMidiPort>(
                port["in_name"].get<std::string>()));

            m_mcp->tmodel()
                .ports()[m_mcp->tmodel().ports().size() - 1]
                ->setOutputName(port["out_name"].get<std::string>());
        }

        m_mcp->tmodel().update(false);
    }
}
