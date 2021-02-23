#include "statusframe.h"

StatusFrame::StatusFrame(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade) :
	Gtk::Frame(cobject), builder(refGlade){
	S &s = S::GetInstance();
	
	builder->get_widget("StatusNameLabel", nameLabel);
	builder->get_widget("StatusLevelLabel", levelLabel);
	builder->get_widget("StatusHPLevelBar", hpBar);
	builder->get_widget("StatusTPLevelBar", tpBar);
	
	nameLabel->set_text(Glib::ustring::compose("[%1] %2", s.character.guild_tag, s.character.name));
	levelLabel->set_text(Glib::ustring::compose("Lvl. %1", s.character.GetLevel()));
	
	s.character.signalChanged.connect(sigc::mem_fun(*this, &StatusFrame::CharacterChanged));
	
	CharacterChanged(ChangeType::HP);
	CharacterChanged(ChangeType::TP);
}

void StatusFrame::CharacterChanged(ChangeType changeType){
	S &s = S::GetInstance();
	if(changeType == ChangeType::HP){
		double hp = (double)((double)s.character.GetHP() / (double)s.character.GetMaxHP());
		hpBar->set_value(hp);
	}
	if(changeType == ChangeType::TP){
		double tp = (double)((double)s.character.GetTP() / (double)s.character.GetMaxTP());
		tpBar->set_value(tp);
	}
}