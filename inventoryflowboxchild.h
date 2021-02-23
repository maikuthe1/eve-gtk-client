#ifndef INVENTORYFLOWBOXCHILD_H
#define INVENTORYFLOWBOXCHILD_H
#include <gtkmm.h>
#include "eobot/singleton.hpp"


class InventoryFlowBoxChild : public Gtk::FlowBoxChild{
protected:
	short cItemID;
	Gtk::EventBox* evBox;
	Gtk::Overlay *overlay;
	Gtk::Image *itemImage;
	Gtk::Label *amountLabel;
	
	bool DoubleClick(GdkEventButton* event){ // Could also connect to child activated in flowbox
		S &s = S::GetInstance();
		if(event->type == Gdk::DOUBLE_BUTTON_PRESS && event->button == 1){
			EIF_Data itemData = s.eif->Get((unsigned int)cItemID);
			switch(itemData.type){
				case EIF_Data::Type::Weapon:
					s.eoclient.Equip(cItemID, 0);
					break;
				case EIF_Data::Type::Shield:
					s.eoclient.Equip(cItemID, 0);
					break;
				case EIF_Data::Type::Armor:
					s.eoclient.Equip(cItemID, 0);
					break;
				case EIF_Data::Type::Hat:
					s.eoclient.Equip(cItemID, 0);
					break;
				case EIF_Data::Type::Boots:
					s.eoclient.Equip(cItemID, 0);
					break;
				case EIF_Data::Type::Gloves:
					s.eoclient.Equip(cItemID, 0);
					break;
				case EIF_Data::Type::Accessory:
					s.eoclient.Equip(cItemID, 0);
					break;
				case EIF_Data::Type::Belt:
					s.eoclient.Equip(cItemID, 0);
					break;
				case EIF_Data::Type::Necklace:
					s.eoclient.Equip(cItemID, 0);
					break;
				case EIF_Data::Type::Ring:
					s.eoclient.Equip(cItemID, 0); // TODO equip different slots
					break;
				case EIF_Data::Type::Armlet:
					s.eoclient.Equip(cItemID, 0); // TODO equip different slots
					break;
				case EIF_Data::Type::Bracer:
					s.eoclient.Equip(cItemID, 0); // TODO equip different slots
					break;
				default:
					s.eoclient.UseItem(cItemID);
					break;
			}
		}
		
		return false;
	}
	
	void ItemChanged(short itemID, short amount){
		if(itemID == cItemID)
			amountLabel->set_text(Glib::ustring::compose("%1", amount));
	}
	
public:
	InventoryFlowBoxChild(short itemID, int amount){
		cItemID = itemID;
		S &s = S::GetInstance();
		evBox = Gtk::manage(new Gtk::EventBox());
		overlay = Gtk::manage(new Gtk::Overlay());
		
		EIF_Data itemData = s.eif->Get((unsigned int)itemID);
		Glib::RefPtr<Gdk::PixbufLoader> loader = Gdk::PixbufLoader::create();
		Atlas::BitmapInfo sprite = s.atlas.GetSprite("gfx023.egf", (int)itemData.graphic * 2);
		loader->write(sprite.bytes, sprite.size);
		loader->close();
		Glib::RefPtr<Gdk::Pixbuf> pixBuf = loader->get_pixbuf();
		itemImage = Gtk::manage(new Gtk::Image(pixBuf));
		Glib::ustring tooltipFormat = Glib::ustring::compose("<span font_weight=\"bold\">%1</span>", itemData.name);
		if(itemData.hp > 0)
			tooltipFormat += Glib::ustring::compose("\rHP: %1", itemData.hp);
		if(itemData.tp > 0)
			tooltipFormat += Glib::ustring::compose("\rTP: %1", itemData.tp);
		if(itemData.str > 0)
			tooltipFormat += Glib::ustring::compose("\rStr: %1", itemData.str);
		if(itemData.mindam > 0 || itemData.maxdam > 0)
			tooltipFormat += Glib::ustring::compose("\rDmg: %1-%2", itemData.mindam, itemData.maxdam);
		if(itemData.wis > 0)
			tooltipFormat += Glib::ustring::compose("\rWis: %1", itemData.wis);
		if(itemData.accuracy > 0)
			tooltipFormat += Glib::ustring::compose("\rAcc: %1", itemData.accuracy);
		if(itemData.con > 0)
			tooltipFormat += Glib::ustring::compose("\rCon: %1", itemData.con);
		if(itemData.armor > 0)
			tooltipFormat += Glib::ustring::compose("\rDef: %1", itemData.armor);
		if(itemData.intl > 0)
			tooltipFormat += Glib::ustring::compose("\rInt: %1", itemData.intl);
		if(itemData.evade > 0)
			tooltipFormat += Glib::ustring::compose("\rEva: %1", itemData.evade);
		if(itemData.agi> 0)
			tooltipFormat += Glib::ustring::compose("\rAgi: %1", itemData.agi);
		if(itemData.cha > 0)
			tooltipFormat += Glib::ustring::compose("\rCha: %1", itemData.cha);
		if(itemData.type == EIF_Data::Type::Teleport){
			tooltipFormat += Glib::ustring::compose("\rMap: %1", itemData.scrollmap);
			tooltipFormat += Glib::ustring::compose("\rX: %1", itemData.scrollx);
			tooltipFormat += Glib::ustring::compose("\rY: %1", itemData.scrolly);
		}
		tooltipFormat += Glib::ustring::compose("\rID: %1", itemData.id);
		set_tooltip_markup(tooltipFormat);
		overlay->add_overlay(*itemImage);
		
		amountLabel = Gtk::manage(new Gtk::Label(std::to_string(amount)));
		amountLabel->set_valign(Gtk::ALIGN_START);
		amountLabel->set_halign(Gtk::ALIGN_START);
		overlay->add_overlay(*amountLabel);
		
		//overlay->set_halign(Gtk::ALIGN_FILL);
		//overlay->set_valign(Gtk::ALIGN_FILL);
		evBox->add(*overlay);
		add(*evBox);
		set_size_request(sprite.width, sprite.height);
		show_all();
		
		evBox->add_events(Gdk::BUTTON_PRESS_MASK);
		evBox->signal_button_press_event().connect(sigc::mem_fun(*this, &InventoryFlowBoxChild::DoubleClick));
		s.signalInventoryChange.connect(sigc::mem_fun(*this, &InventoryFlowBoxChild::ItemChanged));
	}
	
	short GetItemID(){
		return cItemID;
	}
};


#endif