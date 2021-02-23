/*
 * EO Commander by Maiku
 * Don't distribute, alright?
 * */
#include <gtkmm-3.0/gtkmm.h>
#include "mainwindow.h"
#include "loginwindow.h"
#include "eobot/singleton.hpp"
#include <string>
#include <random>
#include <iostream>
#include <chrono>
#include "eobot/singleton.hpp"
#include "eobot/astar.hpp"
sf::Clock reconnect_clock;
sf::Clock init_clock;
bool reconnect = true;
bool firstRun = true;
void initialize_data_handlers()
{
	S &s = S::GetInstance();

	s.config.Load("./config.ini");
	s.eif = shared_ptr<EIF>(new EIF("./pub/dat001.eif"));
	s.enf = shared_ptr<ENF>(new ENF("./pub/dtn001.enf"));
	s.esf = shared_ptr<ESF>(new ESF("./pub/dsl001.esf"));
	s.ecf = shared_ptr<ECF>(new ECF("./pub/dat001.ecf"));

	s.eoclient.controllers = s.eoclient.Explode(s.config.GetValue("Controllers"), ",");

	printf("data handlers initialized\nWelcome to EOBot2! - ");
}

bool mainEO()
{
	S &s = S::GetInstance();
	SetConsoleTextAttribute(s.hConsole, 15);
	s.eoclient.Tick();
	if(s.eoclient.GetState() == EOClient::State::Playing)
	{
			s.eoclient.Tick();
	}

	if(s.call_exit)
	{
		if(s.eoclient.Connected())
		{
			s.eoclient.Disconnect();
		}
	}

	return true;
}


int main(int argc, char** argv)
{
	S &s = S::GetInstance();

	Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "org.mt1.EOCmdr");
	auto builder = Gtk::Builder::create_from_file("GUI/gui.glade");
	Logger* logger = Logger::Instance();
	logger->SetUp(builder);
	initialize_data_handlers();
	sigc::slot<bool> tslot = sigc::ptr_fun(&mainEO);
	Glib::signal_timeout().connect(tslot, 1);
	// Apply CSS
	Glib::RefPtr<Gtk::CssProvider> cssProvider = Gtk::CssProvider::create();
	cssProvider->load_from_path("GUI/css/dark.css");
	Glib::RefPtr<Gtk::StyleContext> styleContext = Gtk::StyleContext::create();
	Glib::RefPtr<Gdk::Screen> screen = Gdk::Screen::get_default();
	styleContext->add_provider_for_screen(screen, cssProvider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

	LoginWindow* lwnd = nullptr;
	MainWindow* wnd = nullptr;
	builder->get_widget_derived("MainWindow", wnd);
	// Run the appliaction
	auto r = app->run(*wnd);

	delete lwnd;
	delete wnd;
	return r;
}
