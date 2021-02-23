#include "handlers.hpp"
#include "../singleton.hpp"
#include "../eodata.hpp"

// Server opened message box
void Message_Box(PacketReader reader)
{
    S &s = S::GetInstance();
	std::string title = reader.GetBreakString();
	std::string message = reader.GetBreakString();

	s.signalMessageBoxOpened.emit(title, message);
}

