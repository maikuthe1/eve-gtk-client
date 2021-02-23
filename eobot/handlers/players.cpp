
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
	short boots = reader.GetShort(); //Boots
	short unknowneq = reader.GetShort(); //?
	short unknowneq2 = reader.GetShort(); //?
	short unknowneq3 = reader.GetShort(); //?
	short armor = reader.GetShort(); //Armor
	short unknowneq4 = reader.GetShort(); //?
	short hat = reader.GetShort(); //Hat
	short shield= reader.GetShort(); //Shield
	short weapon = reader.GetShort(); //Weapon
	//std::stringstream paperdollstream;
	//paperdollstream << hat << "," << weapon << "," << armor << "," << shield << "," << boots;
	//character.paperdoll = paperdollstream.str();
	character.paperdoll[EquipLocation::Boots] = boots;
	character.paperdoll[EquipLocation::Armor] = armor;
	character.paperdoll[EquipLocation::Hat] = hat;
	character.paperdoll[EquipLocation::Shield] = shield;
	character.paperdoll[EquipLocation::Weapon] = weapon;
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
