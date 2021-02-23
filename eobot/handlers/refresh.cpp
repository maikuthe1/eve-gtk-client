#include <iostream>
#include <sstream>
#include <windows.h>
#include "handlers.hpp"
#include "../singleton.hpp"
#include <windows.h>
#include <fstream>
#include <sys/timeb.h>



void Refresh_Reply(PacketReader reader)
{
	S &s = S::GetInstance();

	unsigned char player_amount = reader.GetChar();
	reader.GetByte(); // 255


	std::vector<Character> update_characters;
	std::string players[player_amount];
	for(int i = 0; i < player_amount; ++i)
	{
		Character *character;
		std::string name = reader.GetBreakString();
		if(name == s.character.name)
		{
			character = &s.character;
		}
		else
		{
			character = new Character();
			character->name = name;
		}
		players[i] = character->name;

		character->gameworld_id = reader.GetShort();
		reader.GetShort(); // map id
		character->x = reader.GetShort();
		character->y = reader.GetShort();
		character->direction = static_cast<Direction>(reader.GetChar());
		reader.GetChar(); // ?
		character->guild_tag = reader.GetFixedString(3);
		character->SetLevel(reader.GetChar());
		character->gender = static_cast<Gender>(reader.GetChar());
		character->hair_style = reader.GetChar();
		character->hair_color = reader.GetChar();
		character->race = static_cast<Skin>(reader.GetChar());
		character->SetMaxHP(reader.GetShort());
		character->SetHP(reader.GetShort());
		character->SetMaxTP(reader.GetShort());
		character->SetTP(reader.GetShort());

		// equipment
		unsigned short boots = reader.GetShort(); //Boots
		unsigned short unknowneq = reader.GetShort(); //?
		unsigned short unknowneq2 = reader.GetShort(); //?
		unsigned short unknowneq3 = reader.GetShort(); //?
		unsigned short armor = reader.GetShort(); //Armor
		unsigned short unknowneq4 = reader.GetShort(); //?
		unsigned short hat = reader.GetShort(); //Hat
		unsigned short shield= reader.GetShort(); //Shield
		unsigned short weapon = reader.GetShort(); //Weapon
		character->sitting = static_cast<SitState>(reader.GetChar());
		character->visibility = reader.GetChar();
		reader.GetByte(); // 255
		update_characters.push_back(*character);
		s.signalPlayerAppear(*character);
	}
	s.map.characters = update_characters;
	s.map.npcs.clear();
	Logger* logger = Logger::Instance();
	logger->Log("Refreshing (Clears &amp; refreshes all NPC's, map items and players)");
	while(reader.PeekByte() != 255)
	{
		NPC npc;
		npc.gameworld_index = reader.GetChar();
		npc.id = reader.GetShort();
		npc.x = reader.GetChar();
		npc.y = reader.GetChar();
		npc.direction = static_cast<Direction>(reader.GetChar());
		npc.data = shared_ptr<ENF_Data>(new ENF_Data(s.enf->Get(npc.id)));

		s.map.npcs.push_back(npc);
	}
	reader.GetByte(); // 255

	s.map.items.clear();
	while(reader.Remaining() != 0){
		unsigned short uid = reader.GetShort();
		unsigned short id = reader.GetShort();
		unsigned char x = reader.GetChar(), y = reader.GetChar();
		unsigned int amount = reader.GetThree();

		Map_Item item(uid, id, amount, x, y);
		item.data = shared_ptr<EIF_Data>(new EIF_Data(s.eif->Get(id)));
		s.map.items.push_back(item);
	}

	// TODO: read items on the map
}
