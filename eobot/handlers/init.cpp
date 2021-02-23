

#include "handlers.hpp"
#include "../packet.hpp"
#include "../singleton.hpp"
#include "../const/init.hpp"
#include <windows.h>

#include <cstdio>

void INIT_INIT(PacketReader reader)
{
	S &s = S::GetInstance();
	if(static_cast<InitReply>(reader.PeekByte()) == InitReply::OK){
		InitReply result(static_cast<InitReply>(reader.GetByte()));
		if(result == InitReply::OK)
		{
			//byte yo = reader.GetByte();
			s.eoclient.Initialize(reader);

			s.eoclient.RegisterHandler(PacketFamily::Connection, PacketAction::Player, Connection_Player);
			s.eoclient.RegisterHandler(PacketFamily::Login, PacketAction::Reply, Login_Reply);
			s.eoclient.RegisterHandler(PacketFamily::Welcome, PacketAction::Reply, Welcome_Reply);

			PacketBuilder packet(PacketFamily::Connection, PacketAction::Accept);
			s.eoclient.Send(packet);
			s.signalConnectionReply.emit(result, "OK");
			
			//s.eoclient.LoginRequest(s.config.GetValue("Username"), s.config.GetValue("Password"));
		}
			else
		{
			std::string reason = "Unknown";

			switch(result)
			{
			case InitReply::OutOfDate:
				reason = "OutOfDate";
				break;

			case InitReply::Banned:
				reason = "Banned";
				break;

			case InitReply::Map:
				reason = "Map";
				break;

			case InitReply::EIF:
				reason = "EIF";
				break;

			case InitReply::ENF:
				reason = "ENF";
				break;

			case InitReply::ESF:
				reason = "ESF";
				break;

			case InitReply::Players:
				reason = "Players";
				break;

			case InitReply::MapMutation:
				reason = "MapMutation";
				break;

			case InitReply::FriendListPlayers:
				reason = "FriendListPlayers";
				break;

			case InitReply::ECF:
				reason = "ECF";
				break;

			default:
				break;
			}
			
			s.signalConnectionReply.emit(result, Glib::ustring(reason));

			printf("EOClient: init failed, reason: %s\n", reason.c_str());
			s.eoclient.Disconnect();
		}
	}
	else{
		reader.GetChar(); // friends list or online list
		unsigned short amount = reader.GetShort();
		reader.GetByte(); // 255
		std::vector<Character> list;
		for(unsigned int i = 0; i < amount; ++i){
			Character newCharacter = Character();
			newCharacter.name = reader.GetBreakString();
			newCharacter.title = reader.GetBreakString();
			reader.GetChar(); // 0
			newCharacter.SetIcon((PaperdollIcon)reader.GetChar());
			newCharacter.clas = reader.GetChar();
			newCharacter.guild_tag = reader.GetFixedString(3);
			reader.GetByte();
			list.push_back(newCharacter);
		}
		for(unsigned int i = 0; i < list.size(); i++){
			bool found = false;
			for(unsigned int j = 0; j < s.onlineList.size(); j++){
				if(list[i].name == s.onlineList[j].name){
					found = true;
					break;
				}
			}
			
			if(!found){
				s.onlineList.push_back(list[i]);
				s.signalPlayerConnect.emit(list[i]);
			}
		}
		for(unsigned int i = 0; i < s.onlineList.size(); i++){
			bool found = false;
			for(unsigned int j = 0; j < list.size(); j++){
				if(s.onlineList[i].name == list[j].name){
					found = true;
					break;
				}
			}
			if(!found){
				s.signalPlayerDisconnect.emit(s.onlineList[i]);
				s.onlineList.erase(s.onlineList.begin() + i);
			}
		}
	}

}
