
#include <iostream>
#include <sstream>
#include <windows.h>
#include "handlers.hpp"
#include "../singleton.hpp"

// Player walks in range
void Players_Agree(PacketReader reader)
{
    S &s = S::GetInstance();
    reader.GetByte(); // 255
    Character character;
    character.name = reader.GetBreakString();
    character.gameworld_id = reader.GetShort();
    reader.GetShort(); // map id
    character.x = reader.GetShort();
    character.y = reader.GetShort();
    character.direction = static_cast<Direction>(reader.GetChar());
    reader.GetChar(); // 6 (?)
    character.guild_tag = reader.GetFixedString(3);
    character.SetLevel(reader.GetChar());
    character.gender = static_cast<Gender>(reader.GetChar());
    character.hair_style = reader.GetChar();
    character.hair_color = reader.GetChar();
    character.race = static_cast<Skin>(reader.GetChar());
    character.SetMaxHP(reader.GetShort());
    character.SetHP(reader.GetShort());
    character.SetMaxTP(reader.GetShort());
    character.SetTP(reader.GetShort());

    // equipment
    std::string boots = std::to_string(reader.GetShort()); //Boots
    std::string unknowneq = std::to_string(reader.GetShort()); //?
    std::string unknowneq2 = std::to_string(reader.GetShort()); //?
    std::string unknowneq3 = std::to_string(reader.GetShort()); //?
    std::string armor = std::to_string(reader.GetShort()); //Armor
    std::string unknowneq4 = std::to_string(reader.GetShort()); //?
    std::string hat = std::to_string(reader.GetShort()); //Hat
    std::string shield= std::to_string(reader.GetShort()); //Shield
    std::string weapon = std::to_string(reader.GetShort()); //Weapon
    std::stringstream paperdollstream;
    paperdollstream << hat << "," << weapon << "," << armor << "," << shield << "," << boots;
    character.paperdoll = paperdollstream.str();
    character.sitting = static_cast<SitState>(reader.GetChar());
    character.visibility = reader.GetChar();
    reader.GetByte(); // 255
    reader.GetByte(); // 0 = NPC, 1 = player
	for(unsigned int i = 0; i < s.onlineList.size(); i++){
		if(s.onlineList[i].name == character.name){
			character.SetIcon(s.onlineList[i].GetIcon());
		}
	}
    s.map.characters.push_back(character);
	s.signalPlayerAppear.emit(character);
	
}
