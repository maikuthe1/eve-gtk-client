#include "mapscrolledwindow.h"
#include <iostream>

MapScrolledWindow::MapScrolledWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade){
	add_events(Gdk::BUTTON3_MOTION_MASK | Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK);
	signal_button_press_event().connect(sigc::mem_fun(*this, &MapScrolledWindow::MousePressed), false);
	signal_button_release_event().connect(sigc::mem_fun(*this, &MapScrolledWindow::MouseReleased), false);
	signal_motion_notify_event().connect(sigc::mem_fun(*this, &MapScrolledWindow::MouseMoved));
}

bool MapScrolledWindow::MousePressed(GdkEventButton* event){
	std::cout << "yo";
	if(event->button == 3){
		mouseDown = true;
		previousY = event->y;
		previousX = event->x;
		previousVPosition = get_vadjustment()->get_value();
		previousHPosition = get_hadjustment()->get_value();
	}
	return false;
}

bool MapScrolledWindow::MouseReleased(GdkEventButton* event){
	mouseDown = false;
	return false;
}

bool MapScrolledWindow::MouseMoved(GdkEventMotion* event){
	if(!mouseDown)
		return true;
	
	int currentY = event->y;
	int currentX = event->x;
	int YMovement = (previousY - currentY);
	int XMovement = (previousX - currentX);
	int scrollYPosition = (previousVPosition + YMovement);
	int scrollXPosition = (previousHPosition + XMovement);
	get_vadjustment()->set_value(scrollYPosition);
	get_vadjustment()->set_value(scrollXPosition);
	std::vector<unsigned int> padding;
	return true;
}