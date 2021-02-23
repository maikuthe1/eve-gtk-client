#include "loginwindow.h"

LoginWindow::LoginWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade) :
	Gtk::Window(cobject), builder(refGlade){
	set_title("Login");
	S &s = S::GetInstance();
	
	// Entries
	builder->get_widget("AccountNameEntry", accountNameEntry);
	builder->get_widget("PasswordEntry", passwordEntry);
	builder->get_widget("IPEntry", IPEntry);
	builder->get_widget("PortEntry", portEntry);
	
	// Buttons
	builder->get_widget("LoginButton", loginButton);
	
	// Check Buttons
	builder->get_widget("RememberCheckButton", rememberCheckButton);
	
	// Stacks
	builder->get_widget("LoginStack", loginStack);
	
	// Boxes
	builder->get_widget("LoginBox", loginBox);
	builder->get_widget("CharacterSelectionBox", characterSelectionBox);
	
	// Signals
	rememberCheckButton->signal_toggled().connect(sigc::mem_fun(*this, &LoginWindow::RememberCheckButtonToggled));
	s.signalConnectionReply.connect(sigc::mem_fun(*this, &LoginWindow::ConnectionReply));
	s.signalLoginReply.connect(sigc::mem_fun(*this, &LoginWindow::LoginResult));
	s.signalStateChanged.connect(sigc::mem_fun(*this, &LoginWindow::StateChanged));
	loginButton->signal_clicked().connect(sigc::mem_fun(*this, &LoginWindow::LoginButtonClicked));
	
	
	gioSettings = Gio::Settings::create("org.mt1.EOCmdr.Preferences");
	gtkSettings = Gtk::Settings::get_for_screen(Gdk::Screen::get_default());
	
	// Apply settings on start
	gtkSettings->property_gtk_theme_name() = "Materia-dark-compact";
	gtkSettings->property_gtk_icon_theme_name() = "ZorinGreen-Dark";
	gtkSettings->property_gtk_font_name() = "Pixel Operator Normal 12";
	gtkSettings->property_gtk_application_prefer_dark_theme() = true;
	IPEntry->set_text(gioSettings->get_string("ip"));
	portEntry->set_text(gioSettings->get_string("port"));
	rememberCheckButton->set_active(gioSettings->get_boolean("remember-account"));
	accountNameEntry->set_text(gioSettings->get_string("account-name"));
	passwordEntry->set_text(gioSettings->get_string("password"));
	RememberCheckButtonToggled();
	
	// Bindings
	gioSettings->bind("ip", IPEntry->property_text());
	gioSettings->bind("port", portEntry->property_text());
	gioSettings->bind("remember-account", rememberCheckButton->property_active());
	
}

void LoginWindow::StateChanged(EOClient::State newState){
	if(newState == EOClient::State::Playing){
		loginStack->set_visible_child(*loginBox);
		close();
		//MainWindow* wnd = nullptr;
		//builder->get_widget_derived("MainWindow", wnd);
		//wnd->show();
	}
	
}

void LoginWindow::LoginResult(LoginReply reply, Glib::ustring message){
	S &s = S::GetInstance();
	if(reply == LoginReply::OK){
		for(unsigned int i = 0; i < s.eoclient.account.characters.size(); ++i)
		{
			CharacterSelectPreviewBox* characterPreview = Gtk::manage(new CharacterSelectPreviewBox(Gtk::Orientation::ORIENTATION_VERTICAL, 3, i));
			characterSelectionBox->pack_start(*characterPreview, true, true);
			characterSelectionBox->show_all();
			characterSelectionBox->show_all_children();
		}
		characterSelectionBox->set_visible(true);
		loginStack->set_visible_child(*characterSelectionBox);
	}
	else{
		Gtk::MessageDialog dialog("Login failed", false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_OK);
		dialog.set_secondary_text(message);
		dialog.set_default_response(Gtk::RESPONSE_OK);
		dialog.run();
	}
}

void LoginWindow::ConnectionReply(InitReply reply, Glib::ustring reason){
	S &s = S::GetInstance();
	if(reply == InitReply::OK){
		s.eoclient.LoginRequest(accountNameEntry->get_text(), passwordEntry->get_text());
	}
	else{
		Gtk::MessageDialog dialog("Connection failed", false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_OK);
		dialog.set_secondary_text(reason);
		dialog.set_default_response(Gtk::RESPONSE_OK);
		dialog.run();
	}
}

void LoginWindow::LoginButtonClicked(){
	S &s = S::GetInstance();
	if(s.eoclient.Connect()){
		s.eoclient.RequestInit();
	}
	else{
		Gtk::MessageDialog dialog("Connection failed", false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_OK);
		dialog.set_secondary_text("Could not connect to server.");
		dialog.set_default_response(Gtk::RESPONSE_OK);
		dialog.run();
	}
}

void LoginWindow::RememberCheckButtonToggled(){
	if(rememberCheckButton->get_active()){
		gioSettings->bind("account-name", accountNameEntry->property_text());
		gioSettings->bind("password", passwordEntry->property_text());
	}
	else{
		gioSettings->set_string("account-name", "");
		gioSettings->set_string("password", "");
		//gioSettings->("account-name");
		//gioSettings->unbind("password");
	}
}