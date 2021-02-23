#ifndef MENUBARBUTTON_H
#define MENUBARBUTTON_H
#include <gtkmm.h>
#include "eobot/singleton.hpp"

class MenuBarButton : public Gtk::Button{
private:
	Glib::RefPtr<Gtk::Builder> builder;
	Gtk::Widget* linkWidget;
	double activeTrans = 1, inactiveTrans = 0.4;
	
public:
	MenuBarButton(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade) : 
	Gtk::Button(cobject), builder(refGlade){
		signal_clicked().connect(sigc::mem_fun(*this, &MenuBarButton::Clicked));
	}
	
	void SetLinkWidget(Gtk::Widget* widget){
		linkWidget = widget;
		linkWidget->signal_hide().connect(sigc::mem_fun(*this, &MenuBarButton::LinkHide));
		LinkHide();
	}
	
	Gtk::Widget* GetLinkWidget(){
		return linkWidget;
	}
	
	void Clicked(){
		linkWidget->set_visible(!linkWidget->get_visible());
		//linkWidget->signal_hide()
		set_opacity((linkWidget->get_visible()) ? activeTrans : inactiveTrans );
	}
	
	void LinkHide(){
		set_opacity((linkWidget->get_visible()) ? activeTrans : inactiveTrans );
	}
};

#endif