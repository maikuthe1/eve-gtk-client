#include "draggablebox.h"

DraggableBox::DraggableBox(Gtk::Window* mainWin, Gtk::Overlay *overlay, Gtk::Widget* content, Glib::ustring title, Glib::ustring iconName, unsigned int width, unsigned int height, Gtk::Orientation orientation, bool showCloseButton, bool permanent){
	//std::vector<Gtk::Widget*> children = get_children();
	mainOverlay = overlay;
	win = mainWin;

	gioSettings = Gio::Settings::create("org.mt1.EOCmdr.Preferences");
	gtkSettings = Gtk::Settings::get_for_screen(Gdk::Screen::get_default());

	 gioSettings->bind("window-transparency", property_opacity());

	// The grid
	mainGrid = Gtk::manage(new Gtk::Grid());
	mainGrid->set_size_request(width, height);
	mainGrid->insert_row(0);
	mainGrid->insert_row(1);
	mainGrid->insert_row(2);
	mainGrid->insert_column(0);
	mainGrid->insert_column(1);
	mainGrid->insert_column(2);
	mainGrid->set_column_homogeneous(false);
	mainGrid->set_row_homogeneous(false);

	topSeparatorEventBox = Gtk::manage(new Gtk::EventBox());
	topSeparator = Gtk::manage(new Gtk::Separator(Gtk::ORIENTATION_HORIZONTAL));
	topSeparatorEventBox->add(*topSeparator);
	topSeparator->set_size_request(-1, 3);

	bottomSeparatorEventBox = Gtk::manage(new Gtk::EventBox());
	bottomSeparator = Gtk::manage(new Gtk::Separator(Gtk::ORIENTATION_HORIZONTAL));
	bottomSeparatorEventBox->add(*bottomSeparator);
	bottomSeparator->set_size_request(-1, 3);

	rightSeparatorEventBox = Gtk::manage(new Gtk::EventBox());
	rightSeparator = Gtk::manage(new Gtk::Separator(Gtk::ORIENTATION_VERTICAL));
	rightSeparatorEventBox->add(*rightSeparator);
	rightSeparator->set_size_request(3, -1);

	leftSeparatorEventBox = Gtk::manage(new Gtk::EventBox());
	leftSeparator = Gtk::manage(new Gtk::Separator(Gtk::ORIENTATION_VERTICAL));
	leftSeparatorEventBox->add(*leftSeparator);
	leftSeparator->set_size_request(3, -1);

	tlSeparatorEventBox = Gtk::manage(new Gtk::EventBox());
	tlSeparator = Gtk::manage(new Gtk::Separator(Gtk::ORIENTATION_VERTICAL));
	tlSeparatorEventBox->add(*tlSeparator);
	tlSeparator->set_size_request(3, 3);

	trSeparatorEventBox = Gtk::manage(new Gtk::EventBox());
	trSeparator = Gtk::manage(new Gtk::Separator(Gtk::ORIENTATION_VERTICAL));
	trSeparatorEventBox->add(*trSeparator);
	trSeparator->set_size_request(3, 3);

	blSeparatorEventBox = Gtk::manage(new Gtk::EventBox());
	blSeparator = Gtk::manage(new Gtk::Separator(Gtk::ORIENTATION_VERTICAL));
	blSeparatorEventBox->add(*blSeparator);
	blSeparator->set_size_request(3, 3);

	brSeparatorEventBox = Gtk::manage(new Gtk::EventBox());
	brSeparator = Gtk::manage(new Gtk::Separator(Gtk::ORIENTATION_VERTICAL));
	brSeparatorEventBox->add(*brSeparator);
	brSeparator->set_size_request(3, 3);

	mainGrid->attach(*topSeparatorEventBox, 1,0);
	mainGrid->attach(*rightSeparatorEventBox, 2,1);
	mainGrid->attach(*bottomSeparatorEventBox, 1,2);
	mainGrid->attach(*leftSeparatorEventBox, 0,1);
	mainGrid->attach(*tlSeparatorEventBox, 0,0);
	mainGrid->attach(*trSeparatorEventBox, 2,0);
	mainGrid->attach(*blSeparatorEventBox, 0,2);
	mainGrid->attach(*brSeparatorEventBox, 2,2);

	// Title
	titleEventBox = Gtk::manage(new Gtk::EventBox());
	titleBox = Gtk::manage(new Gtk::Box((orientation == Gtk::ORIENTATION_HORIZONTAL) ? Gtk::ORIENTATION_VERTICAL : Gtk::ORIENTATION_HORIZONTAL, 3));
	icon = Gtk::manage(new Gtk::Image());
	icon->set_from_icon_name(iconName, Gtk::ICON_SIZE_SMALL_TOOLBAR);
	titleBox->pack_start(*icon);
	titleLabel = Gtk::manage(new Gtk::Label(title));
	if(orientation == Gtk::ORIENTATION_HORIZONTAL)
		titleLabel->set_angle(90);
	titleBox->pack_start(*titleLabel);
	titlebar = Gtk::manage(new Gtk::HeaderBar());
	Glib::RefPtr<Gtk::StyleContext> headerContext = titlebar->get_style_context();
	headerContext->add_class("draggableHeader");
	titlebar->set_custom_title(*titleBox);
	titleEventBox->add(*titlebar);

	// Content
	contentBox = Gtk::manage(new Gtk::Box(orientation));
	contentBox->pack_start(*titleEventBox, false, false);
	contentBox->pack_start(*content, true, true);
	mainGrid->attach(*contentBox, 1,1);
	titlebar->set_vexpand(false);
	content->set_vexpand(true);
	content->set_hexpand(true);
	contentBox->set_homogeneous(false);
	Glib::RefPtr<Gtk::StyleContext> mainContext = contentBox->get_style_context();
	mainContext->add_class("window");


	if(showCloseButton){
		closeButton = Gtk::manage(new Gtk::Button("X"));
		titlebar->pack_end(*closeButton);
		Glib::RefPtr<Gtk::StyleContext> context = closeButton->get_style_context();
		context->add_class("flat");
		context->add_class("closeButton");
		closeButton->signal_clicked().connect(sigc::mem_fun(*this, &DraggableBox::Hide));
	}
	add_events(Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK);
	signal_button_press_event().connect(sigc::mem_fun(*this, &DraggableBox::MouseDown), false);
	signal_button_press_event().connect(sigc::mem_fun(*this, &DraggableBox::MouseDown), false);
	titleEventBox->signal_button_press_event().connect(sigc::mem_fun(*this, &DraggableBox::MouseDown), false);
	titleEventBox->signal_button_release_event().connect(sigc::mem_fun(*this, &DraggableBox::MouseUp), false);
	titleEventBox->signal_motion_notify_event().connect(sigc::mem_fun(*this, &DraggableBox::MouseMoved), false);
	bottomSeparatorEventBox->signal_motion_notify_event().connect(sigc::bind<Gtk::Widget*>(sigc::mem_fun(*this, &DraggableBox::BorderDrag), bottomSeparator), false);
	bottomSeparatorEventBox->signal_enter_notify_event().connect(sigc::bind<Gtk::Widget*>(sigc::mem_fun(*this, &DraggableBox::CrossBorder), bottomSeparator), false);
	rightSeparatorEventBox->signal_motion_notify_event().connect(sigc::bind<Gtk::Widget*>(sigc::mem_fun(*this, &DraggableBox::BorderDrag), rightSeparator), false);
	rightSeparatorEventBox->signal_enter_notify_event().connect(sigc::bind<Gtk::Widget*>(sigc::mem_fun(*this, &DraggableBox::CrossBorder), rightSeparator), false);
	leftSeparatorEventBox->signal_motion_notify_event().connect(sigc::bind<Gtk::Widget*>(sigc::mem_fun(*this, &DraggableBox::BorderDrag), leftSeparator), false);
	leftSeparatorEventBox->signal_enter_notify_event().connect(sigc::bind<Gtk::Widget*>(sigc::mem_fun(*this, &DraggableBox::CrossBorder), leftSeparator), false);
	topSeparatorEventBox->signal_motion_notify_event().connect(sigc::bind<Gtk::Widget*>(sigc::mem_fun(*this, &DraggableBox::BorderDrag), topSeparator), false);
	topSeparatorEventBox->signal_enter_notify_event().connect(sigc::bind<Gtk::Widget*>(sigc::mem_fun(*this, &DraggableBox::CrossBorder), topSeparator), false);
	tlSeparatorEventBox->signal_motion_notify_event().connect(sigc::bind<Gtk::Widget*>(sigc::mem_fun(*this, &DraggableBox::BorderDrag), tlSeparator), false);
	tlSeparatorEventBox->signal_enter_notify_event().connect(sigc::bind<Gtk::Widget*>(sigc::mem_fun(*this, &DraggableBox::CrossBorder), tlSeparator), false);
	trSeparatorEventBox->signal_motion_notify_event().connect(sigc::bind<Gtk::Widget*>(sigc::mem_fun(*this, &DraggableBox::BorderDrag), trSeparator), false);
	trSeparatorEventBox->signal_enter_notify_event().connect(sigc::bind<Gtk::Widget*>(sigc::mem_fun(*this, &DraggableBox::CrossBorder), trSeparator), false);
	blSeparatorEventBox->signal_motion_notify_event().connect(sigc::bind<Gtk::Widget*>(sigc::mem_fun(*this, &DraggableBox::BorderDrag), blSeparator), false);
	blSeparatorEventBox->signal_enter_notify_event().connect(sigc::bind<Gtk::Widget*>(sigc::mem_fun(*this, &DraggableBox::CrossBorder), blSeparator), false);
	brSeparatorEventBox->signal_motion_notify_event().connect(sigc::bind<Gtk::Widget*>(sigc::mem_fun(*this, &DraggableBox::BorderDrag), brSeparator), false);
	brSeparatorEventBox->signal_enter_notify_event().connect(sigc::bind<Gtk::Widget*>(sigc::mem_fun(*this, &DraggableBox::CrossBorder), brSeparator), false);

	add(*mainGrid);
	mainOverlay->add_overlay(*this);
	show_all();
	set_halign(Gtk::ALIGN_START);
	set_valign(Gtk::ALIGN_START);

	signal_show().connect(sigc::mem_fun(*this, &DraggableBox::OnShow));
}

void DraggableBox::OnShow(){
	mainOverlay->reorder_overlay(*this, mainOverlay->get_children().size() - 1);
}

void DraggableBox::Hide(){
	if(permanent)
		hide();
	else
		delete this;
}

bool DraggableBox::BorderDrag(GdkEventMotion* event, Gtk::Widget* widget){
	int currentSizeW, currentSizeH;
	mainGrid->get_size_request(currentSizeW, currentSizeH);
	Gtk::Widget* windowTitlebar = win->get_titlebar();
	int windowTitlebarHeight = windowTitlebar->get_height();
	currentSizeH = currentSizeH;
	if(widget == bottomSeparator){
		mainGrid->set_size_request(currentSizeW, currentSizeH + event->y);
	}
	if(widget == rightSeparator){
		mainGrid->set_size_request(currentSizeW + event->x, currentSizeH);
	}
	if(widget == leftSeparator){
		mainGrid->set_size_request(currentSizeW - event->x, currentSizeH);
		set_margin_left(get_margin_left() + event->x);
	}
	if(widget == topSeparator){
		mainGrid->set_size_request(currentSizeW, currentSizeH - event->y);
		set_margin_top(get_margin_top() + event->y);
	}
	if(widget == brSeparator){
		mainGrid->set_size_request(currentSizeW + event->x, currentSizeH + event->y);
	}
	if(widget == trSeparator){
		mainGrid->set_size_request(currentSizeW + event->x, currentSizeH - event->y);
		set_margin_top(get_margin_top() + event->y);
	}
	if(widget == tlSeparator){
		mainGrid->set_size_request(currentSizeW - event->x, currentSizeH - event->y);
		set_margin_top(get_margin_top() + event->y);
		set_margin_left(get_margin_left() + event->x);
	}
	if(widget == blSeparator){
		mainGrid->set_size_request(currentSizeW - event->x, currentSizeH + event->y);
		set_margin_left(get_margin_left() + event->x);
	}
	return false;
}
bool DraggableBox::CrossBorder(GdkEventCrossing* event, Gtk::Widget* widget){
	Glib::RefPtr<Gdk::Window> www = win->get_root_window();
	Glib::RefPtr<Gdk::Display> display = win->get_display();
	Glib::RefPtr<Gdk::Cursor> cursor;
	if(event->type == Gdk::ENTER_NOTIFY){
		if(widget == topSeparator)
		 cursor =  Gdk::Cursor::create(display, "n-resize");
		if(widget == trSeparator)
		 cursor =  Gdk::Cursor::create(display, "ne-resize");
		if(widget == rightSeparator)
		 cursor =  Gdk::Cursor::create(display, "e-resize");
		if(widget == brSeparator)
		 cursor =  Gdk::Cursor::create(display, "se-resize");
		if(widget == bottomSeparator)
		 cursor =  Gdk::Cursor::create(display, "d-resize");
		if(widget == blSeparator)
		 cursor =  Gdk::Cursor::create(display, "sw-resize");
		if(widget == leftSeparator)
		 cursor =  Gdk::Cursor::create(display, "w-resize");
		if(widget == trSeparator)
		 cursor =  Gdk::Cursor::create(display, "nw-resize");
		www->set_cursor(cursor);
	}
	if(event->type == Gdk::LEAVE_NOTIFY){
		cursor =  Gdk::Cursor::create(display, "w-resize");
	}
	return false;
}

bool DraggableBox::MouseDown(GdkEventButton* event){
	if (event->button == 1) {
		mainOverlay->reorder_overlay(*this, mainOverlay->get_children().size() - 1);
		Gtk::Widget* windowTitlebar = win->get_titlebar();
		int windowTitlebarHeight = windowTitlebar->get_height();
		win->get_position(offsetx, offsety);
		// plus distance from pointer to edge of widget
		offsetx += (int)event->x;
		offsety += (int)event->y + windowTitlebarHeight;
		maxx = RoundDownToMultiple(win->get_width() - get_allocated_width(), Sensitivity);
		maxy = RoundDownToMultiple(win->get_height() - get_allocated_height() - windowTitlebarHeight, Sensitivity);
	}
	return false;
}
bool DraggableBox::MouseUp(GdkEventButton* event){
	return false;
}
bool DraggableBox::MouseMoved(GdkEventMotion* event){
	int x = (int)event->x_root - offsetx;
	int y = (int)event->y_root - offsety;
	x = RoundToNearestMultiple(Max(Min(x, maxx), 0), Sensitivity);
	y = RoundToNearestMultiple(Max(Min(y, maxy), 0), Sensitivity);
	if (x != px || y != py) {
		px = x;
		py = y;
		set_margin_top(y);
		set_margin_left(x);
		//set_margin_right(get_allocated_width() + x);
	}
	return false;
}