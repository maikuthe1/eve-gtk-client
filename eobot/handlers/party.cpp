#include "handlers.hpp"
#include "../singleton.hpp"

void Party_Request(PacketReader reader){
	S &s = S::GetInstance();
	unsigned char type = reader.GetChar();
	unsigned short playerID = reader.GetShort();
	std::string name = reader.GetEndString();
	if(std::find(s.blockInviteList.begin(), s.blockInviteList.end(), name) == s.blockInviteList.end())
		s.signalPartyInvite.emit(type, playerID, name);
}

void Party_Create(PacketReader reader){
	S &s = S::GetInstance();
	s.party.clear();
	while(reader.Remaining() > 0){
		Character member = Character();
		member.id = reader.GetShort();
		member.SetPartyLeader((bool)reader.GetChar());
		member.SetLevel(reader.GetChar());
		member.SetHPPercent(reader.GetChar());
		member.name = reader.GetBreakString();
		if(member.GetPartyLeader() == true)
			s.party.insert(s.party.begin(), member);
		else
			s.party.push_back(member);
	}
	s.signalPartyCreate.emit();
}

void Party_List(PacketReader reader){
	S &s = S::GetInstance();
	while(reader.Remaining() > 0){
		Character member = Character();
		member.id = reader.GetShort();
		member.SetPartyLeader((bool)reader.GetChar());
		member.SetLevel(reader.GetChar());
		member.SetHPPercent(reader.GetChar());
		member.name = reader.GetBreakString();
		for(unsigned int i = 0; i < s.party.size(); i++ ){
			if(s.party[i].id == member.id){
				if(member.GetLevel() != s.party[i].GetLevel()){
					s.party[i].SetLevel(member.GetLevel());
				}
				if(member.GetHPPercent() != s.party[i].GetHPPercent()){
					s.party[i].SetHPPercent(member.GetHPPercent());
				}
			}
		}
	}
}

void Party_Agree(PacketReader reader){
	S &s = S::GetInstance();
	unsigned short id = reader.GetShort();
	unsigned char percent = reader.GetChar();
	
	for(unsigned int i = 0; i < s.party.size(); i++ ){
		if(s.party[i].id == id && s.party[i].GetHPPercent() != percent){
			s.party[i].SetHPPercent(percent);
		}
	}
}

void Party_Add(PacketReader reader){
	S &s = S::GetInstance();
	Character member = Character();
	member.id = reader.GetShort();
	member.SetPartyLeader((bool)reader.GetChar());
	member.SetLevel(reader.GetChar());
	member.SetHPPercent(reader.GetChar());
	member.name = reader.GetEndString();
	s.party.push_back(member);
	s.signalPartyAdd.emit(&s.party[s.party.size() - 1]);
}

void Party_Remove(PacketReader reader){
	S &s = S::GetInstance();
	unsigned short id = reader.GetShort();
	for(unsigned int i = 0; i < s.party.size(); i++){
		if(s.party[i].id == id){
			s.signalPartyRemove.emit(&s.party[i]);
			s.party.erase(s.party.begin() + i);
		}
	}

}

void Party_Close(PacketReader reader){
	S &s = S::GetInstance();
	s.party.clear();
	s.signalPartyClose.emit();
}