#include "partybox.h"

PartyBox::PartyBox() : Gtk::Box(Gtk::ORIENTATION_VERTICAL, 3){
	S &s = S::GetInstance();
	for(unsigned int i = 0; i < s.party.size(); i++){
		PartyPlayerFrame *partyPlayerFrame = Gtk::manage(new PartyPlayerFrame(&s.party[i]));
		pack_start(*partyPlayerFrame, false, false);
	}
	show_all();
	if (joinSoundBuffer.loadFromFile("GUI/party_join.ogg")){
		joinSound.setBuffer(joinSoundBuffer);
		joinSound.play();
	}
	
	s.signalPartyRemove.connect(sigc::mem_fun(*this, &PartyBox::PartyRemove));
	s.signalPartyAdd.connect(sigc::mem_fun(*this, &PartyBox::PartyAdd));
}

void PartyBox::PartyRemove(Character* character){
	S &s = S::GetInstance();
	std::vector<Gtk::Widget*> children = get_children();
	for(unsigned int i = 0; i < children.size(); i++){
		PartyPlayerFrame* ppf = (PartyPlayerFrame*)children[i];
		if(ppf->GetCharacter() == character){
			remove(*children[i]);
			if (joinSoundBuffer.loadFromFile("GUI/click_standard_05.wav")){
				joinSound.setBuffer(joinSoundBuffer);
				joinSound.play();
			}
		}
	}
}

void PartyBox::PartyAdd(Character* character){
	S &s = S::GetInstance();
	PartyPlayerFrame *partyPlayerFrame = Gtk::manage(new PartyPlayerFrame(character));
	if (joinSoundBuffer.loadFromFile("GUI/party_join.ogg")){
		joinSound.setBuffer(joinSoundBuffer);
		joinSound.play();
	}
	pack_start(*partyPlayerFrame, false, false);
	show_all();
}