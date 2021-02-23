#include "handlers.hpp"
#include "../singleton.hpp"
#include "../util.hpp"
#include <string>
#include <sstream>
#include <windows.h>
#include "commands.hpp"
#include <fstream>
#include <iostream>
#include "../localmessage.hpp"
#include "../walkaction.hpp"
#include "../astar.hpp"

int lastN = 0;
template <typename T>
std::string std::to_string(T value)
{
	std::ostringstream os ;
	os << value ;
	return os.str() ;
}
bool isEchoing = false;
std::string controller = "";
bool cmdLocked = false;
std::vector<int> bannedItems(1);
bool InArrayInt(int needle, std::vector<int> haystack)
{
	bool found = false;
	for(int i = 0; i < (signed)haystack.size(); i++)
	{
		if(haystack[i] == needle)
		{
			found = true;
			break;
		}
	}
	return found;
}

std::vector<std::string> ProcessCommand(std::string name, std::string message, short gameworld_id, bool guildChat)
{

	S &s = S::GetInstance();
	std::string messageOrig = message;
	std::vector<std::string> ret;
	std::transform(name.begin(), name.end(), name.begin(), ::tolower);
	std::transform(message.begin(), message.end(), message.begin(), ::tolower);



	int char_index = s.map.GetCharacterIndex(gameworld_id);
	bool isController = false;
	for(unsigned int i = 0; i < s.eoclient.controllers.size(); i++){
		if(name.compare(s.eoclient.controllers[i]) != 0)
			isController = true;
	}
	if(!isController)
		return ret;

	std::vector<std::string> args = Args(message);

	std::string command = "";
	if(args.size() > 0)
	{
		command = args[0];
	}
	int index = s.map.GetCharacterIndex(name);
	if((command.compare("eor") != 0 && command.compare("stats") && command.compare("sitwin") != 0 && command.compare("lottery") != 0 && command.compare("number") != 0 && command.compare("love") != 0)){
			//if(name.compare("Maikutheone") != 0 && name.compare("maikutheone") != 0 && name.compare("Cheque") != 0 && name.compare("cheque") != 0 && name.compare("plasmastar") != 0 && name.compare("Plasmastar") != 0 && name.compare("virtue") != 0 && name.compare("Virtue") != 0 && name.compare("advrid") != 0 && name.compare("Advrid") != 0)
				//return ret;
	}
	if(index != -1)
	{
		std::string name_upper = name;
		name[0] = std::toupper(name[0]);
	}

	int cindex = s.map.GetCharacterIndex(name);
	if(index != -1)
	{
		if(s.map.characters[cindex].GetLevel() == 0){}
			//return ret;
	}

	if(command == "pickup")
	{
		for(unsigned int i = 0; i < s.map.items.size(); i++){
			int dx = std::abs(s.map.items[i].x - s.character.x);
			int dy = std::abs(s.map.items[i].y - s.character.y);
			int distance = dx + dy;

			if(distance <= 2){
				s.eoclient.PickUp(s.map.items[i].uid);
			}
		}
	}
	else if(command == "walkto" || command == "walk"){
		int x = std::stoi(args[1]);
		int y = std::stoi(args[2]);
	}
	else if(command == "reset" || command == "refresh"){
		s.eoclient.RefreshRequest();
	}
	else if(command == "items")
	{
		std::string nMessage = "";
		for(int i = 0; i < (signed)s.inventory.items.size() + 2; i++)
		{
			int item_id = s.inventory.items[i].first;
			std::string item_name = s.eif->Get(item_id).name;
			EIF_Data eif_data = s.eif->GetByNameLowercase(item_name);
			int item_amount = s.inventory.GetItemAmount(item_id);

			if(args.size() == 1)
			{
				nMessage += item_name + "("+ std::to_string(item_id) +") x "+ std::to_string(item_amount) +", ";
			}
			else
			{
				std::string searchString;
				for(int i = 1; i < (signed)args.size(); i++)
				{
					searchString += args[i];
					if(i != (signed)args.size() - 1)
						searchString += " ";
				}
				std::string itemNameLower = Lowercase(item_name);
				if (itemNameLower.find(searchString) != std::string::npos)
				{
					nMessage += item_name + "("+ std::to_string(item_id) +") x "+ std::to_string(item_amount) +", ";
				}
			}

		}
		ret.push_back(nMessage);

	}
		else if(command == "dropitem" || command == "di")
	{
		std::string itemName;
		if(command == "dropitem")
		{
			message = message.replace(message.find("dropitem "), std::string("dropitem ").size(), "");
		}
		if(command == "di")
		{
			message = message.replace(message.find("di "), std::string("di ").size(), "");
		}
		int amount;
		std::vector<std::string> messageSplit = s.eoclient.Explode(message, " ");
		amount = stoi(messageSplit[0]);
		for(int i = 1; i < (signed)messageSplit.size(); i++)
		{
			itemName += messageSplit[i];
			if(i != (signed)messageSplit.size() - 1)
				itemName += " ";
		}
		int item_id = s.eif->GetByNameLowercase(itemName.c_str()).id;
		s.eoclient.DropItem(item_id, amount, s.character.x, s.character.y);
	}
	else if(command == "stand" || command == "sit")
	{
			s.eoclient.Sit(SitAction::Stand);
	}
	else if(command == "move" && args.size() >= 2)
	{
		int times = 1;
		if(controller == name || controller == "")
		{
			if(args.size() >= 3)
			{
				times = atoi(args[2].c_str());
			}
			std::string direction = args[1];
			Direction direction2 = Direction::Up;
			if(direction == "up")
			{
				direction2 = Direction::Up;
			}
			if(direction == "down")
			{
				direction2 = Direction::Down;
			}
			if(direction == "left")
			{
				direction2 = Direction::Left;
			}
			if(direction == "right")
			{
				direction2 = Direction::Right;
			}
			//s.eoclient.walkAmount = times;
			//s.eoclient.walkDirection = direction2;
		}
	}
	else if(command == "invite"){
		if(args.size() == 2){
			//for(unsigned int i = 0; i < s.map.characters.size(); i++)
				//if(s.map.characters[i].name == args[1])
					//s.eoclient.PartyInvite(s.map.characters[i].gameworld_id);
		}
	}
	else if(command == "pm"){
		std::string toSay = message.replace(message.find("pm " + args[1] + " "), std::string("pm " + args[1] + " ").size(), "");
		s.eoclient.TalkTell(args[1], toSay);
	}
	else if(command == "face" && args.size() >= 2)
	{
		std::string direction = args[1];
		Direction direction2 = Direction::Up;
		if(direction == "up")
		{
			direction2 = Direction::Up;
		}
		if(direction == "down")
		{
			direction2 = Direction::Down;
		}
		if(direction == "left")
		{
			direction2 = Direction::Left;
		}
		if(direction == "right")
		{
			direction2 = Direction::Right;
		}
		s.eoclient.Face(direction2);
	}
	else if(command == "attack" && args.size() >= 2)
	{
		std::string direction = args[1];
		Direction direction2;
		if(direction == "up")
		{
			direction2 = Direction::Up;
		}
		if(direction == "down")
		{
			direction2 = Direction::Down;
		}
		if(direction == "left")
		{
			direction2 = Direction::Left;
		}
		if(direction == "right")
		{
			direction2 = Direction::Right;
		}
		s.eoclient.Attack(direction2);
	}
	else if(command == "emote" && args.size() >= 2)
	{
		if(args[1] == "love" || args[1] == "hearts")
		{
			PacketBuilder packet(PacketFamily::Emote, PacketAction::Report);
			packet.AddChar((unsigned char)Emote::Hearts);
			s.eoclient.Send(packet);
		}
		if(args[1] == "happy" || args[1] == "smiley")
		{
			PacketBuilder packet(PacketFamily::Emote, PacketAction::Report);
			packet.AddChar((unsigned char)Emote::Happy);
			s.eoclient.Send(packet);
		}
		if(args[1] == "depressed" || args[1] == "cloud")
		{
			PacketBuilder packet(PacketFamily::Emote, PacketAction::Report);
			packet.AddChar((unsigned char)Emote::Depressed);
			s.eoclient.Send(packet);
		}
		if(args[1] == "sad" || args[1] == "rain")
		{
			PacketBuilder packet(PacketFamily::Emote, PacketAction::Report);
			packet.AddChar((unsigned char)Emote::Sad);
			s.eoclient.Send(packet);
		}
		if(args[1] == "angry" || args[1] == "mad")
		{
			PacketBuilder packet(PacketFamily::Emote, PacketAction::Report);
			packet.AddChar((unsigned char)Emote::Angry);
			s.eoclient.Send(packet);
		}
		if(args[1] == "confused" || args[1] == "what")
		{
			PacketBuilder packet(PacketFamily::Emote, PacketAction::Report);
			packet.AddChar((unsigned char)Emote::Confused);
			s.eoclient.Send(packet);
		}
		if(args[1] == "surprised" || args[1] == "exclemation")
		{
			PacketBuilder packet(PacketFamily::Emote, PacketAction::Report);
			packet.AddChar((unsigned char)Emote::Surprised);
			s.eoclient.Send(packet);
		}
		if(args[1] == "moon" || args[1] == "afk")
		{
			PacketBuilder packet(PacketFamily::Emote, PacketAction::Report);
			packet.AddChar((unsigned char)Emote::Happy);
			s.eoclient.Send(packet);
		}
		if(args[1] == "suicidal" || args[1] == "lynch")
		{
			PacketBuilder packet(PacketFamily::Emote, PacketAction::Report);
			packet.AddChar((unsigned char)Emote::Suicidal);
			s.eoclient.Send(packet);
		}
		if(args[1] == "embarassed" || args[1] == "blush")
		{
			PacketBuilder packet(PacketFamily::Emote, PacketAction::Report);
			packet.AddChar((unsigned char)Emote::Embarassed);
			s.eoclient.Send(packet);
		}
		if(args[1] == "drunk" || args[1] == "drink")
		{
			PacketBuilder packet(PacketFamily::Emote, PacketAction::Report);
			packet.AddChar((unsigned char)Emote::Drunk);
			s.eoclient.Send(packet);
		}
		if(args[1] == "trade" || args[1] == "trading")
		{
			PacketBuilder packet(PacketFamily::Emote, PacketAction::Report);
			packet.AddChar((unsigned char)Emote::Trade);
			s.eoclient.Send(packet);
		}
		if(args[1] == "levelup" || args[1] == "lvl")
		{
			PacketBuilder packet(PacketFamily::Emote, PacketAction::Report);
			packet.AddChar((unsigned char)Emote::LevelUp);
			s.eoclient.Send(packet);
		}
		if(args[1] == "playful" || args[1] == "wtf")
		{
			PacketBuilder packet(PacketFamily::Emote, PacketAction::Report);
			packet.AddChar((unsigned char)Emote::Playful);
			s.eoclient.Send(packet);
		}
	}

	else if(command == "say")
	{
		std::string toSay = message.replace(message.find("say "), std::string("say ").size(), "");
		s.eoclient.TalkPublicOverride(toSay);
	}
	else if(command == "glb")
	{
		std::string toSay = message.replace(message.find("glb "), std::string("glb ").size(), "");
		s.eoclient.TalkGlobal(toSay);
	}
	else if(command == "wru")
	{
		std::string x = std::to_string(s.character.x);
		std::string y = std::to_string(s.character.y);
		std::string message = "I'm at " + x + ", "+ y;
		s.eoclient.TalkPublic(message);
	}

	else if(command == "useitemid" && args.size() >= 2)
	{
		int times = 1;
		if(args.size() == 3)
		{
			times = atoi(args[2].c_str());
		}
		for(int i = 0; i < times; i++)
		{
			s.eoclient.UseItem(std::stoi(args[1]));
			Sleep(50);
		}
	}

	return ret;
}

void Talk_Player(PacketReader reader)
{
	S &s = S::GetInstance();

	short gameworld_id = reader.GetShort();
	std::string message = reader.GetEndString();

	std::string name = "[Unknown]";
	int i = s.map.GetCharacterIndex(gameworld_id);
	int lvl = 0;
	if(i != -1)
	{
		name = s.map.characters[i].name;
		lvl = s.map.characters[i].GetLevel();
	}
	ChatMessage localMessage(name, "", message, ChatType::Local);
	if(isEchoing)
	{
		s.eoclient.TalkPublicOverride(message);
	}
	if(message[0] == '#')
	{
		message.erase(0, 1);

		if(!cmdLocked)
		{
			std::vector<std::string> ret = ProcessCommand(name, message, gameworld_id, false);
			std::vector<std::string> args = Args(message);

		}
	}
	s.signalMessageReceived(localMessage);
}

void Talk_Guild(PacketReader reader)
{
	S &s = S::GetInstance();

	short gameworld_id = reader.GetShort();
	std::string name = reader.GetBreakString();
	std::string message = reader.GetBreakString();
	if(isEchoing)
	{
		s.eoclient.TalkGuild(message);
	}
	if(message[0] == '&')
	{
		message.erase(0, 1);

		std::vector<std::string> ret = ProcessCommand(name, message, gameworld_id, true);
		std::vector<std::string> args = Args(message);

	}
}

void Talk_Tell(PacketReader reader)
{
	S &s = S::GetInstance();

	std::string name = reader.GetBreakString();
	std::string message = reader.GetBreakString();
	if(message[0] == '#')
	{
		message.erase(0, 1);
		int index = s.map.GetCharacterIndex(name);

		std::vector<std::string> ret = ProcessCommand(name, message, s.map.characters[index].gameworld_id, false);
	}
	ChatMessage privateMessage(name, s.character.name, message, ChatType::Private);
	s.signalMessageReceived(privateMessage);
}



void Talk_Message(PacketReader reader)
{
	S &s = S::GetInstance();
	std::string name = reader.GetBreakString();
	std::string message = reader.GetBreakString();
	short gameworld_id = reader.GetShort();
	int i = s.map.GetCharacterIndex(gameworld_id);
	int lvl = 0;
	if(i != -1)
	{
		lvl = s.map.characters[i].GetLevel();
	}
	ChatMessage globalMessage(name, "", message, ChatType::Global);
	s.signalMessageReceived(globalMessage);
}

void Talk_Party(PacketReader reader){
	S &s = S::GetInstance();
	unsigned short id = reader.GetShort();
	std::string name = "UNKNOWN";
	for(unsigned int i = 0; i < s.party.size(); i++){
		if(s.party[i].id == id)
			name = s.party[i].name;
	}
	std::string message = reader.GetEndString();
	ChatMessage partyMessage(name, s.character.name, message, ChatType::Party);
	int col=12;
	FlushConsoleInputBuffer(s.hConsole);
	SetConsoleTextAttribute(s.hConsole, col);
	printf("\n[PRTY]%s: %s\n", name.c_str(), message.c_str());
	SetConsoleTextAttribute(s.hConsole, 15);
	s.signalMessageReceived(partyMessage);
}

void Talk_Reply(PacketReader reader){ // PM not delivered
	S &s = S::GetInstance();
	reader.GetShort(); // TALK_NOTFOUND (1)
	std::string name = reader.GetEndString();
	
	s.signalMessageNotDelivered.emit(name);
}