#include "handlers.hpp"
#include "../singleton.hpp"
#include <iostream>

void Paperdoll_Report(PacketReader reader){
	S &s = S::GetInstance();
	std::string name = reader.GetBreakString(); // Name
	for(Character character : s.map.characters){
		if(character.name == name){
			character.SetHome(reader.GetBreakString());
			character.SetPartner(reader.GetBreakString());
			character.SetTitle(reader.GetBreakString());
			character.SetGuild(reader.GetBreakString());
			character.SetGuildRank(reader.GetBreakString());
			character.SetClass(reader.GetShort());
			character.SetGender(reader.GetChar());
			reader.GetInt(); //0
			for(int i = 0; i < character.paperdoll.size(); i++){
				character.paperdoll[i] = reader.GetShort();
			}
			for(int i = 0; i < character.cosmetic_paperdoll.size(); i++){
				character.cosmetic_paperdoll[i] = reader.GetShort();
			}
			for(int i = 0; i < character.preview_paperdoll.size(); i++){
				character.preview_paperdoll[i] = reader.GetShort();
			}

			character.SetIcon(static_cast<PaperdollIcon>(reader.GetChar()));
			break;
		}
	}
}

void Paperdoll_Reply(PacketReader reader){
	S &s = S::GetInstance();
	Paperdoll paperdoll = Paperdoll();
	paperdoll.name = reader.GetBreakString();
	paperdoll.home = reader.GetBreakString();
	paperdoll.partner = reader.GetBreakString();
	paperdoll.title = reader.GetBreakString();
	paperdoll.guild = reader.GetBreakString();
	paperdoll.guildRank = reader.GetBreakString();
	paperdoll.id = reader.GetShort();
	paperdoll.clas = reader.GetChar();
	paperdoll.gender = reader.GetChar();
	reader.GetChar(); // 0
	for(int i = 0; i < paperdoll.paperdoll_data.size(); i++){
		paperdoll.paperdoll_data[i] = reader.GetShort();
		if(paperdoll.id == s.character.id)
			s.character.paperdoll[i] = paperdoll.paperdoll_data[i];
	}
	paperdoll.icon = static_cast<PaperdollIcon>(reader.GetChar());
	s.signalPaperdollOpened.emit(paperdoll);
}

void Paperdoll_Agree(PacketReader reader){
	S &s = S::GetInstance();
	unsigned short playerID = reader.GetShort();
	AvatarSlot slot = (AvatarSlot)reader.GetChar();
	reader.GetChar(); // Sound

	// Paperdoll data: BAHWS
	// These are graphics!
	unsigned short boots = reader.GetShort();
	unsigned short armor = reader.GetShort();
	unsigned short helmet = reader.GetShort();
	unsigned short weapon = reader.GetShort();
	unsigned short sword = reader.GetShort();

	unsigned short itemID = reader.GetShort();
	unsigned int amount = reader.GetThree();
	unsigned char subLoc = reader.GetChar();
	s.inventory.SetAmount(itemID, amount);
	// Later
	/*reply.AddShort(character->maxhp);
	reply.AddShort(character->maxtp);
	reply.AddShort(character->display_str);
	reply.AddShort(character->display_intl);
	reply.AddShort(character->display_wis);
	reply.AddShort(character->display_agi);
	reply.AddShort(character->display_con);
	reply.AddShort(character->display_cha);
	reply.AddShort(character->mindam);
	reply.AddShort(character->maxdam);
	reply.AddShort(character->accuracy);
	reply.AddShort(character->evade);
	reply.AddShort(character->armor);*/
}

void Paperdoll_Remove(PacketReader reader){
	S &s = S::GetInstance();
	unsigned short playerID = reader.GetShort();
	AvatarSlot slot = (AvatarSlot)reader.GetChar();
	reader.GetChar(); // Sound

	// Paperdoll data: BAHWS
	// These are graphics!
	unsigned short boots = reader.GetShort();
	unsigned short armor = reader.GetShort();
	unsigned short helmet = reader.GetShort();
	unsigned short weapon = reader.GetShort();
	unsigned short sword = reader.GetShort();

	unsigned short itemID = reader.GetShort();
	//unsigned int amount = reader.GetThree();
	unsigned char subLoc = reader.GetChar();
	s.inventory.AddItem(itemID, 1);
	// Later
	/*reply.AddShort(character->maxhp);
	reply.AddShort(character->maxtp);
	reply.AddShort(character->display_str);
	reply.AddShort(character->display_intl);
	reply.AddShort(character->display_wis);
	reply.AddShort(character->display_agi);
	reply.AddShort(character->display_con);
	reply.AddShort(character->display_cha);
	reply.AddShort(character->mindam);
	reply.AddShort(character->maxdam);
	reply.AddShort(character->accuracy);
	reply.AddShort(character->evade);
	reply.AddShort(character->armor);*/
}