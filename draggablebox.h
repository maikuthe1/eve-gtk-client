#ifndef DRAGGABLEBOX_H
#define DRAGGABLEBOX_H
#include <gtkmm.h>
#include "eobot/singleton.hpp"

class DraggableBox : public Gtk::EventBox{
protected:

	const gint Sensitivity = 2;
	inline static int Min(const int a, const int b) { return b < a ? b : a; }
	inline static int Max(const int a, const int b) { return b > a ? b : a; }

	inline static int RoundDownToMultiple(const int i, const int m)
	{
		return i/m*m;
	}
	inline static int RoundToNearestMultiple(const int i, const int m)
	{
		if (i % m > (double)m / 2.0d)
			return (i/m+1)*m;
		return i/m*m;
	}
	
	Glib::RefPtr<Gtk::Builder> builder;
	
	// Grid
	Gtk::Grid* mainGrid;
	
	// Seperators
	Gtk::Separator *topSeparator, *rightSeparator, *bottomSeparator, *leftSeparator, *tlSeparator, *trSeparator, *blSeparator, *brSeparator;
	
	// Settings
	Glib::RefPtr<Gio::Settings> gioSettings;
	Glib::RefPtr<Gtk::Settings> gtkSettings;
	
	// Bindings
	Glib::RefPtr<Glib::Binding> transparencyBind;
	
	// Headerbars
	Gtk::HeaderBar *titlebar;
	
	// Eventboxes
	Gtk::EventBox *titleEventBox, *topSeparatorEventBox, *rightSeparatorEventBox, *bottomSeparatorEventBox, *leftSeparatorEventBox, *tlSeparatorEventBox, *trSeparatorEventBox, *blSeparatorEventBox, *brSeparatorEventBox;
	
	// Images
	Gtk::Image *icon;
	
	// Fixeds
	
	// Overlays
	Gtk::Overlay *mainOverlay;
	
	// Windows
	Gtk::Window *win;
	
	// Labels
	Gtk::Label *titleLabel;
	
	// Boxes
	Gtk::Box *titleBox, *contentBox;
	
	// Buttons
	Gtk::Button *closeButton;
	
	bool MouseDown(GdkEventButton* event);
	bool MouseUp(GdkEventButton* event);
	bool MouseMoved(GdkEventMotion* event);
	bool CrossBorder(GdkEventCrossing* event, Gtk::Widget* widget);
	bool BorderDrag(GdkEventMotion* event, Gtk::Widget* widget);
	
	int offsetx, offsety, px, py, maxx, maxy;
	Glib::RefPtr<Gdk::Window> www;
	Glib::RefPtr<Gdk::Display> display;
	void OnShow();
public:
	DraggableBox(Gtk::Window* mainWin, Gtk::Overlay *overlay, Gtk::Widget* content, Glib::ustring title, Glib::ustring iconName, unsigned int width, unsigned int height, Gtk::Orientation orientation = Gtk::ORIENTATION_VERTICAL, bool showCloseButton = true);
	Gtk::HeaderBar* GetTitlebar(){
		return titlebar;
	}
	void Hide();
};


#endif