#ifndef PREFERENCESPOPOVER_H
#define PREFERENCESPOPOVER_H
#include <gtkmm.h>
#include "logger.h"

class PreferencesPopover : public Gtk::Popover
{
	
	// Buttons
	Gtk::Button* aboutButton;
	Gtk::Button *logPreview;
	Gtk::FontButton *fontSelect;
	
	// Check buttons
	Gtk::CheckButton* showRecentLogCheck;
	Gtk::CheckButton* useAnimationsCheck;
	Gtk::CheckButton* darkModeCheck;
	
	// Text combos
	Gtk::ComboBoxText* themeCombo;
	Gtk::ComboBoxText* iconThemeCombo;
	
	// Adjustments
	Glib::RefPtr<Gtk::Adjustment> windowTransparencyAdjustment;
	
	// Scales
	Gtk::Scale *windowTransparencyScale;
	
	// Bindings
	Glib::RefPtr<Glib::Binding> showLogInStatusbarBind;
	Glib::RefPtr<Glib::Binding> useAnimationBinding;
	Glib::RefPtr<Glib::Binding> themeBinding;
	Glib::RefPtr<Glib::Binding> iconThemeBinding;
	Glib::RefPtr<Glib::Binding> fontBinding;
	Glib::RefPtr<Glib::Binding> darkModeBinding;
	
	// Settings
	Glib::RefPtr<Gio::Settings> gioSettings;
	Glib::RefPtr<Gtk::Settings> gtkSettings;
		
	// Signals
	void aboutButtonClicked();
	void dialogResponse(int responseID);
	
	Logger* logger;
	Glib::RefPtr<Gtk::Builder> builder;
	
public:
	PreferencesPopover(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& refGlade);
	

};

#endif // PREFERENCESPOPOVER_H
