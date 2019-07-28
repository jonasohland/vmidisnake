/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a JUCE application.

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "Files.hpp"


//==============================================================================
class NewProjectApplication  : public JUCEApplication
{
public:
    //==============================================================================
    NewProjectApplication() : m_vMidiMenuModel() {}

    const String getApplicationName() override       { return ProjectInfo::projectName; }
    const String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override       { return true; }

    //==============================================================================
    void initialise (const String& commandLine) override
    {
        // This method is where you should put your application's initialisation code..
        
        MenuBarModel::setMacMainMenu(&m_vMidiMenuModel);
        mainWindow.reset (new MainWindow (getApplicationName()));
        
        
        
        auto* cmp = static_cast<MainComponent*>(mainWindow->getContentComponent());
        
        m_vMidiMenuModel.setMainComponent(cmp);
    }

    void shutdown() override
    {
        MenuBarModel::setMacMainMenu(nullptr);
        mainWindow = nullptr;
    }

    //==============================================================================
    void systemRequestedQuit() override
    {
        quit();
    }

    void anotherInstanceStarted (const String& commandLine) override
    {
    }

    //==============================================================================
    /*
        This class implements the desktop window that contains an instance of
        our MainComponent class.
    */
    class MainWindow : public DocumentWindow {
      public:
        MainWindow(String name)
            : DocumentWindow(
                  name,
                  Desktop::getInstance().getDefaultLookAndFeel().findColour(
                      ResizableWindow::backgroundColourId),
                  DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar(true);
            setContentOwned(new MainComponent(), true);

#if JUCE_IOS || JUCE_ANDROID
            setFullScreen(true);
#else
            setResizable(true, true);
            centreWithSize(getWidth(), getHeight());
#endif

            setVisible(true);
        }

        void closeButtonPressed() override
        {
            // This is called when the user tries to close this window. Here,
            // we'll just
            // ask the app to quit when this happens, but you can change this to
            // do
            // whatever you need.
            JUCEApplication::getInstance()->systemRequestedQuit();
        }

        /* Note: Be careful if you override any DocumentWindow methods - the
           base
           class uses a lot of them, so by overriding you might break its
           functionality.
           It's best to do all your work in your content component instead, but
           if
           you really have to override any DocumentWindow methods, make sure
           your
           subclass also calls the superclass's method.
        */

      private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
    };

  private:
    
    VMidiMenuModel m_vMidiMenuModel;
    std::unique_ptr<MainWindow> mainWindow;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (NewProjectApplication)
