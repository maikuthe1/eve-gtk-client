#ifndef CHATSCROLLEDWINDOW_H
#define CHATSCROLLEDWINDOW_H
#include <gtkmm.h>
#include "eobot/singleton.hpp"

class ChatScrolledWindow : public Gtk::ScrolledWindow{
protected:
	Glib::RefPtr<Gtk::Builder> builder;
	ChatType chatType;
	Glib::ustring name;
	
public:
	ChatScrolledWindow(){ set_vexpand_set(true); };
	ChatScrolledWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade) : 
	Gtk::ScrolledWindow(cobject), builder(refGlade){ };
	ChatType GetChatType(){
		return chatType;
	}
	void SetChatType(ChatType newType){
		chatType = newType;
	}
	Glib::ustring GetName(){
		return name;
	}
	void SetName(Glib::ustring newName){
		name = newName;
	}
};


#endif