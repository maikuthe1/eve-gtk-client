#ifndef PARTYBOX_H
#define PARTYBOX_H
#include <gtkmm-3.0/gtkmm.h>
#include "eobot/singleton.hpp"
#include "partyplayerframe.h"
#include <SFML/Audio.hpp>


class PartyBox : public Gtk::Box{
protected:
	// Sounds
	sf::Sound joinSound;
	sf::SoundBuffer joinSoundBuffer;
	void PartyRemove(Character* character);
	void PartyAdd(Character* character);
public:
	PartyBox();
};

#endif