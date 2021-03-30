#ifndef CHATTAB_H
#define CHATTAB_H
#include <gtkmm.h>
#include <SFML/Audio.hpp>
#include "chatscrolledwindow.h"

class ChatTab : public Gtk::Box
{
public:
	ChatTab(Glib::ustring title) : Gtk::Box(Gtk::Orientation::ORIENTATION_HORIZONTAL, 3){
		// Title
		label = Gtk::manage(new Gtk::Label(title));

		// Buttons
		buttonBox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL));

		// Notification button
		notificationButton = Gtk::manage(new Gtk::Button());
		notificationButton->set_image_from_icon_name("notifications-disabled-symbolic");
		Glib::RefPtr<Gtk::StyleContext> context = notificationButton->get_style_context();
		context->add_class("flat");
		context->add_class("closeButton");
		buttonBox->pack_start(*notificationButton, false, false);
		// Close button
		closeButton = Gtk::manage(new Gtk::Button("X"));
		//closeButton->set_image_from_icon_name("edit-delete-symbolic");
		Glib::RefPtr<Gtk::StyleContext> context2 = closeButton->get_style_context();
		context2->add_class("flat");
		context2->add_class("closeButton");
		buttonBox->pack_start(*closeButton, false, false);


		pack_start(*label);
		pack_end(*buttonBox);

		show_all();
	}
	Gtk::Button* GetCloseButton(){
		return closeButton;
	}
	ChatScrolledWindow* GetChild(){
		return child;
	}
	Gtk::Box* GetBoxChild(){
		return boxChild;
	}
	void SetChild(ChatScrolledWindow* newChild){
		child = newChild;
	}
	void SetBoxChild(Gtk::Box* newBoxChild){
		boxChild = newBoxChild;
	}
	sf::Sound GetSound(){ return sound; }
	void SetSound(sf::Sound newSound){ sound = newSound; }
	sf::SoundBuffer GetSoundBuffer(){ return buffer; }
	void SetSoundBuffer(sf::SoundBuffer newBuffer){ buffer = newBuffer; sound.setBuffer(buffer); }

protected:
	Gtk::Label* label;
	Gtk::Button* closeButton, *notificationButton;
	ChatScrolledWindow* child;
	Gtk::Box* boxChild, *buttonBox;
	sf::Sound sound;
	sf::SoundBuffer buffer;

};

#endif