#include "logger.h"
#include <cstdlib> // NULL

Logger* Logger::ms_instance = NULL;

void Logger::SetUp(const Glib::RefPtr<Gtk::Builder>& refGlade){
	builder = refGlade;

	// Get objects from glade file
	builder->get_widget("logBox", logBox);
	builder->get_widget("logListBox", logListBox);
	builder->get_widget("logPreview", logPreview);
	builder->get_widget("logClearButton", logClearButton);
	builder->get_widget("logPreviewTime", logPreviewTime);
	builder->get_widget("logPreviewLabel", logPreviewLabel);
	builder->get_widget("logPreviewImage", logPreviewImage);
	builder->get_widget("logScrollWindow", logScrollWindow);
	builder->get_widget("logToggleButton", logToggleButton);
	builder->get_widget("logInfoToggleButton", logInfoToggleButton);
	builder->get_widget("logPacketToggleButton", logPacketToggleButton);
	builder->get_widget("logErrorToggleButton", logErrorToggleButton);
	builder->get_widget("logWarningToggleButton", logWarningToggleButton);

	// Set up signals
	logToggleButton->signal_clicked().connect(sigc::mem_fun(*this, &Logger::logToggleButtonClicked));
	logPreview->signal_clicked().connect(sigc::mem_fun(*this, &Logger::logPreviewClicked));
	logClearButton->signal_clicked().connect(sigc::mem_fun(*this, &Logger::logClearButtonClicked));

	// Make log info type toggle buttons unfocusable
	logInfoToggleButton->set_focus_on_click(false);
	logWarningToggleButton->set_focus_on_click(false);
	logErrorToggleButton->set_focus_on_click(false);
	logPacketToggleButton->set_focus_on_click(false);

	Glib::RefPtr<Gio::Settings> gioSettings;
	gioSettings = Gio::Settings::create("org.mt1.EOCmdr.Settings");
	logBox->set_visible(gioSettings->get_boolean("log-visible"));
}

void Logger::Log(Glib::ustring logText, LogTypes logType){
	// Create a new LogRow object
	LogRow* newRow = new LogRow(logText, logType);

	// Add it to the list of log items
	logItems.push_back(newRow);

	// Add it to the listbox
	logListBox->append(*newRow);
	Glib::RefPtr<Glib::Binding> newBinding;
	if(logType == LogTypes::Info){
		newBinding = Glib::Binding::bind_property(logInfoToggleButton->property_active(), newRow->property_visible(),  Glib::BINDING_DEFAULT);
		newRow->set_visible(logInfoToggleButton->get_active());
	}
	if(logType == LogTypes::Warning){
		newBinding = Glib::Binding::bind_property(logWarningToggleButton->property_active(), newRow->property_visible(),  Glib::BINDING_DEFAULT);
		newRow->set_visible(logWarningToggleButton->get_active());
	}
	if(logType == LogTypes::Error){
		newBinding = Glib::Binding::bind_property(logErrorToggleButton->property_active(), newRow->property_visible(),  Glib::BINDING_DEFAULT);
		newRow->set_visible(logErrorToggleButton->get_active());
	}
	if(logType == LogTypes::Packet){
		newBinding = Glib::Binding::bind_property(logPacketToggleButton->property_active(), newRow->property_visible(),  Glib::BINDING_DEFAULT);
		newRow->set_visible(logPacketToggleButton->get_active());
	}
	logBindings.push_back(newBinding);

	// Update the log preview in statusbar
	logPreviewLabel->set_markup(newRow->GetFormattedText());
	logPreviewImage->set_from_icon_name(newRow->GetLogIcon(), Gtk::ICON_SIZE_BUTTON);
	logPreviewTime->set_text(newRow->GetLogTime());
	logPreview->set_tooltip_text(logText);
	logPreviewLabel->show_all();

	ScrollToBottom();
}

void Logger::ScrollToBottom(){
	Glib::RefPtr<Gtk::Adjustment> adj = logScrollWindow->get_vadjustment();
	adj->set_value(adj->get_upper());
}

void Logger::logToggleButtonClicked(){
	// Show/hide the log
	logBox->set_visible(!logBox->is_visible());
}

void Logger::logPreviewClicked(){
	// Show the log
	logBox->set_visible(true);

	// Select last item from log item list
	if(logItems.size() >= 1)
		logListBox->select_row(*logItems[logItems.size() - 1]);
}

void Logger::logClearButtonClicked(){
	// Delete all log items from list
	for(auto const& value: logItems) {
		delete value;
	}
	logItems.clear();
}

Logger::Logger()
{
}

Logger::~Logger()
{
}

Logger* Logger::Instance()
{
	if (ms_instance == NULL) {
		ms_instance = new Logger();
	}
	return ms_instance;
}

void Logger::Release()
{
	if (ms_instance) {
		delete ms_instance;
	}
	ms_instance = NULL;
}

