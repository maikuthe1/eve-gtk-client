

#include "handlers.hpp"
#include "../singleton.hpp"
#include "../util.hpp"

void Walk_Player(PacketReader reader)
{
	S &s = S::GetInstance();

	short gameworld_id = reader.GetShort();
	int i = s.map.GetCharacterIndex(gameworld_id);
	if(i == -1)
		return;

	s.map.characters[i].direction = static_cast<Direction>(reader.GetChar());
	s.map.characters[i].x = reader.GetChar();
	s.map.characters[i].y = reader.GetChar();
}

void Walk_Reply(PacketReader reader)
{
	S &s = S::GetInstance();
	for(unsigned int i = 0; i < s.map.items.size(); i++){
		int dx = std::abs(s.map.items[i].x - s.character.x);
		int dy = std::abs(s.map.items[i].y - s.character.y);
		unsigned int distance = dx + dy;

		if(distance > s.eoclient.seeDistance){
			s.map.items.erase(s.map.items.begin() + i);
		}
	}
	reader.GetByte(); // 255
	reader.GetByte(); // 255
	while(reader.Remaining() != 0){
		unsigned short uid = reader.GetShort();
		unsigned short id = reader.GetShort();
		unsigned char x = reader.GetChar(), y = reader.GetChar();
		unsigned int amount = reader.GetThree();

		Map_Item item(uid, id, amount, x, y);
		item.data = shared_ptr<EIF_Data>(new EIF_Data(s.eif->Get(id)));
		s.map.items.push_back(item);
	}

	s.character.x = s.character.newX;
	s.character.y = s.character.newY;
}

void Warp_Request(PacketReader reader)
{
	S &s = S::GetInstance();

	WarpType type = (WarpType)reader.GetChar();
	unsigned short mapID = reader.GetShort();

	s.eoclient.RefreshRequest();

	if(type == WarpType::WARP_LOCAL){
		printf("\nWe warped locally!");
		unsigned char x = reader.GetChar();
		unsigned char y = reader.GetChar();


		s.character.x = x;
		s.character.y = y;
	}
	if(type == WarpType::WARP_SWITCH){
		s.emf = shared_ptr<EMF>(new EMF(mapID));
		if(s.emf->exists)
		{
			s.map.Reset();
		}
		else
		{
			// Couldn't load map
		}

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
		s.map.id = mapID;
		map_rid[2] = reader.GetByte();
		map_rid[3] = reader.GetByte();
		reader.GetThree(); // map file size
	}
}
