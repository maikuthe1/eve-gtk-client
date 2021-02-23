#ifndef CHATMESSAGEBOX_H
#define CHATMESSAGEBOX_H
#include <gtkmm.h>
#include "eobot/singleton.hpp"

class ChatMessageBox : public Gtk::Box{
public:
	ChatMessageBox(Gtk::Orientation porientation, gint pmargin, ChatMessage message) 
	: Gtk::Box(porientation, pmargin), message(message){
		S &s = S::GetInstance();
		nameBox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 3));
		Gtk::Separator* seperator = Gtk::manage(new Gtk::Separator(Gtk::ORIENTATION_HORIZONTAL));
		//pack_start(*seperator);
		pack_start(*nameBox, true, true);
		
		// Guild / Name
		Glib::ustring formattedName;
		if(message.type == ChatType::Local)
		formattedName = Glib::ustring::compose("<span font_weight=\"bold\" >[%1]%2:</span>", 
															s.map.characters[s.map.GetCharacterIndex(message.sender)].guild_tag, 
															message.sender);
		if(message.type == ChatType::Global || message.type == ChatType::Guild || message.type == ChatType::Party)
			formattedName = Glib::ustring::compose("<span font_weight=\"bold\" >%1:</span>", message.sender);
		if(message.type == ChatType::Private)
			formattedName = Glib::ustring::compose("<span font_weight=\"bold\" >%1:</span>", message.sender);
		nameLabel = Gtk::manage(new Gtk::Label());
		nameLabel->set_valign(Gtk::ALIGN_START);
		nameLabel->set_markup(formattedName);
		nameBox->pack_start(*nameLabel, false, false);
		
		// Time
		GDateTime *date_time;
		gchar *dt_format;
		date_time = g_date_time_new_now_local();
		dt_format = g_date_time_format(date_time, "%H:%M");
		Glib::ustring time = Glib::ustring(dt_format);
		g_free(dt_format);
		Glib::ustring formattedTime = Glib::ustring::compose("<font scale=\"0.9\" >%1</font>", time);
		timeLabel = Gtk::manage(new Gtk::Label(time));
		timeLabel->set_opacity(0.3f);
		timeLabel->set_valign(Gtk::ALIGN_START);
		//timeLabel->set_markup(formattedTime);
		nameBox->pack_end(*timeLabel, false, false);
		
		// Message
		messageLabel = Gtk::manage(new Gtk::Label(message.message));
		messageLabel->set_halign(Gtk::ALIGN_START);
		messageLabel->set_line_wrap(true);
		messageLabel->set_line_wrap_mode(Pango::WRAP_CHAR);
		nameBox->pack_start(*messageLabel, false, true);
		
		Gtk::Separator* seperator2 = Gtk::manage(new Gtk::Separator(Gtk::ORIENTATION_HORIZONTAL));
		//pack_start(*seperator2);
		Glib::RefPtr<Gtk::StyleContext> context = get_style_context();
		context->add_class("message");
		show_all_children();
		show_all();
	}
	
protected:
	Gtk::Box* nameBox;
	Gtk::Label *nameLabel, *timeLabel, *messageLabel;
	ChatMessage message;
};

#endif