// Supress unused variable warning
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"



#ifndef MAP_HPP_INCLUDED
#define MAP_HPP_INCLUDED

#include "eodata.hpp"
#include "character.hpp"
#include "npc.hpp"

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "mapitem.hpp"

using std::shared_ptr;

// class responsible for rendering the map and holding data about map's objects
class Map
{
public:
    std::vector<Character> characters;
    std::vector<NPC> npcs;
    std::vector<Map_Item> items;
    unsigned int id;
    enum WalkResult
	{
		WalkFail = 0,
		WalkOK = 1,
		WalkWarped = 2
	};

    Map();
    void Load();
    void Reset();
    void Process();
    void Draw();

    int GetCharacterIndex(short gameworld_id);
    int GetCharacterIndex(std::string name);
    void RemoveCharacter(short gameworld_id);
    int GetNPCIndex(unsigned char gameworld_index);
    void RemoveNPC(unsigned char gameworld_index);
    bool Occupied(unsigned char x, unsigned char y, bool skipNPC = true);
    WalkResult Walk(Direction direction);

};

#endif // MAP_HPP_INCLUDED
