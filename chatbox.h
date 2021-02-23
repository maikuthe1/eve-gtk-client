#ifndef CHATBOX_H
#define CHATBOX_H
#include <gtkmm.h>
#include <SFML/Audio.hpp>
#include "eobot/singleton.hpp"
#include "chatmessagebox.h"
#include "chatscrolledwindow.h"
#include "chattab.h"

class ChatBox : public Gtk::Box{
protected:
	Glib::RefPtr<Gtk::Builder> builder;
	
	// Boxes
	Gtk::Box *localChatBox, *globalChatBox, *partyChatBox, *guildChatBox, *emoteBox;
	
	// Notebooks
	Gtk::Notebook *chatNotebook;
	
	// Entries
	Gtk::Entry* chatInput;
	
	// Buttons
	Gtk::Button* sendButton;
	
	// Scrolled Windows
	ChatScrolledWindow *localScroll, *globalScroll, *partyScroll, *guildScroll, *emoteScroll;
	
	std::vector<ChatTab*> privateChats;
	
	Character selectedCharacter;
	sf::Sound sound;
	sf::SoundBuffer buffer;
	
	// Signals
	void MessageReceived(ChatMessage message);
	void SendMessage();
	void IconPressed(Gtk::EntryIconPosition iconPosition, const GdkEventButton *eventButton);
	void TabCloseButtonClicked(ChatTab* tab);
	void TabSwitched(Gtk::Widget* switched, const unsigned int uuuu);
	void MessageNotDelivered(std::string name);
	
public:
	ChatBox(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade);
	void StartChat();
	void SetSelectedCharacter(Character character){
		selectedCharacter = character;
	}
};


#endif