#ifndef LOCALPLAYERFRAME_H
#define LOCALPLAYERFRAME_H
#include <gtkmm.h>
#include "eobot/const/character.hpp"
#include "eobot/character.hpp"
#include "eobot/singleton.hpp"
#include "logger.h"

class LocalPlayerFrame : public Gtk::Frame{
protected:
	Glib::RefPtr<Gtk::Builder> builder;
	
	// Labels
	Gtk::Label *nameLabel, *levelLabel, *hpLabel, *tpLabel;
	
	// Levelbars
	Gtk::LevelBar *hpBar, *tpBar;
	
	// Boxes
	Gtk::Box *mainBox, *vitalityBox, *bottomBox, *iconBox, *nameBox;
	
	// Images
	Gtk::Image *partyStatus;
	
	Character character;
	
	void CharacterChanged(ChangeType changeType);
public:
	LocalPlayerFrame(Character character);
	Character GetCharacter(){ return character; }
};


#endif