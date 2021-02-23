#include "handlers.hpp"
#include "../singleton.hpp"

void NPC_Junk(PacketReader reader){
	S &s = S::GetInstance();
	unsigned short id = reader.GetShort();
	for(unsigned int i = 0; i < s.map.npcs.size(); i++){
		if(s.map.npcs[i].id == id)
			s.map.npcs.erase(s.map.npcs.begin() + i);
	}
}

void NPC_Spec(PacketReader reader)
{
    S &s = S::GetInstance();
    /////////////////////////// Packet is NPC:Accept if the player leveled up
    unsigned short killer = reader.GetShort(); // killer gameworld_id
    unsigned char direction = reader.GetChar(); // killer direction
    short index = reader.GetShort();
	if(killer == s.character.gameworld_id){
		// We killed it
	}

    // TODO: read the rest or not
    if(reader.Remaining() > 0) // seems to always be more
    {
		unsigned short uid = reader.GetShort();
		unsigned short id = reader.GetShort();
		unsigned char x = reader.GetChar(), y = reader.GetChar();
		unsigned int dropAmount = reader.GetInt();
		unsigned int amount = reader.GetThree();
		if(id != 0){
			Map_Item item(uid, id, amount, x, y);
			item.data = shared_ptr<EIF_Data>(new EIF_Data(s.eif->Get(id)));
			s.map.items.push_back(item);
		}
		if(reader.Remaining() > 0){
			unsigned int exp = reader.GetInt();
			if(killer == s.character.gameworld_id){
				int expGained = exp - s.character.GetEXP();
				s.character.SetEvade(exp);
				for(unsigned int i = 0; i < s.map.npcs.size(); i++){
					
				}

			}
		}


	}
	s.map.RemoveNPC(index);
}


void Shop_Buy(PacketReader reader)
{
	S &s = S::GetInstance();
	unsigned int hasItem = reader.GetInt();
	unsigned short item = reader.GetShort();
	unsigned int amount = reader.GetInt();
	unsigned char weight = reader.GetChar();
	unsigned char maxWeight = reader.GetChar();

	s.character.weight = weight;
	s.character.max_weight = maxWeight;
	bool itemFound = false;
	for(unsigned int i = 0; i < s.inventory.items.size(); i++){
		if((unsigned short)s.inventory.items[i].first == item){
			itemFound = true;
			break;
		}
	}

	for( auto const& [itemid, itemamount] : s.inventory.items )
	{
		if((unsigned short)itemid == item){
			itemFound = true;
			break;
		}
	}
	if(itemFound)
		s.inventory.SetAmount(item, amount);
	else
		s.inventory.AddItem(item, amount);
}
