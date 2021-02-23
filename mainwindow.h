#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <gtkmm.h>
#include <SFML/Audio.hpp>
#include "loginwindow.h"
#include "chatbox.h"
#include "statusframe.h"
#include "localplayerframe.h"
#include "preferencespopover.h"
#include "onlineplayerbox.h"
#include "draggablebox.h"
#include "partyinvitebox.h"
#include "partybox.h"
#include "inventoryscrolledwindow.h"
#include "menubarbutton.h"
#include "messagebox.h"
#include "logger.h"
#include <fstream>"
#include "paperdollbox.h"

class LocalPlayerEventBox : public Gtk::EventBox{
protected:
	LocalPlayerFrame *localPlayerFrame;
public:
	LocalPlayerEventBox(){};

	LocalPlayerFrame* GetLocalPlayerFrame(){
		return localPlayerFrame;
	}

	void SetLocalPlayerFrame(LocalPlayerFrame* frame){
		localPlayerFrame = frame;
	}
};

class MainWindow : public Gtk::Window{
protected:
	Glib::RefPtr<Gtk::Builder> builder;

	// ScrolledWindows
	Gtk::ScrolledWindow *mapScroll, *playersScrolledWindow, *minimapScrolledWindow;
	InventoryScrolledWindow *inventoryScrolledWindow;

	// Buttons
	Gtk::Button* preferencesButton, *logoutButton;
	MenuBarButton *inventoryButton, *spellsToggleButton, *characterToggleButton, *chatToggleButton, *playersToggleButton, *mapToggleButton;

	// Revealers

	// Labels
	Gtk::Label *titleLabel;

	// Frames
	StatusFrame* statusFrame;

	// Popovers
	PreferencesPopover *preferencesPopover;

	// Adjustments
	Glib::RefPtr<Gtk::Adjustment> scrollVAdjustment;
	Glib::RefPtr<Gtk::Adjustment> scrollHAdjustment;

	// Alignments
	Gtk::Alignment* mapAlignment;

	// Expanders
	Gtk::Expander *localExpander, *partyExpander, *onlineExpander;

	// Boxes
	ChatBox *chatBox;
	Gtk::Box *localBox, *onlineBox, *logBox, *characterBox;
	PartyBox *partyBox;

	// Menus
	Gtk::Menu *playerRightClickMenu;

	// Draggables
	DraggableBox *chatWindow, *playersWindow, *partyWindow, *mapWindow, *logWindow, *inventoryWindow, *characterWindow;

	// Overlays
	Gtk::Overlay *mainOverlay;

	// Windows
	LoginWindow* loginWindow;

	// Images
	Gtk::Image *mapImage;

	// Grids
	Gtk::Grid* mapGrid, *characterGrid;

	// Settings
	Glib::RefPtr<Gio::Settings> gioSettings, gioPrefs;
	Glib::RefPtr<Gtk::Settings> gtkSettings;

	// GL
	Gtk::GLArea *glArea;

	// Sounds
	sf::Sound adminSound;
	sf::SoundBuffer adminSoundBuffer;

	Character selectedCharacter;

	bool mouseDown = false;
	int previousY = 0;
	int previousX = 0;
	int previousVPosition = 0;
	int previousHPosition = 0;

	bool MousePressed(GdkEventButton* event);
	bool MouseReleased(GdkEventButton* event);
	bool PlayerClicked(GdkEventButton* event, LocalPlayerFrame* playerFrame);
	void InviteJoin(unsigned char type);
	bool MouseMoved(GdkEventMotion* event);
	void PlayerAppear(Character character);
	void PlayerConnect(Character character);
	void PlayerRemove(Character character);
	void PlayerDisconnect(Character character);
	void PreferencesButtonClicked();
	void StateChanged(EOClient::State newState);
	void LogoutButtonClicked();
	void ToggleButtonClicked(Gtk::Widget* widget);
	void PartyInvite(unsigned char type, unsigned short id, std::string name);
	void PartyCreate();
	void PartyClose();
	void MessageBoxOpened(std::string title, std::string message);

public:
	MainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade);

};
#endif