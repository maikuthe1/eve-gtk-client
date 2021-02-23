#ifndef LOGROW_H
#define LOGROW_H

enum LogTypes {Info = 0, Warning = 1, Error = 2, Packet = 3};

class LogRow : public Gtk::ListBoxRow
{
public:
	LogRow(Glib::ustring logMessage, LogTypes logType) : text(logMessage), type(logType)
	{
		// Get time
		GDateTime *date_time;
		gchar *dt_format;
		date_time = g_date_time_new_now_local();
		dt_format = g_date_time_format(date_time, "%H:%M:%S");
		time = Glib::ustring(dt_format);
		g_free(dt_format);

		// Set up horizontal box
		box = Gtk::manage(new Gtk::Box(Gtk::Orientation::ORIENTATION_HORIZONTAL, 3));

		// Set up image and label text/formatting
		Gtk::Image* icon = Gtk::manage(new Gtk::Image());

		if(type == LogTypes::Info){
			iconName = "dialog-information";
			formattedText = Glib::ustring::compose("<span font_desc=\"monospace 9\" >%1</span>",text);
		}
		if(type == LogTypes::Packet){
			iconName = "dialog-information";
			formattedText = Glib::ustring::compose("<span font_desc=\"monospace 9\" >%1</span>",text);
		}
		if(type == LogTypes::Warning){
			iconName = "dialog-warning";
			formattedText = Glib::ustring::compose("<span font_desc=\"monospace 9\" foreground=\"#c1c17d7d1111\">\%1</span>", text);
		}
		if(type == LogTypes::Error){
			iconName = "dialog-error";
			formattedText = Glib::ustring::compose("<span font_desc=\"monospace 9\" foreground=\"#cccc00000000\">\%1</span>", text);
		}
		icon->set_from_icon_name(iconName, Gtk::ICON_SIZE_BUTTON);
		icon->set_valign(Gtk::Align::ALIGN_START);

		// Add icon
		box->pack_start(*icon, false, false);

		textLabel = Gtk::manage(new Gtk::Label(text));
		textLabel->set_markup(formattedText);
		textLabel->set_selectable(true);
		textLabel->set_halign(Gtk::Align::ALIGN_START);
		Glib::RefPtr<Gtk::StyleContext> context = textLabel->get_style_context();
		context->add_class("LogShadow");
		// Add label
		box->pack_start(*textLabel, false, true);

		// Add time label to end of box
		timeLabel = Gtk::manage(new Gtk::Label(time));
		box->pack_end(*timeLabel, false, false);

		box->show_all_children();

		// Add box to row
		add(*box);
		set_name("log-padding");
		show_all_children();
	}
	Glib::ustring GetFormattedText(){
		return formattedText;
	}
	Glib::ustring GetLogIcon(){
		return iconName;
	}
	LogTypes GetLogType(){
		return type;
	}
	Glib::ustring GetLogTime(){
		return time;
	}
protected:
	Glib::ustring text;
	LogTypes type;
	Glib::ustring iconName;
	Gtk::Box* box;
	Gtk::Image* icon;
	Gtk::Label* textLabel;
	Glib::ustring formattedText;
	Gtk::Label* timeLabel;
	Glib::ustring time;
};
#endif