#ifndef INVENTORYFLOWBOX_H
#define INVENTORYFLOWBOX_H
#include <gtkmm.h>
#include "eobot/singleton.hpp"
#include "inventoryflowboxchild.h"

class InventoryFlowBox : public Gtk::FlowBox{
protected:
	Glib::RefPtr<Gtk::Builder> builder;
	void ItemAdded(short itemID, int amount){
		InventoryFlowBoxChild *invChild = Gtk::manage(new InventoryFlowBoxChild(itemID, amount));
		add(*invChild);
	}
	
	void ItemChanged(short itemID, int amount){
		if(amount == 0){
			std::vector<Gtk::Widget*> children = get_children();
			for(unsigned int i = 0; i < children.size(); i++){
				InventoryFlowBoxChild *child = (InventoryFlowBoxChild*)children[i];
				if(child->GetItemID() == itemID){
					remove(*child);
					delete(child);
					check_resize();
					break;
				}
			}
		}
	}
	
public:
	InventoryFlowBox(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade) :
	Gtk::FlowBox(cobject), builder(refGlade){
		S &s = S::GetInstance();
		s.signalInventoryAdd.connect(sigc::mem_fun(*this, &InventoryFlowBox::ItemAdded));
		s.signalInventoryChange.connect(sigc::mem_fun(*this, &InventoryFlowBox::ItemChanged));
	}
};

#endif