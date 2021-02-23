#include "handlers.hpp"
#include "../singleton.hpp"
#include "../util.hpp"
#include <string>
#include <sstream>
#include <windows.h>
#include "commands.hpp"
#include <fstream>
#include <iostream>

void NPC_Player(PacketReader reader)
{
    S &s = S::GetInstance();
	unsigned int length = reader.Length();

	if(length == 9) // Npc walk
	{
		unsigned char index = reader.GetChar();
		unsigned char x = reader.GetChar();
		unsigned char y = reader.GetChar();
		Direction direction = static_cast<Direction>(reader.GetChar());
		reader.GetByte(); // 255
		reader.GetByte(); // 255
		reader.GetByte(); // 255

		for(unsigned int i = 0; i < s.map.npcs.size(); i++)
		{
			if(s.map.npcs[i].gameworld_index == index){
				s.map.npcs[i].direction = direction;
				s.map.npcs[i].x = x;
				s.map.npcs[i].y = y;
			}
		}
	}

	if(length == 20) // Npc attack
	{
		reader.GetByte(); // 255
		unsigned char index = reader.GetChar(); // index
		unsigned char unk1 = reader.GetChar(); // builder.AddChar(1 + (target->hp == 0));
		Direction direction = static_cast<Direction>(reader.GetChar()); // direction
		unsigned short playerID = reader.GetShort(); // playerid
		unsigned int amount  = reader.GetThree(); // amount
		unsigned int percent = reader.GetThree(); // perctentage ?? builder.AddThree(util::clamp<int>(double(target->hp) / double(target->maxhp) * 100.0, 0, 100));
		reader.GetByte(); // 255
		reader.GetByte(); // 255

		// If the attacked player is not the local player, packet ends here

		if(s.character.gameworld_id == playerID) // Attacked player is us
		{
			unsigned short hp = reader.GetShort(); // target hp
			unsigned short mp = reader.GetShort(); // target mp
			if(amount > 0){
				s.character.SetHP(hp);
			}
		}
	}
}


// Should go in a different file
void Effect_Spec(PacketReader reader)
{
	S &s = S::GetInstance();
	unsigned int length = reader.Length();
	unsigned char type = reader.GetChar(); // 2 for spikes??

	if(type == (unsigned char) 2) // spikes
	{
		unsigned short amount = reader.GetShort();
		unsigned short hp = reader.GetShort();
		unsigned short maxHp = reader.GetShort();

		s.character.SetHP(hp);
		s.character.SetMaxHP(maxHp);
	}
}
