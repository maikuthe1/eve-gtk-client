

#include "handlers.hpp"
#include "../singleton.hpp"

// bot drops an item
void Item_Drop(PacketReader reader)
{
    S &s = S::GetInstance();
    unsigned short id = reader.GetShort();
    unsigned int amount = reader.GetThree();
    unsigned int hasItem = reader.GetInt();
    unsigned short uid = reader.GetShort();
    unsigned char x = reader.GetChar(), y = reader.GetChar();
    unsigned char weight= reader.GetChar();
    unsigned char maxWeight= reader.GetChar();
    Map_Item item(uid, id, amount, x, y);
    item.data = shared_ptr<EIF_Data>(new EIF_Data(s.eif->Get(id)));
    s.map.items.push_back(item);
	s.inventory.DelItem(id, amount);
}

// Item added to map
// Walk:Reply shows new items when you walk in/out range (i think)
// Welcome packet has current items on connect as does refresh:reply
void Item_Add(PacketReader reader)
{
    S &s = S::GetInstance();
    unsigned short id = reader.GetShort(); // item id
    unsigned short uid = reader.GetShort(); // unique id?? like character map_index maybe
    unsigned int amount = reader.GetThree(); // amount
    unsigned char x = reader.GetChar(); // x
    unsigned char y = reader.GetChar(); // y
    Map_Item item(uid, id, amount, x, y);
    item.data = shared_ptr<EIF_Data>(new EIF_Data(s.eif->Get(id)));
    s.map.items.push_back(item);
}

// Item removed from map
void Item_Remove(PacketReader reader)
{
	S &s = S::GetInstance();
	unsigned short uid = reader.GetShort();
	for(unsigned int i = 0; i < s.map.items.size(); i++){
		if(s.map.items[i].uid == uid){
            s.map.items.erase(s.map.items.begin() + i);
            break;
        }
	}
}

void Item_Get(PacketReader reader)
{
	S &s = S::GetInstance();
	unsigned short uid = reader.GetShort(); // unique id?? like character map_index maybe
	unsigned short id = reader.GetShort(); // item id
	unsigned int taken = reader.GetThree(); // amount taken i think
	unsigned char weight = reader.GetChar(); // current weight
	unsigned char maxWeight = reader.GetChar(); // max weight

	s.character.weight = weight;
	s.character.max_weight = maxWeight;
	std::string name = s.eif->Get(id).name;

	s.inventory.AddItem(id, taken);
	for(unsigned int i = 0; i < s.map.items.size(); i++){
		if(s.map.items[i].uid == uid){
			s.map.items.erase(s.map.items.begin() + i);
			break;
		}
	}
	//s.signalInventoryAdd.emit(id, taken);
}

// Item used
void Item_Reply(PacketReader reader)
{
	S &s = S::GetInstance();
	EIF_Data::Type type = (EIF_Data::Type)reader.GetChar(); // item type
	unsigned short id = reader.GetShort(); // item id
	unsigned int amount = reader.GetInt();

	EIF_Data* item = new EIF_Data(s.eif->Get(id));

	switch (item->type)
	{
		case EIF_Data::Type::Heal:
		{
			unsigned char weight = reader.GetChar();
			unsigned char maxWeight = reader.GetChar();

			unsigned int hpGain = reader.GetInt();
			unsigned short hp = reader.GetShort();
			unsigned short tp = reader.GetShort();

			// Inventory
			s.inventory.SetAmount(id, amount);
			s.character.weight = weight;
			s.character.max_weight = maxWeight;

			// Health / TP
			s.character.SetHP(hp);
			s.character.SetTP(tp);
			break;
		}

		default:
			s.inventory.SetAmount(id, amount);
			break;
	}

	delete(item);
}
