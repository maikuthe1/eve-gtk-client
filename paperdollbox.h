#ifndef PAPERDOLLBOX_H
#define PAPERDOLLBOX_H
#include "eobot/character.hpp"

class PaperdollBox : public Gtk::Box{
protected:
	Character character;
	bool local;
	// Grids
	Gtk::Grid *paperdollGrid, *accessoryGrid;

	// Boxes
	Gtk::Box *gloveCharmBox, *beltBootBox;

	// Buttons
	Gtk::Button *hatButton, *necklaceButton, *weaponButton, *armorButton, *shieldButton, *glovesButton,
				*beltButton, *ringlButton, *ringrButton, *braceletlButton, *braceletrButton, *bracerrButton,
				*bracerlButton, *charmButton, *bootsButton;

	// Images
	Gtk::Image *hatButtonImage, *necklaceButtonImage, *weaponButtonImage, *armorButtonImage, *shieldButtonImage, *glovesButtonImage,
				*beltButtonImage, *ringlButtonImage, *ringrButtonImage, *braceletlButtonImage, *braceletrButtonImage, *bracerrButtonImage,
				*bracerlButtonImage, *charmButtonImage, *bootsButtonImage;

public:
	PaperdollBox(Character owner) : Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 3), character(owner){
		S &s = S::GetInstance();
		set_margin_left(3);
		set_margin_top(3);
		set_margin_bottom(3);
		set_margin_right(3);
		local = (character.name == s.character.name);

		// Setup grid
		paperdollGrid = Gtk::manage(new Gtk::Grid());
		paperdollGrid->set_row_spacing(3);
		paperdollGrid->set_column_spacing(3);
		paperdollGrid->insert_column(0);
		paperdollGrid->insert_column(1);
		paperdollGrid->insert_column(2);
		paperdollGrid->insert_row(0);
		paperdollGrid->insert_row(1);
		paperdollGrid->insert_row(2);
		paperdollGrid->set_column_homogeneous(true);
		paperdollGrid->set_row_homogeneous(false);

		// Hat
		hatButton = Gtk::manage(new Gtk::Button());
		paperdollGrid->attach(*hatButton, 1, 0);
		hatButton->set_size_request(-1, 60);

		// Necklace
		necklaceButton = Gtk::manage(new Gtk::Button());
		paperdollGrid->attach(*necklaceButton, 2, 0);
		necklaceButton->set_valign(Gtk::ALIGN_END);

		// Weapon
		weaponButton = Gtk::manage(new Gtk::Button());
		paperdollGrid->attach(*weaponButton, 0, 1);
		paperdollGrid->get_child_at(0, 1)->set_size_request(-1, 110);

		// Armor
		armorButton = Gtk::manage(new Gtk::Button());
		paperdollGrid->attach(*armorButton, 1, 1);

		// Shield
		shieldButton = Gtk::manage(new Gtk::Button());
		paperdollGrid->attach(*shieldButton, 2, 1);

		// Glove/charm box
		gloveCharmBox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 3));
		paperdollGrid->attach(*gloveCharmBox, 0, 2);

		// Gloves
		glovesButton = Gtk::manage(new Gtk::Button());
		gloveCharmBox->pack_start(*glovesButton);
		glovesButton->set_size_request(-1, 70);

		// Charm
		charmButton = Gtk::manage(new Gtk::Button());
		gloveCharmBox->pack_start(*charmButton);
		charmButton->set_halign(Gtk::ALIGN_END);

		// Belt/boots box
		beltBootBox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 3));
		paperdollGrid->attach(*beltBootBox, 1, 2);

		// Belt
		beltButton = Gtk::manage(new Gtk::Button());
		beltBootBox->pack_start(*beltButton);

		// Boots
		bootsButton = Gtk::manage(new Gtk::Button());
		beltBootBox->pack_start(*bootsButton);
		bootsButton->set_size_request(-1, 70);

		// Accessories box
		accessoryGrid = Gtk::manage(new Gtk::Grid());
		paperdollGrid->attach(*accessoryGrid, 2, 2);
		accessoryGrid->set_row_spacing(3);
		accessoryGrid->set_column_spacing(3);
		accessoryGrid->insert_column(0);
		accessoryGrid->insert_column(1);
		accessoryGrid->insert_row(0);
		accessoryGrid->insert_row(1);
		accessoryGrid->insert_row(2);
		ringlButton = Gtk::manage(new Gtk::Button());
		accessoryGrid->attach(*ringlButton, 0, 0);
		ringrButton = Gtk::manage(new Gtk::Button());
		accessoryGrid->attach(*ringrButton, 0, 1);
		braceletlButton = Gtk::manage(new Gtk::Button());
		accessoryGrid->attach(*braceletlButton, 0, 2);
		braceletrButton = Gtk::manage(new Gtk::Button());
		accessoryGrid->attach(*braceletrButton, 1, 0);
		bracerlButton = Gtk::manage(new Gtk::Button());
		accessoryGrid->attach(*bracerlButton, 1, 1);
		bracerrButton = Gtk::manage(new Gtk::Button());
		accessoryGrid->attach(*bracerrButton, 1, 2);
		pack_start(*paperdollGrid);

		// Images


		show_all();
	};
};

#endif