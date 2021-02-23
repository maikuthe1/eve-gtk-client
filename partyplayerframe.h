#ifndef PARTYPLAYERBOX_H
#define PARTYPLAYERBOX_H
#include <gtkmm-3.0/gtkmm.h>
#include "eobot/singleton.hpp"

class PartyPlayerFrame : public Gtk::Frame{
protected:
	Gtk::Image* leaderIcon;
	Gtk::Box* nameBox, *hpBox;
	Gtk::Label* nameLabel, *lvlLabel, *hpLabel, *kickLabel;
	Gtk::LevelBar *hpLevelBar;
	Gtk::EventBox *kickEventBox;
	
	Character* m_character;
	
	void CharacterChanged(ChangeType changeType){
		if(changeType == ChangeType::HP){
			hpLevelBar->set_value((int)m_character->GetHPPercent());
		}
		if(changeType == ChangeType::Level){
			lvlLabel = Gtk::manage(new Gtk::Label(Glib::ustring::compose("Lvl. %1", m_character->GetLevel())));
		}
	}
	
	bool Remove(GdkEventButton* event, unsigned short id){
		S &s = S::GetInstance();
		s.eoclient.PartyRemove(id);
		return false;
	}
public:
	PartyPlayerFrame(Character* character){
		S &s = S::GetInstance();
		m_character = character;
		
		nameBox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 3));
		
		if(m_character->GetPartyLeader()){
			leaderIcon = Gtk::manage(new Gtk::Image("GUI/partyleader.png"));
			nameBox->pack_start(*leaderIcon,false,false);
		}
		
		nameLabel = Gtk::manage(new Gtk::Label(m_character->name));
		nameBox->pack_start(*nameLabel, false, false);
		
		lvlLabel = Gtk::manage(new Gtk::Label(Glib::ustring::compose("Lvl. %1", m_character->GetLevel())));
		nameBox->pack_start(*lvlLabel, false, false);
		
		set_label_widget(*nameBox);
		
		hpBox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 3));
		
		hpLabel = Gtk::manage(new Gtk::Label("HP:"));
		hpBox->pack_start(*hpLabel, false, false);
		
		if(m_character->name == s.character.name || s.party[0].name.compare(s.character.name) == 0){
			kickEventBox = Gtk::manage(new Gtk::EventBox());
			kickLabel = Gtk::manage(new Gtk::Label());
			//Glib::ustring formatString = Glib::ustring::compose("<span font_style=\"underline\" >kick</span>");
			kickLabel->set_markup("<span underline=\"single\" >kick</span> x");
			kickEventBox->add(*kickLabel);
			hpBox->pack_end(*kickEventBox, false, false);
			kickEventBox->signal_button_release_event().connect(sigc::bind<unsigned short>(sigc::mem_fun(*this, &PartyPlayerFrame::Remove), m_character->id), false);
		}
		else{
			kickLabel = Gtk::manage(new Gtk::Label());
			kickLabel->set_markup("<span underline=\"single\" >kick</span> x");
			kickLabel->set_opacity(0.5);
			hpBox->pack_end(*kickLabel, false, false);
		}
		
		hpLevelBar = Gtk::manage(new Gtk::LevelBar());
		hpLevelBar->set_min_value(0);
		hpLevelBar->set_min_value(100);
		hpLevelBar->set_orientation(Gtk::ORIENTATION_HORIZONTAL);
		hpLevelBar->set_value((int)m_character->GetHPPercent());
		hpBox->pack_end(*hpLevelBar, true, true);
		
		hpBox->set_margin_bottom(3);
		hpBox->set_margin_right(3);
		hpBox->set_margin_left(3);
		hpBox->set_margin_top(3);
		
		add(*hpBox);
		
		set_margin_bottom(3);
		set_margin_right(3);
		set_margin_left(3);
		set_margin_top(3);
		
		show_all();
	}
	
	Character* GetCharacter(){
		return m_character;
	}
};


#endif