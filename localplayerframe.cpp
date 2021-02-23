#include "localplayerframe.h"

LocalPlayerFrame::LocalPlayerFrame(Character character) : character(character){
	S &s = S::GetInstance();
	if(character.GetIcon() == PaperdollIcon::Normal)
		partyStatus = s.atlas.GetSubSprite("gfx002.egf", 32, 0, 117, 13, 13);
	if(character.GetIcon() == PaperdollIcon::GM || character.GetIcon() == PaperdollIcon::HGM)
		partyStatus = s.atlas.GetSubSprite("gfx002.egf", 32, 0, 156, 13, 13);
	else
		partyStatus = s.atlas.GetSubSprite("gfx002.egf", 32, 0, 117, 13, 13);
	levelLabel = Gtk::manage(new Gtk::Label(Glib::ustring::compose("Lvl. %1", character.GetLevel())));
	levelLabel->set_valign(Gtk::ALIGN_END);
	nameBox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 3));
	nameBox->pack_start(*partyStatus);
	nameLabel = Gtk::manage(new Gtk::Label());
	nameLabel->set_markup(Glib::ustring::compose("<span font_weight=\"bold\">[%1] %2</span>", character.guild_tag, character.name));
	nameBox->pack_start(*nameLabel);
	nameBox->pack_start(*levelLabel);
	set_label_widget(*nameBox);
	
	mainBox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 3));
	vitalityBox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 3));
	vitalityBox->set_margin_bottom(3);
	vitalityBox->set_margin_left(3);
	vitalityBox->set_margin_top(3);
	vitalityBox->set_margin_right(3);
	hpLabel = Gtk::manage(new Gtk::Label("HP:"));
	tpLabel = Gtk::manage(new Gtk::Label("TP:"));
	hpBar = Gtk::manage(new Gtk::LevelBar());
	tpBar = Gtk::manage(new Gtk::LevelBar());
	vitalityBox->pack_start(*hpLabel, false, true);
	vitalityBox->pack_start(*hpBar, true, true);
	vitalityBox->pack_start(*tpLabel, false, true);
	vitalityBox->pack_start(*tpBar, true, true);
	mainBox->pack_start(*vitalityBox);
	bottomBox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 3));
	bottomBox->set_margin_bottom(3);
	bottomBox->set_margin_left(3);
	bottomBox->set_margin_right(3);
	iconBox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 3));
	
	character.signalChanged.connect(sigc::mem_fun(*this, &LocalPlayerFrame::CharacterChanged));
	CharacterChanged(ChangeType::HP);
	CharacterChanged(ChangeType::TP);
	
	add(*mainBox);
	show_all();
}

void LocalPlayerFrame::CharacterChanged(ChangeType changeType){
	S &s = S::GetInstance();
	if(changeType == ChangeType::HP){
		double hp = (double)((double)character.GetHP() / (double)character.GetMaxHP());
		hpBar->set_value(hp);
	}
	if(changeType == ChangeType::TP){
		double tp = (double)((double)character.GetTP() / (double)character.GetMaxTP());
		tpBar->set_value(tp);
	}
}