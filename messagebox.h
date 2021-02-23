#ifndef MESSAGEBOXBOX_H
#define MESSAGEBOXBOX_H
#include <gtkmm.h>
#include "eobot/singleton.hpp"

// MessageBox was taken by GTK :(
class EOMessageBox : public Gtk::Box{
protected:
	Gtk::HeaderBar *headerBar;
	Gtk::Button *okButton;
	Gtk::TextView *textView;
	Gtk::ScrolledWindow *scrolledWindow;
	Glib::RefPtr<Gtk::TextBuffer> textBuffer;
	int sizes[256] = {
		 3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3, /* NUL -  SI */
		 3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3,  3, /* DLE -  US */
		 3,  3,  5,  7,  6,  8,  6,  2,  3,  3,  4,  6,  3,  3,  3,  5, /* ' ' - '/' */
		 6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  3,  3,  6,  6,  6,  6, /* '0' - '?' */
		11,  7,  7,  7,  8,  7,  6,  8,  8,  3,  5,  7,  6,  9,  8,  8, /* '@' - 'O' */
		 7,  8,  8,  7,  7,  8,  7, 11,  7,  7,  7,  3,  5,  3,  6,  6, /* 'P' - '_' */
		 3,  6,  6,  6,  6,  6,  3,  6,  6,  2,  2,  6,  2,  8,  6,  6, /* '`' - 'o' */
		 6,  6,  3,  5,  3,  6,  6,  8,  5,  5,  5,  4,  2,  4,  7,  3, /* 'p' - DEL */
		 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		 3,  3,  6,  6,  6,  6,  2,  6,  3,  9,  4,  6,  6,  3,  8,  6,
		 4,  6,  3,  3,  3,  6,  6,  3,  3,  3,  4,  6,  8,  8,  8,  6,
		 7,  7,  7,  7,  7,  7, 10,  7,  7,  7,  7,  7,  3,  3,  3,  3,
		 8,  8,  8,  8,  8,  8,  8,  6,  8,  8,  8,  8,  8,  7,  7,  6,
		 6,  6,  6,  6,  6,  6, 10,  6,  6,  6,  6,  6,  2,  4,  4,  4,
		 6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  5,  6,  5,
	};

	std::string BreakString(std::string theString){
		unsigned int maxWidth = 198;
		int index = 0;
		int count = 1;
		for(char currChar : theString){
			count += sizes[currChar];
			if(count >= maxWidth){
				count = 0;
				theString.insert(index, "\n");
			}
			index++;
		}

		return theString;
	}

public:
	EOMessageBox(Glib::ustring title, Glib::ustring message) : Gtk::Box(Gtk::ORIENTATION_VERTICAL, 3){
		//headerBar = Gtk::manage(new Gtk::HeaderBar());
		//headerBar->set_title(title);
		//pack_start(headerBar);

		//textBuffer = new Glib::RefPtr<Gtk::TextBuffer>();
		/*
		 * Instead of creating a new buffer we could do textView->get_buffer();
		 */
		//textBuffer = Gtk::TextBuffer::create();
		//textBuffer->set_text(message);


		textView = Gtk::manage(new Gtk::TextView());
		//textView->set_buffer(textBuffer);
		textView->get_buffer()->set_text(BreakString(message));

		scrolledWindow = Gtk::manage(new Gtk::ScrolledWindow());
		scrolledWindow->set_propagate_natural_width(true);
		scrolledWindow->add(*textView);
		pack_start(*scrolledWindow, true, true);

		show_all_children();
		show();
	};
};

#endif