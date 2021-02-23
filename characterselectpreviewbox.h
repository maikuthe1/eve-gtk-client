#ifndef CHARACTERSELECTPREVIEWBOX_H
#define CHARACTERSELECTPREVIEWBOX
#include <gtkmm.h>
#include "eobot/singleton.hpp"

class CharacterSelectPreviewBox : public Gtk::Box{
public:
	CharacterSelectPreviewBox(Gtk::Orientation porientation, gint pmargin, int index) 
	: Gtk::Box(porientation, pmargin), index(index){
		S &s = S::GetInstance();
		nameBox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 3));
		nameLabel = Gtk::manage(new Gtk::Label(s.eoclient.account.characters[index].name));
		levelLabel = Gtk::manage(new Gtk::Label(std::to_string(s.eoclient.account.characters[index].GetLevel())));
		nameBox->pack_start(*nameLabel, false, false);
		nameBox->pack_end(*levelLabel, false, false);
		nameBox->show_all();
		nameBox->show_all_children();
		pack_start(*nameBox, true, true, 0);
		
		characterPreviewImage = Gtk::manage(new Gtk::Image("gui/person.png"));
		pack_start(*characterPreviewImage, true,true, 0);
		
		playButton = Gtk::manage(new Gtk::Button("Play"));
		playButton->set_halign(Gtk::ALIGN_CENTER);
		pack_end(*playButton, false, false);
		playButton->signal_clicked().connect(sigc::mem_fun(*this, &CharacterSelectPreviewBox::PlayButtonClicked));
		
		show_all();
		show_all_children();
	}
	
protected:
	Gtk::Box* nameBox;
	Gtk::Label *nameLabel, *levelLabel;
	Gtk::Image* characterPreviewImage;
	Gtk::Button* playButton;
	int index;
	void PlayButtonClicked(){
		S &s = S::GetInstance();
		s.eoclient.SelectCharacter(s.eoclient.account.characters[index].id);
		s.character = s.eoclient.account.characters[index];
	}
};

#endif