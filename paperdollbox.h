#ifndef PAPERDOLLBOX_H
#define PAPERDOLLBOX_H

class PaperdollBox : public Gtk::Box{
protected:
	bool isLocal;
	// Grids
	Gtk::Grid *paperdollGrid, *accesoryGrid;

	// Boxes
	Gtk::Box *gloveCharmBox, *beltBootBox;

	// Buttons
	Gtk::Button *hatButton, *necklaceButton, *weaponButton, *armorButton, *shieldButton, *glovesButton,
				*beltButton, *ringlButton, *ringrButton, *braceletlButton, *braceletrButton, *bracerrButton,
				*charmButton, *bootsButton;

public:
	PaperdollBox(Paperdoll paperdoll) : Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 3){
		paperdollGrid = Gtk::manage(new Gtk::Grid());
		paperdollGrid->set_row_spacing(3);
		paperdollGrid->set_column_spacing(3);
		paperdollGrid->insert_column(0);
		paperdollGrid->insert_column(1);
		paperdollGrid->insert_column(2);
		paperdollGrid->insert_row(0);
		paperdollGrid->insert_row(1);
		paperdollGrid->insert_row(2);
		pack_start(*paperdollGrid);

		show_all();
	};
};

#endif