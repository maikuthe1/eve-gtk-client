#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H
#include <gtkmm.h>
#include "eobot/singleton.hpp"
#include "characterselectpreviewbox.h"
//#include "mainwindow.h"

class LoginWindow : public Gtk::Window{
protected:
	Glib::RefPtr<Gtk::Builder> builder;
	
	// Buttons
	Gtk::Button* loginButton;
	
	// Entries
	Gtk::Entry *accountNameEntry, *passwordEntry, *IPEntry, *portEntry;
	
	// Check Buttons
	Gtk::CheckButton* rememberCheckButton;
	
	// Stacks
	Gtk::Stack* loginStack;
	
	// Boxes
	Gtk::Box* characterSelectionBox, *loginBox;
	
	// Settings
	Glib::RefPtr<Gio::Settings> gioSettings;
	Glib::RefPtr<Gtk::Settings> gtkSettings;
	
	// Bindings
	Glib::RefPtr<Glib::Binding> IPBind;
	Glib::RefPtr<Glib::Binding> portBind;
	Glib::RefPtr<Glib::Binding> rememberBind;
	Glib::RefPtr<Glib::Binding> accountBind;
	Glib::RefPtr<Glib::Binding> passwordBind;
	
	void RememberCheckButtonToggled();
	void LoginButtonClicked();
	void ConnectionReply(InitReply reply, Glib::ustring reason);
	void LoginResult(LoginReply reply, Glib::ustring message);
	void StateChanged(EOClient::State newState);

public:
	LoginWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade);
};
#endif