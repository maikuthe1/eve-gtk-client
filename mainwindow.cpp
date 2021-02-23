#include "mainwindow.h"

MainWindow::MainWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade) :
	Gtk::Window(cobject), builder(refGlade){
	S &s = S::GetInstance();
	// Widgets
	builder->get_widget("MapScrolledWindow", mapScroll);
	scrollVAdjustment = mapScroll->get_vadjustment();
	scrollHAdjustment = mapScroll->get_hadjustment();
	builder->get_widget_derived("LoginWindow", loginWindow);
	builder->get_widget_derived("ChatBox", chatBox);
	builder->get_widget("PlayersScrolledWindow", playersScrolledWindow);
	builder->get_widget("LocalBox", localBox);
	builder->get_widget("OnlineBox", onlineBox);
	builder->get_widget("CharacterBox", characterBox);
	builder->get_widget("TitleLabel", titleLabel);
	builder->get_widget("MinimapScrolledWindow", minimapScrolledWindow);
	builder->get_widget("MapGrid", mapGrid);
	builder->get_widget("LocalExpander", localExpander);
	builder->get_widget("PartyExpander", partyExpander);
	builder->get_widget("OnlineExpander", onlineExpander);
	builder->get_widget("MainOverlay", mainOverlay);
	builder->get_widget("PreferencesButton", preferencesButton);
	builder->get_widget_derived("InventoryToggleButton", inventoryButton);
	builder->get_widget("LogoutButton", logoutButton);
	builder->get_widget("InventoryToggleButton", inventoryButton);
	//builder->get_widget_derived("SpellsToggleButton", spellsToggleButton);
	builder->get_widget_derived("ChatToggleButton", chatToggleButton);
	builder->get_widget_derived("CharacterToggleButton", characterToggleButton);
	builder->get_widget_derived("PlayersToggleButton", playersToggleButton);
	builder->get_widget_derived("InventoryScrolledWindow", inventoryScrolledWindow);
	builder->get_widget_derived("MapToggleButton", mapToggleButton);
	builder->get_widget("MapImage", mapImage);
	builder->get_widget("GLArea", glArea);
	builder->get_widget("LogBox", logBox);
	builder->get_widget("CharacterGrid", characterGrid);
	builder->get_widget_derived("PreferencesPopover", preferencesPopover);

	chatWindow = new DraggableBox((Gtk::Window*)this, mainOverlay, chatBox, "Chat", "chat-symbolic", 375, 350);
	playersWindow = new DraggableBox((Gtk::Window*)this, mainOverlay, playersScrolledWindow, "Players", "players-symbolic", 250, 500);
	mapWindow = new DraggableBox((Gtk::Window*)this, mainOverlay, minimapScrolledWindow, "Map", "globe-symbolic", 250, 250);
	logWindow = new DraggableBox((Gtk::Window*)this, mainOverlay, logBox, "Log", "globe-symbolic", 500, 300);
	inventoryWindow = new DraggableBox((Gtk::Window*)this, mainOverlay, inventoryScrolledWindow, "Inventory", "", 500, 300, Gtk::ORIENTATION_HORIZONTAL, false);
	characterWindow = new DraggableBox((Gtk::Window*)this, mainOverlay, characterBox, "Character", "stats-symbolic", 0, 0);

	loginWindow->show();

	mapScroll->add_events(Gdk::BUTTON3_MOTION_MASK | Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK);
	mapScroll->signal_button_press_event().connect(sigc::mem_fun(*this, &MainWindow::MousePressed), true);
	mapScroll->signal_button_release_event().connect(sigc::mem_fun(*this, &MainWindow::MouseReleased), true);
	mapScroll->signal_motion_notify_event().connect(sigc::mem_fun(*this, &MainWindow::MouseMoved));
	s.signalPlayerAppear.connect(sigc::mem_fun(*this, &MainWindow::PlayerAppear));
	s.signalPlayerConnect.connect(sigc::mem_fun(*this, &MainWindow::PlayerConnect));
	s.signalPlayerRemove.connect(sigc::mem_fun(*this, &MainWindow::PlayerRemove));
	s.signalPlayerDisconnect.connect(sigc::mem_fun(*this, &MainWindow::PlayerDisconnect));
	s.signalStateChanged.connect(sigc::mem_fun(*this, &MainWindow::StateChanged));
	s.signalPartyInvite.connect(sigc::mem_fun(*this, &MainWindow::PartyInvite));
	s.signalPartyCreate.connect(sigc::mem_fun(*this, &MainWindow::PartyCreate));
	s.signalPartyClose.connect(sigc::mem_fun(*this, &MainWindow::PartyClose));
	s.signalMessageBoxOpened.connect(sigc::mem_fun(*this, &MainWindow::MessageBoxOpened));
	preferencesButton->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::PreferencesButtonClicked));
	logoutButton->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::LogoutButtonClicked));
	//inventoryButton->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::LogoutButtonClicked));



	// Set up right click menus (These should go in their own class)
	playerRightClickMenu = Gtk::manage(new Gtk::Menu());
	playerRightClickMenu->set_reserve_toggle_size(false);
	Gtk::Box* itemBox = Gtk::manage(new Gtk::Box());
	Gtk::Image* icon = Gtk::manage(new Gtk::Image());
	icon->set_from_icon_name("user-available-symbolic", Gtk::ICON_SIZE_MENU);
	Gtk::Label* label = Gtk::manage(new Gtk::Label("Message"));
	itemBox->pack_start(*icon);
	itemBox->pack_start(*label);
	Gtk::MenuItem *item = Gtk::manage(new Gtk::MenuItem(*itemBox));
	item->signal_activate().connect(sigc::mem_fun(*chatBox, &ChatBox::StartChat));
	playerRightClickMenu->append(*item);

	itemBox = Gtk::manage(new Gtk::Box());
	icon = Gtk::manage(new Gtk::Image());
	label = Gtk::manage(new Gtk::Label());
	icon->set_from_icon_name("media-playlist-shuffle-symbolic", Gtk::ICON_SIZE_MENU);
	label->set_text("Trade");
	itemBox->pack_start(*icon);
	itemBox->pack_start(*label);
	item = Gtk::manage(new Gtk::MenuItem(*itemBox));
	playerRightClickMenu->append(*item);

	itemBox = Gtk::manage(new Gtk::Box());
	icon = Gtk::manage(new Gtk::Image());
	label = Gtk::manage(new Gtk::Label());
	icon->set_from_icon_name("network-transmit-symbolic", Gtk::ICON_SIZE_MENU);
	label->set_text("Invite");
	itemBox->pack_start(*icon);
	itemBox->pack_start(*label);
	item = Gtk::manage(new Gtk::MenuItem(*itemBox));
	item->signal_activate().connect(sigc::bind<unsigned char>(sigc::mem_fun(*this, &MainWindow::InviteJoin), 1));
	playerRightClickMenu->append(*item);

	itemBox = Gtk::manage(new Gtk::Box());
	icon = Gtk::manage(new Gtk::Image());
	label = Gtk::manage(new Gtk::Label());
	icon->set_from_icon_name("network-receive-symbolic", Gtk::ICON_SIZE_MENU);
	label->set_text("Join");
	itemBox->pack_start(*icon);
	itemBox->pack_start(*label);
	item = Gtk::manage(new Gtk::MenuItem(*itemBox));
	item->signal_activate().connect(sigc::bind<unsigned char>(sigc::mem_fun(*this, &MainWindow::InviteJoin), 0));
	playerRightClickMenu->append(*item);
	playerRightClickMenu->show_all();

	inventoryButton->SetLinkWidget(inventoryWindow);
	playersToggleButton->SetLinkWidget(playersWindow);
	chatToggleButton->SetLinkWidget(chatWindow);
	mapToggleButton->SetLinkWidget(mapWindow);
	characterToggleButton->SetLinkWidget(characterWindow);

	// Apply settings on start
	gioPrefs = Gio::Settings::create("org.mt1.EOCmdr.Preferences");
	gioSettings = Gio::Settings::create("org.mt1.EOCmdr.Settings");
	gtkSettings = Gtk::Settings::get_for_screen(Gdk::Screen::get_default());
	chatWindow->set_visible(gioSettings->get_boolean("chat-visible"));
	gioSettings->bind("chat-visible", chatWindow->property_visible());
	inventoryWindow->set_visible(gioSettings->get_boolean("inventory-visible"));
	gioSettings->bind("inventory-visible", inventoryWindow->property_visible());
	characterWindow->set_visible(gioSettings->get_boolean("character-visible"));
	gioSettings->bind("character-visible", characterWindow->property_visible());
	mapWindow->set_visible(gioSettings->get_boolean("map-visible"));
	gioSettings->bind("map-visible", mapWindow->property_visible());
	playersWindow->set_visible(gioSettings->get_boolean("players-visible"));
	gioSettings->bind("players-visible", playersWindow->property_visible());

	Logger* logger = Logger::Instance();
	logger->Log("Welcome to EO Commander!");


	Glib::RefPtr<Gdk::PixbufLoader> loader = Gdk::PixbufLoader::create();
	Atlas::BitmapInfo sprite = s.atlas.GetSprite("gfx023.egf", 652);
	//Atlas::BitmapInfo sprite = s.atlas.GetSprite("gfx002.egf", 1);
	loader->write(sprite.bytes, sprite.size);
	loader->close();
	Glib::RefPtr<Gdk::Pixbuf> pixBuf = loader->get_pixbuf();
	unsigned char* pixels = new unsigned char[pixBuf->get_byte_length()];
	pixels = pixBuf->get_pixels();
	std::ofstream stream("output.bmp", std::ios::binary | std::ios::out);
	stream.write((char*)pixels, pixBuf->get_byte_length());
	stream.close();


	int channels;
	int rowstride;
	channels = (true) ? 4 : 3;
	rowstride = sprite.width * channels;
	/* Always align rows to 32-bit boundaries */
	rowstride = (rowstride + 3) & ~3;

	unsigned char* newPixels = new unsigned char[rowstride * sprite.height];


	int padding = rowstride - (sprite.width * 4);
	printf("\n\nOriginal pixbuf data: BPP %i, Stride: %i\nNew pixbuf data: Stride: %i, Padding: %i", pixBuf->get_bits_per_sample(), pixBuf->get_rowstride(), rowstride, padding);

	unsigned int column = 0;
	int cbyte = 0;
	int con = 0;
	for(int i = 0; i < rowstride * sprite.height; i++){
		if(column == 3){
			column = 0;
			if(pixels[cbyte] == 0 && pixels[cbyte - 1] == 0 && pixels[cbyte - 2] == 0)
				newPixels[i] = 0;
			else
				newPixels[i] = 255;
		}
		else{
			newPixels[i] = pixels[cbyte++];
			column++;
		}
	}

	Glib::RefPtr<Gdk::Pixbuf> pixBuf2 = Gdk::Pixbuf::create_from_data(newPixels, Gdk::COLORSPACE_RGB, true, 8, sprite.width, sprite.height, rowstride);

	DraggableBox *draggy = new DraggableBox((Gtk::Window*)this, mainOverlay, new Gtk::Image(pixBuf2), "test", "chat-symbolic", 0, 0);
}

void MainWindow::InviteJoin(unsigned char type){
	S &s = S::GetInstance();
	s.eoclient.PartyInvite(type, (unsigned short)selectedCharacter.gameworld_id);
}

void MainWindow::PartyInvite(unsigned char type, unsigned short id, std::string name){
	// // //// /// /// /// // /// /////// Make a delete function id draggablebox, save the memory!!!
	//inviteWindow = new DraggableBox((Gtk::Window*)this, mainOverlay, inviteBox, "Party invite from PLAYER_NAME", "network-transmit-symbolic", 30, 120);
	PartyInviteBox* invWin = Gtk::manage(new PartyInviteBox(type, id, name));
	Glib::ustring text;
	if(type == 1)
		text = "Party invite from " + name;
		else
			text = name + " would like to join your party";
	DraggableBox* inviteWindow = Gtk::manage(new DraggableBox((Gtk::Window*)this, mainOverlay, invWin, text, "network-transmit-symbolic", 30, 120));
}

void MainWindow::PartyCreate(){
	partyBox = Gtk::manage(new PartyBox());
	partyExpander->add(*partyBox);
}

void MainWindow::PartyClose(){
	std::vector<Gtk::Widget*> children = partyBox->get_children();
	for(unsigned int i = 0; i < children.size(); i++)
		partyBox->remove(*children[i]);
	delete(partyBox);
	//DraggableBox* partyWindow = Gtk::manage(new DraggableBox((Gtk::Window*)this, mainOverlay, new PartyBox(), "Party", "party-symbolic", 100, 300));
	//partyBox = Gtk::manage(new PartyBox());
	//partyExpander->add(*partyBox);
}

void MainWindow::MessageBoxOpened(std::string title, std::string message){
	EOMessageBox *newMessageBox = Gtk::manage(new EOMessageBox(title, message));
	DraggableBox *newDraggable = new DraggableBox((Gtk::Window*)this, mainOverlay, newMessageBox, title, "chat-symbolic", 250, 269);
}

void MainWindow::ToggleButtonClicked(Gtk::Widget *widget){
	widget->set_visible(!widget->get_visible());
}

void MainWindow::PreferencesButtonClicked(){
	preferencesPopover->popup();
}
void MainWindow::LogoutButtonClicked(){
	S &s = S::GetInstance();
	s.eoclient.Disconnect();
}

void MainWindow::StateChanged(EOClient::State newState){
	S &s = S::GetInstance();
	if(newState == EOClient::State::Uninitialized){
		builder->get_widget_derived("LoginWindow", loginWindow);
		//overlay->set_visible(true);
		loginWindow->show();
	}
	if(newState == EOClient::State::Playing){
		std::vector<Gtk::Widget*> children = localBox->get_children();
		for(unsigned int i = 0; i < children.size(); i++){
			localBox->remove(*children[i]);
		}
		children = onlineBox->get_children();
		for(unsigned int i = 0; i < children.size(); i++){
			onlineBox->remove(*children[i]);
		}
		s.eoclient.RefreshRequest();
		s.onlineList.clear();
		s.eoclient.RequestPlayerList();
		builder->get_widget_derived("StatusFrame", statusFrame);
		titleLabel->set_text(Glib::ustring::compose("[%1] %2 | Lvl. %3 | HP %4\% | TP %5\%", s.character.guild_tag, s.character.name, s.character.GetLevel(), (int)((double)s.character.GetHP() / (double)s.character.GetMaxHP() * 100), (int)((double)s.character.GetTP() / (double)s.character.GetMaxTP() * 100)));
		set_title(Glib::ustring::compose("%1 - 3hrs 10min TNL", s.character.name));
		//overlay->set_visible(false);
	}
}

void MainWindow::PlayerDisconnect(Character character){
	std::vector<Gtk::Widget*> children = onlineBox->get_children();
	for(unsigned int i = 0; i < children.size(); i++){
		OnlinePlayerBox* onlinePlayerBox = (OnlinePlayerBox*)children[i];
		if(onlinePlayerBox->GetCharacter().name == character.name){ // FIX ME // FIX ME // FIX ME (character should be a pointer!)
			onlineBox->remove(*onlinePlayerBox);
			break;
		}
	}
	onlineBox->show_all();
	children = onlineBox->get_children();
	onlineExpander->set_label(Glib::ustring::compose("Online [%1]", children.size()));
}

void MainWindow::PlayerConnect(Character character){
	if((int)character.admin_level > 0 || character.GetIcon() == PaperdollIcon::GM || character.GetIcon() == PaperdollIcon::HGM){
		if (adminSoundBuffer.loadFromFile("GUI/admin.ogg")){
			adminSound.setBuffer(adminSoundBuffer);
			adminSound.play();
		}
	}
	OnlinePlayerBox* onlinePlayerBox = Gtk::manage(new OnlinePlayerBox(character));
	onlineBox->pack_start(*onlinePlayerBox);
	onlineBox->show_all();
	std::vector<Gtk::Widget*> children = onlineBox->get_children();
	onlineExpander->set_label(Glib::ustring::compose("Online [%1]", children.size()));
	onlineBox->show_all();
}

void MainWindow::PlayerRemove(Character character){
	std::vector<Gtk::Widget*> children = localBox->get_children();
	for(unsigned int i = 0; i < children.size(); i++){
		LocalPlayerEventBox* eventBox = (LocalPlayerEventBox*)children[i];
		if(eventBox->GetLocalPlayerFrame()->GetCharacter().name == character.name){ // FIX ME // FIX ME // FIX ME (character should be a pointer!)
			localBox->remove(*eventBox);
			break;
		}
	}
	localBox->show_all();
	children = localBox->get_children();
	localExpander->set_label(Glib::ustring::compose("Near Me [%1]", children.size()));
}

void MainWindow::PlayerAppear(Character character){
	S &s = S::GetInstance();
	if(character.name.compare(s.character.name) == 0)
		return;
	std::vector<Gtk::Widget*> children = localBox->get_children();
	for(unsigned int i = 0; i < children.size(); i++){
		LocalPlayerEventBox* eventBox = (LocalPlayerEventBox*)children[i];
		if(eventBox->GetLocalPlayerFrame()->GetCharacter().name == character.name){ // FIX ME // FIX ME // FIX ME (character should be a pointer!)
			return;
		}
	}
	LocalPlayerFrame* newCharacter = Gtk::manage(new LocalPlayerFrame(character));
	LocalPlayerEventBox* newEventBox = Gtk::manage(new LocalPlayerEventBox());
	newEventBox->add(*newCharacter);
	newEventBox->SetLocalPlayerFrame(newCharacter);
	localBox->pack_start(*newEventBox);
	localBox->show_all();
	children = localBox->get_children();
	localExpander->set_label(Glib::ustring::compose("Near Me [%1]", children.size()));
	newEventBox->add_events(Gdk::BUTTON_PRESS_MASK);
	newEventBox->signal_button_press_event().connect(sigc::bind<LocalPlayerFrame*>(sigc::mem_fun(*this, &MainWindow::PlayerClicked), newCharacter), false);
}

bool MainWindow::PlayerClicked(GdkEventButton* event, LocalPlayerFrame* playerFrame){
	if(event->button == 3){
		selectedCharacter = playerFrame->GetCharacter();
		chatBox->SetSelectedCharacter(playerFrame->GetCharacter());
		playerRightClickMenu->popup_at_pointer((GdkEvent*)event);
	}
	return false;
}

bool MainWindow::MousePressed(GdkEventButton* event){
	if(event->button == 3){
		mouseDown = true;
		previousY = event->y;
		previousX = event->x;
		previousVPosition = scrollVAdjustment->get_value();
		previousHPosition = scrollHAdjustment->get_value();
	}
	return true;
}

bool MainWindow::MouseReleased(GdkEventButton* event){
	mouseDown = false;
	return true;
}

bool MainWindow::MouseMoved(GdkEventMotion* event){
	if(!mouseDown)
		return true;
	int offsetx, offsety;
	int currentY = event->y;
	int currentX = event->x;
	int YMovement = (previousY - currentY);
	int XMovement = (previousX - currentX);
	int scrollYPosition = (previousVPosition + YMovement);
	int scrollXPosition = (previousHPosition + XMovement);
	scrollVAdjustment->set_value(scrollYPosition);
	scrollHAdjustment->set_value(scrollXPosition);
	std::vector<unsigned int> padding;
	return true;
}















/* Image transparency stuff
Atlas::BitmapInfo sprite = s.atlas.GetSprite("gfx015.egf", 403);

	Glib::RefPtr<Gdk::PixbufLoader> loader = Gdk::PixbufLoader::create();
	loader->write(sprite.bytes, sprite.size);
	// Declare another BMP image
	EasyBMP::BMP AnotherImage;
	// Read from a file
	AnotherImage.ReadFromData(sprite.bytes);
	AnotherImage.SetBitDepth(24);
	std::vector<unsigned char> newimg = AnotherImage.WriteToFile("poop2.bmp");
	printf("\n\n\n\nNewimg size: %i", newimg.size());
	unsigned char nnn[newimg.size()];
	for(int i = 0; i < newimg.size(); i++)
		nnn[i] = newimg[i];

unsigned char rawData[79] = {
	0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0x00, 0x00, 0x65, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x6D, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x69,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0x72, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00
};


	Glib::RefPtr<Gdk::Pixbuf> pixBuf = Gdk::Pixbuf::create_from_data(rawData, Gdk::COLORSPACE_RGB, true, 8, 5, 5, 16);//loader->get_pixbuf();
	printf("\n\n\n\nStride: %i", pixBuf->get_bits_per_sample());
	loader->close();
	char* pixels = new char[100];
	long long unsigned int sizee = 100;
	printf("\n\n\n\n\n\n\nByte length: %i", pixBuf->get_bits_per_sample());
	DraggableBox *draggy = new DraggableBox((Gtk::Window*)this, mainOverlay, new Gtk::Image(pixBuf), "test", "chat-symbolic", 0, 0);
	//unsigned char* pixels = new unsigned char[pixBuf->get_byte_length()];
	//= pixBuf->get_pixels();


	std::ofstream stream("output.bmp", std::ios::binary | std::ios::out);
	stream.write((char*)nnn, sizeof(nnn));
	stream.close();

	int stride = 30*24;  // bits per row
	stride += 31;            // round up to next 32-bit boundary
	stride /= 32;            // DWORDs per row
	stride *= 4;             // bytes per row

	*/