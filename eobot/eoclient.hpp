// Supress unused variable warning
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"

#ifndef EOCLIENT_HPP_INCLUDED
#define EOCLIENT_HPP_INCLUDED

#include "config.hpp"
#include "packet.hpp"
#include "handlers/handlers.hpp"
#include "character.hpp"
#include "localmessage.hpp"
#include "npc.hpp"
#include "walkaction.hpp"
#include "astar.hpp"

#include <giomm-2.4/giomm.h>
#include <SFML/Network.hpp>
#include <string>
#include <memory>

using std::unique_ptr;

struct Account
{
	std::vector<Character> characters;
};

class EOClient
{
public:
	enum class PacketState
	{
		ReadLen1,
		ReadLen2,
		ReadData
	};

	enum class State
	{
		Uninitialized,
		Initialized,
		LoggedIn,
		Playing
	};

private:
	unique_ptr<sf::TcpSocket> socket;
	bool connected;

	std::string send_buffer;
	std::string recv_buffer;

	PacketState packet_state;
	unsigned char raw_length[2];
	unsigned int length;
	std::string data;
	Glib::RefPtr<Gio::Settings> gioSettings;

	typedef void(*handler_func)(PacketReader packet);
	std::map<PacketFamily, std::map<PacketAction, handler_func>> handlers;

	int seq_start;
	int seq;

	int session_id;

	State state;

public:
	std::vector<std::string> controllers;
	unsigned int seeDistance = 11;
	int refreshed;
	unsigned char range = 5;
	Direction GetDirection(short x, short y);
	sf::Clock lastTick;
	int accountNum;
	PacketProcessor processor;
	Account account;
	sf::Clock refresh_clock;
	sf::Clock uptime_clock;
	EOClient(bool initialize = false);
	bool Connect();
	void Disconnect();
	bool Connected();
	std::vector<std::string> Explode(std::string str,  std::string delimiter);
	void Send(PacketBuilder packet);
	void HandleData(std::string data);
	void RegisterHandler(PacketFamily family, PacketAction action, handler_func func);
	void UnregisterHandler(PacketFamily family, PacketAction action);
	void Tick();
	void Reset();
	void InitSequenceByte(unsigned char s1, unsigned char s2);
	void UpdateSequenceByte(unsigned short s1, unsigned char s2);
	
	int GenSequenceByte();
	void SetState(State state);
	State GetState();
	void SendChatMessage(ChatMessage message);

	void DropItem(short itemID, int amount, int x, int y);
	std::map<std::vector<std::string>, std::vector<std::string>> responses;
	void RequestInit();
	void Initialize(PacketReader reader);
	void LoginRequest(std::string username, std::string password);
	void LoginRequest(Glib::ustring username, Glib::ustring password);
	void AccountRequest(std::string username);
	void AccountCreate(std::string username, std::string password/*, std::string real_name, std::string location, std::string email*/);
	void SelectCharacter(unsigned int id);
	void CharacterRequest();
	void RequestPlayerList();
	void AcceptPartyInvite(unsigned char type, unsigned short playerID);
	void CreateCharacter(short gender,short hairstyle,short haircolor,short race, std::string name);
	void TalkPublicOverride(std::string message);
	void TalkPublic(std::string message);
	void TalkParty(std::string message);
	void TalkWPublic(std::wstring message);
	void PartyRemove(unsigned short id);
	void TalkGuild(std::string message);
	void TalkGlobal(std::string message);
	void TalkTell(std::string name, std::string message);
	void Face(Direction direction);
	void RefreshRequest();
	void PartyInvite(unsigned char type, unsigned short playerID);
	void TradeRequest(short gameworld_id);
	void TradeAccept(short gameworld_id);
	void TradeAdd(short item_id, int item_amount);
	void Equip(unsigned short itemID, unsigned short subLoc);
	void TradeAgree();
	void TradeClose();
	void Walk(Direction direction);
	void Attack(Direction direction);
	void Sit(SitAction sit_action);
	void UseItem(short itemId);
	void OpenDoor(int x, int y);
	void Christmas();
	void TakeChest(int x, int y, int itemId);
	void OpenShop(short npcId);
	void BuyShop(short itemId, int amount);
	void PickUp(short uid);
};

#endif // EOCLIENT_HPP_INCLUDED
