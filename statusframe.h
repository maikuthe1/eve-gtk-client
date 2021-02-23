#ifndef STATUSFRAME_H
#define STATUSFRAME_H
#include <gtkmm.h>
#include "eobot/const/character.hpp"
#include "eobot/singleton.hpp"
#include "logger.h"

class StatusFrame : public Gtk::Frame{
protected:
	Glib::RefPtr<Gtk::Builder> builder;
	
	// Labels
	Gtk::Label *nameLabel, *levelLabel;
	
	// Levelbars
	Gtk::LevelBar *hpBar, *tpBar;
	
	void CharacterChanged(ChangeType changeType);
	
public:
	StatusFrame(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade);
};


#endif