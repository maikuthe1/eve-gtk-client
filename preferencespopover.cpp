#include "preferencespopover.h"

PreferencesPopover::PreferencesPopover(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade) :
	Gtk::Popover(cobject), builder(refGlade)
{
	logger = Logger::Instance();
	
	// Get objects from glade file
	builder->get_widget("aboutButton", aboutButton);
	builder->get_widget("showRecentLogCheck", showRecentLogCheck);
	builder->get_widget("useAnimationsCheck", useAnimationsCheck);
	builder->get_widget("logPreview", logPreview);
	builder->get_widget("themeComboBox", themeCombo);
	builder->get_widget("iconThemeCombo", iconThemeCombo);
	builder->get_widget("darkModeCheck", darkModeCheck);
	builder->get_widget("fontSelect", fontSelect);
	//builder->get_widget("WindowTransparencyAdjustment", windowTransparencyAdjustment);
	builder->get_widget("WindowTransparencyScale", windowTransparencyScale);
	windowTransparencyAdjustment = windowTransparencyScale->get_adjustment();
	
	// Set up signals
	aboutButton->signal_clicked().connect(sigc::mem_fun(*this, &PreferencesPopover::aboutButtonClicked));
	
	gioSettings = Gio::Settings::create("org.mt1.EOCmdr.Preferences");
	gtkSettings = Gtk::Settings::get_for_screen(Gdk::Screen::get_default());
	
	// Bind settings
	gioSettings->bind("show-log-in-statusbar", showRecentLogCheck->property_active());
	gioSettings->bind("use-animations", useAnimationsCheck->property_active());
	gioSettings->bind("icon-theme", iconThemeCombo->property_active_id());
	gioSettings->bind("dark-mode", darkModeCheck->property_active());
	gioSettings->bind("theme", themeCombo->property_active_id());
	gioSettings->bind("font", fontSelect->property_font_name());
	gioSettings->bind("window-transparency", windowTransparencyAdjustment->property_value());
	showLogInStatusbarBind = Glib::Binding::bind_property(showRecentLogCheck->property_active(), logPreview->property_visible(),  Glib::BINDING_DEFAULT);
	useAnimationBinding = Glib::Binding::bind_property(useAnimationsCheck->property_active(), gtkSettings->property_gtk_enable_animations(),  Glib::BINDING_DEFAULT);
	themeBinding = Glib::Binding::bind_property(themeCombo->property_active_id(), gtkSettings->property_gtk_theme_name(),  Glib::BINDING_DEFAULT);
	iconThemeBinding = Glib::Binding::bind_property(iconThemeCombo->property_active_id(), gtkSettings->property_gtk_icon_theme_name(),  Glib::BINDING_DEFAULT);
	fontBinding = Glib::Binding::bind_property(fontSelect->property_font_name(), gtkSettings->property_gtk_font_name(),  Glib::BINDING_DEFAULT);
	darkModeBinding = Glib::Binding::bind_property(darkModeCheck->property_active(), gtkSettings->property_gtk_application_prefer_dark_theme(),  Glib::BINDING_DEFAULT);
	
	// Apply settings on start
	logPreview->set_visible(gioSettings->get_boolean("show-log-in-statusbar"));
	gtkSettings->property_gtk_enable_animations() = gioSettings->get_boolean("use-animations");
	gtkSettings->property_gtk_theme_name() = gioSettings->get_string("theme");
	gtkSettings->property_gtk_icon_theme_name() = gioSettings->get_string("icon-theme");
	gtkSettings->property_gtk_font_name() = gioSettings->get_string("font");
	gtkSettings->property_gtk_application_prefer_dark_theme() = gioSettings->get_boolean("dark-mode");
	windowTransparencyScale->set_value(gioSettings->get_double("window-transparency"));
	windowTransparencyAdjustment->set_value(gioSettings->get_double("window-transparency"));
}

void PreferencesPopover::aboutButtonClicked(){
	
}

void PreferencesPopover::dialogResponse(int responseID){
	logger = Logger::Instance();
	
}

