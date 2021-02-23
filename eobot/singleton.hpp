// Supress unused variable warning
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"

#ifndef SINGLETON_HPP_INCLUDED
#define SINGLETON_HPP_INCLUDED

#include "eoclient.hpp"
#include "config.hpp"
#include "eodata.hpp"
#include "map.hpp"
#include "inventory.hpp"
#include <windows.h>
#include "const/init.hpp"
#include "const/account.hpp"
#include "../logger.h"
#include "atlas.hpp"

#include <memory>
#include <vector>
#include <sigc++-2.0/sigc++/sigc++.h>

using std::shared_ptr;

class S
{
public:
    static S& GetInstance()
    {
        static S    instance;
        return instance;
    }
private:
    S() { call_exit = false; hConsole = GetStdHandle(STD_OUTPUT_HANDLE); }
public:
	sigc::signal<void, InitReply, Glib::ustring> signalConnectionReply;
	sigc::signal<void, LoginReply, Glib::ustring> signalLoginReply;
	sigc::signal<void, EOClient::State> signalStateChanged;
	sigc::signal<void, ChatMessage> signalMessageReceived;
	sigc::signal<void, Character*, ChangeType> signalCharacterChanged;
	sigc::signal<void, Character> signalPlayerConnect;
	sigc::signal<void, Character> signalPlayerDisconnect;
	sigc::signal<void, Character> signalPlayerAppear;
	sigc::signal<void, Character> signalPlayerRemove;
	sigc::signal<void, Character*> signalPartyRemove;
	sigc::signal<void, Character*> signalPartyAdd;
	sigc::signal<void> signalPartyClose;
	sigc::signal<void, std::string> signalMessageNotDelivered;
	sigc::signal<void, std::string, std::string> signalMessageBoxOpened;
	sigc::signal<void, Paperdoll> signalPaperdollOpened;
	sigc::signal<void> signalPartyCreate;
	sigc::signal<void, unsigned char, unsigned short, std::string> signalPartyInvite;
	sigc::signal<void, short, int> signalInventoryAdd;
	sigc::signal<void, short, int> signalInventoryRemove;
	sigc::signal<void, short, int> signalInventoryChange;

	int times = 0;
    S(S const&)               = delete;
    void operator=(S const&)  = delete;

    bool call_exit;
    bool archive = true;

    EOClient eoclient;
    Config config;
    HANDLE  hConsole;

    shared_ptr<EMF> emf;
    shared_ptr<EIF> eif;
    shared_ptr<ENF> enf;
    shared_ptr<ESF> esf;
    shared_ptr<ECF> ecf;

    Map map;
    Character character;
	Atlas atlas;
	std::vector<Character> onlineList;
	std::vector<Character> party;
	std::vector<std::string> blockInviteList;
    Inventory inventory;
};

#endif // SINGLETON_HPP_INCLUDED
