

#include "handlers.hpp"
#include "../singleton.hpp"
#include "../eodata.hpp"
#include "../character.hpp"
#include <windows.h>

void Welcome_Reply(PacketReader reader)
{

	S &s = S::GetInstance();
	int sub_id = reader.GetShort();
	if(sub_id != 1 && sub_id != 2)
	{
		// Couldn't use the character at this moment??
		return;
	}

	if(sub_id == 1)
	{
		s.character.gameworld_id = reader.GetShort();
		unsigned int character_id = reader.GetInt();
		if(character_id != s.character.id)
		{
			// Character couldn't be loaded
			return;
		}

		short map_id = reader.GetShort();
		s.emf = shared_ptr<EMF>(new EMF(map_id));
		if(s.emf->exists)
		{
			// Map loaded
			s.map.Reset();
		}
		else
		{
			// Map couldn't be loaded
		}
		s.map.id = map_id;
		std::array<unsigned char, 4> map_rid;
		PacketReader reader_copy(reader);
		std::string remaining_data = reader_copy.GetEndString();
		if(remaining_data[0] == 0xFF)
		{
			reader.GetByte(); // 0xFF
			reader.GetByte(); // PK information
		}
		else
		{
			map_rid[0] = reader.GetByte();
			map_rid[1] = reader.GetByte();
		}

		map_rid[2] = reader.GetByte();
		map_rid[3] = reader.GetByte();
		reader.GetThree(); // map file size

		std::array<unsigned char, 4> pub_rid;
		std::array<unsigned char, 2> pub_len;

		for(int i = 0; i < 4; ++i)
		{
			pub_rid[0] = reader.GetByte();
			pub_rid[1] = reader.GetByte();
			pub_rid[2] = reader.GetByte();
			pub_rid[3] = reader.GetByte();
			pub_len[0] = reader.GetByte();
			pub_len[1] = reader.GetByte();
		}

		s.character.name = reader.GetBreakString();
		s.character.title = reader.GetBreakString();
		s.character.guild = reader.GetBreakString();
		s.character.guild_rank = reader.GetBreakString();
		s.character.clas = reader.GetChar();
		s.character.guild_tag = reader.GetFixedString(3);
		s.character.admin_level = static_cast<AdminLevel>(reader.GetChar());

		s.character.SetLevel(reader.GetChar());
		s.character.SetEXP(reader.GetInt());
		s.character.SetUsage(reader.GetInt());
		s.character.SetHP(reader.GetShort());
		s.character.SetMaxHP(reader.GetShort());
		s.character.SetTP(reader.GetShort());
		s.character.SetMaxTP(reader.GetShort());
		s.character.SetMaxSP(reader.GetShort());
		s.character.SetStatPoints(reader.GetShort());
		s.character.SetSkillPoints(reader.GetShort());
		s.character.SetKarma(reader.GetShort());
		s.character.SetMinDamage(reader.GetShort());
		s.character.SetMaxDamage(reader.GetShort());
		s.character.SetAccuracy(reader.GetShort());
		s.character.SetEvade(reader.GetShort());
		s.character.SetArmor(reader.GetShort());

		s.character.SetStr(reader.GetShort());
		s.character.SetWis(reader.GetShort());
		s.character.SetInt(reader.GetShort());
		s.character.SetAgi(reader.GetShort());
		s.character.SetCon(reader.GetShort());
		s.character.SetCha(reader.GetShort());
//Papaerdoll 0: Ancient Boots

//Papaerdoll 1: War Charm

//Papaerdoll 2: Death Gloves

//Papaerdoll 3: Leather Belt

//Papaerdoll 4: Oron

//Papaerdoll 5: Emerald Necklace

//Papaerdoll 6: Helmet of Darkness

//Papaerdoll 7: Fire Arrows

//Papaerdoll 8: Twin Blades

//Papaerdoll 9: Golden Ring

//Papaerdoll 10:

//Papaerdoll 11: Golden Elf Braclet

//Papaerdoll 12:

//Papaerdoll 13: Leather Bracer

//Papaerdoll 14:
		for(unsigned int i = 0; i < 15; i++){ // Paperdoll length is 15
			unsigned short itemID = reader.GetShort();
			EIF_Data itemData = s.eif->Get(itemID);
		}


		PacketBuilder packet(PacketFamily::Welcome, PacketAction::Message);
		packet.AddThree(1);
		packet.AddInt(s.character.id);
		s.eoclient.Send(packet);
	}
	else if(sub_id == 2)
	{
		reader.GetByte(); // 255

		for (int i = 0; i < 9; ++i)
		{
			reader.GetBreakString();
		}

		s.character.weight = reader.GetChar();
		s.character.max_weight = reader.GetChar();

		// inventory items
		s.inventory.Clear();
		while(reader.PeekByte() != 255)
		{
			short item_id = reader.GetShort();
			int item_amount = reader.GetInt();
			s.inventory.AddItem(item_id, item_amount);
		}
		reader.GetByte(); // 255

		while(reader.PeekByte() != 255)
		{
			reader.GetShort(); // spell id
			reader.GetShort(); // spell level
		}
		reader.GetByte(); // 255

		unsigned char player_amount = reader.GetChar();
		reader.GetByte(); // 255
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
			for(int pi = 0; pi < 9; ++pi)
			{
				reader.GetShort(); // item id
			}

			character->sitting = static_cast<SitState>(reader.GetChar());
			character->visibility = reader.GetChar();
			reader.GetByte(); // 255

			s.map.characters.push_back(*character);
			s.signalPlayerAppear(*character);
		}

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
		reader.GetByte();

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

		s.eoclient.UnregisterHandler(PacketFamily::Login, PacketAction::Reply);
		s.eoclient.UnregisterHandler(PacketFamily::Welcome, PacketAction::Reply);
		s.eoclient.RegisterHandler(PacketFamily::Players, PacketAction::Agree, Players_Agree);
		s.eoclient.RegisterHandler(PacketFamily::Avatar, PacketAction::Remove, Avatar_Remove);
		s.eoclient.RegisterHandler(PacketFamily::Walk, PacketAction::Player, Walk_Player);
		s.eoclient.RegisterHandler(PacketFamily::Face, PacketAction::Player, Face_Player);
		s.eoclient.RegisterHandler(PacketFamily::Walk, PacketAction::Reply, Walk_Reply);
		s.eoclient.RegisterHandler(PacketFamily::Talk, PacketAction::Player, Talk_Player);
		s.eoclient.RegisterHandler(PacketFamily::Talk, PacketAction::Tell, Talk_Tell);
		s.eoclient.RegisterHandler(PacketFamily::Talk, PacketAction::Message, Talk_Message);
		s.eoclient.RegisterHandler(PacketFamily::Talk, PacketAction::Request, Talk_Guild);
		s.eoclient.RegisterHandler(PacketFamily::Talk, PacketAction::Open, Talk_Party);
		s.eoclient.RegisterHandler(PacketFamily::Sit, PacketAction::Player, Sit_Player);
		s.eoclient.RegisterHandler(PacketFamily::Sit, PacketAction::Close, Sit_Close);
		s.eoclient.RegisterHandler(PacketFamily::Trade, PacketAction::Request, Trade_Request);
		s.eoclient.RegisterHandler(PacketFamily::Trade, PacketAction::Open, Trade_Open);
		s.eoclient.RegisterHandler(PacketFamily::Trade, PacketAction::Close, Trade_Close);
		s.eoclient.RegisterHandler(PacketFamily::Trade, PacketAction::Reply, Trade_Reply);
		s.eoclient.RegisterHandler(PacketFamily::Trade, PacketAction::Spec, Trade_Spec);
		s.eoclient.RegisterHandler(PacketFamily::Trade, PacketAction::Agree, Trade_Agree);
		s.eoclient.RegisterHandler(PacketFamily::Trade, PacketAction::Use, Trade_Use);
		s.eoclient.RegisterHandler(PacketFamily::Emote, PacketAction::Player, Emote_Player);
		s.eoclient.RegisterHandler(PacketFamily::Appear, PacketAction::Reply, Appear_Reply);
		s.eoclient.RegisterHandler(PacketFamily::NPC, PacketAction::Spec, NPC_Spec);
		s.eoclient.RegisterHandler(PacketFamily::NPC, PacketAction::Junk, NPC_Junk);
		s.eoclient.RegisterHandler(PacketFamily::NPC, PacketAction::Player, NPC_Player);
		s.eoclient.RegisterHandler(PacketFamily::Refresh, PacketAction::Reply, Refresh_Reply);
		s.eoclient.RegisterHandler(PacketFamily::Item, PacketAction::Drop, Item_Drop);
		s.eoclient.RegisterHandler(PacketFamily::Item, PacketAction::Add, Item_Add);
		s.eoclient.RegisterHandler(PacketFamily::Warp, PacketAction::Request, Warp_Request);
		s.eoclient.RegisterHandler(PacketFamily::Item, PacketAction::Reply, Item_Reply); // used item
		s.eoclient.RegisterHandler(PacketFamily::Effect, PacketAction::Spec, Effect_Spec);
		s.eoclient.RegisterHandler(PacketFamily::Item, PacketAction::Get, Item_Get);
		s.eoclient.RegisterHandler(PacketFamily::Item, PacketAction::Remove, Item_Remove);
		s.eoclient.RegisterHandler(PacketFamily::Shop, PacketAction::Buy, Shop_Buy);
		s.eoclient.RegisterHandler(PacketFamily::Talk, PacketAction::Reply, Talk_Reply);
		s.eoclient.RegisterHandler(PacketFamily::Party, PacketAction::Request, Party_Request);
		s.eoclient.RegisterHandler(PacketFamily::Party, PacketAction::Create, Party_Create);
		s.eoclient.RegisterHandler(PacketFamily::Party, PacketAction::Add, Party_Add);
		s.eoclient.RegisterHandler(PacketFamily::Party, PacketAction::Remove, Party_Remove);
		s.eoclient.RegisterHandler(PacketFamily::Party, PacketAction::Close, Party_Close);
		s.eoclient.RegisterHandler(PacketFamily::Party, PacketAction::List, Party_List);
		s.eoclient.RegisterHandler(PacketFamily::Party, PacketAction::Agree, Party_Agree);
		s.eoclient.RegisterHandler(PacketFamily::Paperdoll, PacketAction::Agree, Paperdoll_Agree);
		s.eoclient.RegisterHandler(PacketFamily::Paperdoll, PacketAction::Remove, Paperdoll_Remove);
		s.eoclient.RegisterHandler(PacketFamily::Message, PacketAction::Accept, Message_Box);

		//s.eoclient.RegisterHandler(PacketFamily::Recover, PacketAction::Agree, Recover_Agree); // non local player heals

		s.eoclient.SetState(EOClient::State::Playing);
	}
}
