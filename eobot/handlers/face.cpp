#include "handlers.hpp"
#include "../singleton.hpp"
#include "../util.hpp"
#include <string>
#include <sstream>
#include <windows.h>
#include "commands.hpp"
#include <fstream>
#include <iostream>

void Face_Player(PacketReader reader)
{
	S &s = S::GetInstance();

	short gameworld_id = reader.GetShort();
	int i = s.map.GetCharacterIndex(gameworld_id);
	s.map.characters[i].direction = static_cast<Direction>(reader.GetChar());
}
