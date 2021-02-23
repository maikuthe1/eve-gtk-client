#include "chatbox.h"

ChatBox::ChatBox(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade) :
	Gtk::Box(cobject), builder(refGlade){
	S &s = S::GetInstance();

	// Widgets
	builder->get_widget("LocalChatBox", localChatBox);
	builder->get_widget("GlobalChatBox", globalChatBox);
	builder->get_widget("PartyChatBox", partyChatBox);
	builder->get_widget("GuildChatBox", guildChatBox);
	builder->get_widget("ChatEntry", chatInput);
	builder->get_widget("ChatSendButton", sendButton);
	builder->get_widget_derived("LocalChatScrolledWindow", localScroll);
	localScroll->SetChatType(ChatType::Local);
	builder->get_widget_derived("GlobalChatScrolledWindow", globalScroll);
	globalScroll->SetChatType(ChatType::Global);
	builder->get_widget_derived("PartyChatScrolledWindow", partyScroll);
	partyScroll->SetChatType(ChatType::Party);
	builder->get_widget_derived("GuildChatScrolledWindow", guildScroll);
	guildScroll->SetChatType(ChatType::Guild);
	builder->get_widget_derived("EmoteScrolledWindow", emoteScroll);
	builder->get_widget("ChatNotebook", chatNotebook);
	builder->get_widget("ChatEmoteBox", emoteBox);

	// Signals
	s.signalMessageReceived.connect(sigc::mem_fun(*this, &ChatBox::MessageReceived));
	s.signalMessageNotDelivered.connect(sigc::mem_fun(*this, &ChatBox::MessageNotDelivered));
	sendButton->signal_clicked().connect(sigc::mem_fun(*this, &ChatBox::SendMessage));
	chatInput->signal_activate().connect(sigc::mem_fun(*this, &ChatBox::SendMessage));
	chatInput->signal_icon_press().connect(sigc::mem_fun(*this, &ChatBox::IconPressed));
	chatNotebook->signal_switch_page().connect(sigc::mem_fun(*this, &ChatBox::TabSwitched));
}

void ChatBox::MessageNotDelivered(std::string name){
	S &s = S::GetInstance();
	Glib::ustring nameU = name;
	if(nameU == s.character.name)
		return;
	for(unsigned int i = 0; i < privateChats.size(); i++){
		if(privateChats[i]->GetChild()->GetName() == nameU){
			Gtk::Label* notDeliveredLabel = Gtk::manage(new Gtk::Label("- Player unavailable -"));
			privateChats[i]->GetBoxChild()->pack_start(*notDeliveredLabel);
			privateChats[i]->GetBoxChild()->show_all();
		}
	}
}

void ChatBox::TabSwitched(Gtk::Widget* switched, const unsigned int uuuu){
	chatInput->grab_focus();
}

void ChatBox::StartChat(){
	S &s = S::GetInstance();
	bool chatOpen = false;
		ChatTab* chatTab;
	Glib::ustring name = selectedCharacter.name;
	if(name == s.character.name)
		return;
	for(unsigned int i = 0; i < privateChats.size(); i++){
		if(privateChats[i]->GetChild()->GetName() == name){
			chatTab = privateChats[i];
			chatOpen = true;
		}
	}
	if(!chatOpen){
		ChatScrolledWindow* scrolledWin = new ChatScrolledWindow();
		Gtk::Box* chatBox = new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 3);
		chatBox->set_margin_top(3);
		chatBox->set_margin_right(3);
		chatBox->set_margin_bottom(3);
		chatBox->set_margin_left(3);
		scrolledWin->SetChatType(ChatType::Private);
		scrolledWin->SetName(selectedCharacter.name);
		scrolledWin->add(*chatBox);
		ChatTab* newTab = new ChatTab((Glib::ustring)selectedCharacter.name);
		newTab->SetChild(scrolledWin);
		newTab->SetBoxChild(chatBox);
		int page = chatNotebook->append_page(*scrolledWin, *newTab);
		chatNotebook->show_all();
		privateChats.push_back(newTab);
		chatTab = newTab;
		newTab->show_all();
		scrolledWin->show_all();
		chatBox->show_all();
		newTab->GetCloseButton()->signal_clicked().connect(sigc::bind<ChatTab*>(sigc::mem_fun(*this, ChatBox::TabCloseButtonClicked), chatTab));
		chatNotebook->set_current_page(page);
		if (buffer.loadFromFile("GUI/pmnew.ogg")){
			sound.setBuffer(buffer);
			sound.play();
		}
	}
}

void ChatBox::IconPressed(Gtk::EntryIconPosition iconPosition, const GdkEventButton *eventButton){
	if(iconPosition == Gtk::ENTRY_ICON_SECONDARY){
		emoteScroll->set_visible(!emoteScroll->get_visible());
	}
}
void ChatBox::SendMessage(){
	S &s = S::GetInstance();
	Logger* logger = Logger::Instance();
	int currentPage = chatNotebook->get_current_page();
	ChatScrolledWindow* currentWidget = (ChatScrolledWindow*)chatNotebook->get_nth_page(currentPage);
	Glib::ustring messageText = chatInput->get_text();
	ChatMessage message(s.character.name, (std::string)currentWidget->GetName(), (std::string)messageText, currentWidget->GetChatType());
	//logger->Log(Glib::ustring::compose("Sending message to %1", currentWidget->GetName()));
	MessageReceived(message);
	s.eoclient.SendChatMessage(message);
	chatInput->set_text("");
}

void ChatBox::MessageReceived(ChatMessage message){
	S &s = S::GetInstance();
	ChatMessageBox* messageBox = Gtk::manage(new ChatMessageBox(Gtk::ORIENTATION_VERTICAL, 3, message));
	Gtk::Revealer* messageRevealer = Gtk::manage(new Gtk::Revealer());
	messageRevealer->set_transition_type(Gtk::REVEALER_TRANSITION_TYPE_SLIDE_UP);
	messageRevealer->set_transition_duration(150);
	messageRevealer->add(*messageBox);
	messageRevealer->show_all();
	messageRevealer->set_reveal_child(false);

	if(message.type == ChatType::Private){
		bool chatOpen = false;
		ChatTab* chatTab;
		Glib::ustring name = message.sender;
		if(name == s.character.name)
			name = message.receiver;
		for(unsigned int i = 0; i < privateChats.size(); i++){
			if(privateChats[i]->GetChild()->GetName() == name){
				chatTab = privateChats[i];
				chatOpen = true;
			}
		}
		if(!chatOpen){
			ChatScrolledWindow* scrolledWin = new ChatScrolledWindow();
			Gtk::Box* chatBox = new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 3);
			chatBox->set_margin_top(3);
			chatBox->set_margin_right(3);
			chatBox->set_margin_bottom(3);
			chatBox->set_margin_left(3);
			scrolledWin->SetChatType(ChatType::Private);
			scrolledWin->SetName(message.sender);
			scrolledWin->add(*chatBox);
			ChatTab* newTab = new ChatTab((Glib::ustring)message.sender);
			newTab->SetChild(scrolledWin);
			newTab->SetBoxChild(chatBox);
			chatNotebook->append_page(*scrolledWin, *newTab);
			chatNotebook->show_all();
			privateChats.push_back(newTab);
			chatTab = newTab;
			newTab->show_all();
			scrolledWin->show_all();
			chatBox->show_all();
			newTab->GetCloseButton()->signal_clicked().connect(sigc::bind<ChatTab*>(sigc::mem_fun(*this, ChatBox::TabCloseButtonClicked), chatTab));
			if (buffer.loadFromFile("GUI/pmnew.ogg")){
				sound.setBuffer(buffer);
				sound.play();
			}
		}
		else{
			if (buffer.loadFromFile("GUI/pm.ogg")){
				sound.setBuffer(buffer);
				sound.play();
			}
		}
		privateChats.back()->GetBoxChild()->pack_start(*messageRevealer, false, false);
		messageRevealer->set_reveal_child(true);
		Glib::RefPtr<Gtk::Adjustment> adj = chatTab->GetChild()->get_vadjustment();
		adj->set_value(adj->get_upper());
	}
	if(message.type == ChatType::Local){
		localChatBox->pack_start(*messageRevealer, false, false);
		messageRevealer->set_reveal_child(true);
		Glib::RefPtr<Gtk::Adjustment> adj = localScroll->get_vadjustment();
		adj->set_value(adj->get_upper());
	}
	if(message.type == ChatType::Global){
		globalChatBox->pack_start(*messageRevealer, false, false);
		messageRevealer->set_reveal_child(true);
		Glib::RefPtr<Gtk::Adjustment> adj = globalScroll->get_vadjustment();
		adj->set_value(adj->get_upper());
	}
	if(message.type == ChatType::Party){
		partyChatBox->pack_start(*messageRevealer, false, false);
		messageRevealer->set_reveal_child(true);
		Glib::RefPtr<Gtk::Adjustment> adj = partyScroll->get_vadjustment();
		adj->set_value(adj->get_upper());
	}
}

void ChatBox::TabCloseButtonClicked(ChatTab* tab){
	std::vector<ChatTab*>::iterator it = std::find(privateChats.begin(), privateChats.end(), tab);
	privateChats.erase(it);
	chatNotebook->remove_page(*tab->GetChild());
}