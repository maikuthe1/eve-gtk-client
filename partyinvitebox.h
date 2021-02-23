#ifndef PARTYINVITEBOX_H
#define PARTYINVITEBOX_H
#include <gtkmm.h>
#include "eobot/singleton.hpp"

class PartyInviteBox : public Gtk::Box{
protected:
	unsigned short playerID;
	std::string playerName;
	unsigned char invType;
	
	// Boxes
	Gtk::Box *buttonBox;
	
	// Buttons
	Gtk::Button *acceptButton, *declineButton;
	
	// Check Buttons
	Gtk::CheckButton *rejectCheckButton;
	
	void CheckReject(){
		S &s = S::GetInstance();
		if(rejectCheckButton->get_active())
			s.blockInviteList.push_back(playerName); // Deletes the grid, not correct add delete function to draggable!
	}
	
	void AcceptButtonClicked(){
		S &s = S::GetInstance();
		CheckReject();
		s.eoclient.AcceptPartyInvite(invType, playerID);
		delete(get_parent());
	}
	
	void DeclineButtonClicked(){
		S &s = S::GetInstance();
		CheckReject();
		delete(get_parent()); // Deletes the grid, not correct, should delete the eventbox
	}
	
public:
	PartyInviteBox(unsigned char type, unsigned short pID, std::string pName) : Gtk::Box(Gtk::ORIENTATION_VERTICAL){
		playerID = pID;
		playerName = pName;
		invType = type;
		
		buttonBox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL));
		buttonBox->set_spacing(20);
		buttonBox->set_valign(Gtk::ALIGN_CENTER);
		buttonBox->set_halign(Gtk::ALIGN_CENTER);
		acceptButton = Gtk::manage(new Gtk::Button("Join"));
		declineButton = Gtk::manage(new Gtk::Button("Decline"));
		buttonBox->pack_start(*acceptButton);
		buttonBox->pack_start(*declineButton);
		pack_start(*buttonBox, true, true);
		
		rejectCheckButton = Gtk::manage(new Gtk::CheckButton("Auto. reject invitations from \" "+ playerName +"\""));
		pack_end(*rejectCheckButton);
		
		acceptButton->signal_clicked().connect(sigc::mem_fun(*this, &PartyInviteBox::AcceptButtonClicked));
		declineButton->signal_clicked().connect(sigc::mem_fun(*this, &PartyInviteBox::DeclineButtonClicked));
		
		show_all_children();
	};
};

#endif