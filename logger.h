#ifndef LOGGER_H
#define LOGGER_H
#include <gtkmm.h>
#include <vector>
#include <map>
#include "logrow.h"

// Singleton
class Logger
{
	static Logger* ms_instance;

	// (list)Boxes
	Gtk::Box *logBox;
	Gtk::ListBox *logListBox;

	// Buttons
	Gtk::Button *logPreview;
	Gtk::Button *logClearButton;
	Gtk::Button *logToggleButton;
	Gtk::ToggleButton *logInfoToggleButton;
	Gtk::ToggleButton *logWarningToggleButton;
	Gtk::ToggleButton *logErrorToggleButton;
	Gtk::ToggleButton *logPacketToggleButton;

	// Labels
	Gtk::Label *logPreviewLabel;
	Gtk::Label *logPreviewTime;

	// Images
	Gtk::Image *logPreviewImage;

	// Scroll windows
	Gtk::ScrolledWindow* logScrollWindow;

	// Signals
	void logToggleButtonClicked();
	void logPreviewClicked();
	void logClearButtonClicked();

	Glib::RefPtr<Gtk::Builder> builder;
	std::vector<LogRow*> logItems;
	std::vector<Glib::RefPtr<Glib::Binding>> logBindings;
	void ScrollToBottom();

public:
	static Logger* Instance();
	void SetUp(const Glib::RefPtr<Gtk::Builder>& refGlade);
	static void Release();
	void Log(Glib::ustring logText, LogTypes logType = LogTypes::Info);

private:
	Logger();
	~Logger();

};

#endif // LOGGER_H
