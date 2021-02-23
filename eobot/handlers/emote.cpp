#include "handlers.hpp"
#include "../singleton.hpp"
#include "../util.hpp"
#include <string>
#include <sstream>
#include <windows.h>
#include "commands.hpp"
#include <fstream>
#include <iostream>

void Emote_Player(PacketReader reader)
{
    S &s = S::GetInstance();
    enum Emote : unsigned char
    {Happy = 1, Depressed = 2, Sad = 3, Angry = 4, Confused = 5, Surprised = 6, Hearts = 7, Moon = 8, Suicidal = 9, Embarassed = 10, Drunk = 11, Trade = 12, LevelUp = 13, Playful = 14};


    short gameworld_id = reader.GetShort();
    char emote = reader.GetChar();
	
    switch(emote)
    {
    case Emote::Happy :
        break;
    case Emote::Hearts :
        break;
    case Emote::Suicidal :
        break;
    case Emote::Playful :
        break;
    case Emote::Depressed :
        break;
    case Emote::Moon :
        break;
    case Emote::Surprised :
        break;
    case Emote::Angry :
        break;
    case Emote::Confused :
        break;
    case Emote::Embarassed :
        break;
    }
}
