#ifndef MAPSCROLLEDWINDOW_H
#define MAPSCROLLEDWINDOW_H
#include <gtkmm-3.0/gtkmm.h>


class MapScrolledWindow : public Gtk::ScrolledWindow
{
public:
	MapScrolledWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade);
protected:
	bool mouseDown = false;
	int previousY = 0;
	int previousX = 0;
	int previousVPosition = 0;
	int previousHPosition = 0;
	
	// Event handlers
	bool MousePressed(GdkEventButton* event);
	bool MouseReleased(GdkEventButton* event);
	bool MouseMoved(GdkEventMotion* event);
	
};

#endif