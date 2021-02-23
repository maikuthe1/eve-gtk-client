#ifndef ONLINEPLAYERBOX_H
#define ONLINEPLAYERBOX_H
#include <gtkmm.h>
#include "eobot/singleton.hpp"

class OnlinePlayerBox : public Gtk::Box{
private:
	Character character;
	Gtk::Image *statusImage;
	Gtk::Label *nameLabel, *titleLabel, *classLabel, *guildLabel;
	
public:
	OnlinePlayerBox(Character newCharacter) : Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 3){
		character = newCharacter;
		S &s = S::GetInstance();
		if(character.GetIcon() == PaperdollIcon::Normal)
			statusImage = s.atlas.GetSubSprite("gfx002.egf", 32, 0, 117, 13, 13);
		if(character.GetIcon() == PaperdollIcon::GM || character.GetIcon() == PaperdollIcon::HGM)
			statusImage = s.atlas.GetSubSprite("gfx002.egf", 32, 0, 156, 13, 13);
		if(character.GetIcon() == PaperdollIcon::Party)
			statusImage = s.atlas.GetSubSprite("gfx002.egf", 32, 0, 130, 13, 13);
		if(character.GetIcon() == PaperdollIcon::GMParty || character.GetIcon() == PaperdollIcon::HGMParty)
			statusImage = s.atlas.GetSubSprite("gfx002.egf", 32, 0, 169, 13, 13);
		//else
			//statusImage = s.atlas.GetSubSprite("gfx002.egf", 32, 0, 117, 13, 13);
		pack_start(*statusImage,false,false);
		
		guildLabel = Gtk::manage(new Gtk::Label("["+character.guild_tag+"]"));
		Glib::RefPtr<Gtk::StyleContext> context = guildLabel->get_style_context();
		context->add_class("guildTag");
		pack_start(*guildLabel,false,false);
		
		nameLabel = Gtk::manage(new Gtk::Label());
		nameLabel->set_markup(Glib::ustring::compose("<span font_weight=\"bold\" >%2</span>", character.guild_tag, character.name));
		pack_start(*nameLabel,false,false);
		
		
		ECF_Data classData = s.ecf->Get(character.clas);
		classLabel = Gtk::manage(new Gtk::Label(classData.name));
		pack_end(*classLabel,false,false);
		show_all();
		
		titleLabel = Gtk::manage(new Gtk::Label(character.title));
		titleLabel->set_tooltip_text(character.title);
		titleLabel->set_ellipsize(Pango::ELLIPSIZE_END);
		titleLabel->set_opacity(0.4);
		pack_end(*titleLabel, false, false);
		
	}
	Character GetCharacter(){ return character; }
};
#endif