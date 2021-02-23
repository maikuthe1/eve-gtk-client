#ifndef INVENTORYSCROLLEDWINDOW_H
#define INVENTORYSCROLLEDWINDOW_H
#include <gtkmm.h>
#include "eobot/singleton.hpp"
#include "inventoryflowbox.h"

class InventoryScrolledWindow : public Gtk::ScrolledWindow{
protected:
	Glib::RefPtr<Gtk::Builder> builder;
	InventoryFlowBox *inventoryFlowBox;
	Gtk::Label *weightLabel;

public:
	InventoryScrolledWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade) :
	Gtk::ScrolledWindow(cobject), builder(refGlade){
		builder->get_widget_derived("InventoryFlowbox", inventoryFlowBox);
		builder->get_widget("WeightLabel", weightLabel);
	}
};

#endif