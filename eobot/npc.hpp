// Supress unused variable warning
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"



#ifndef NPC_HPP_INCLUDED
#define NPC_HPP_INCLUDED

#include "const/character.hpp"
#include "eodata.hpp"
#include <SFML/Window.hpp>


#include <memory>

using std::shared_ptr;

class NPC
{
public:
    unsigned char gameworld_index;
    short id;
    unsigned int hp;
    sf::Clock spawned;
    Direction direction;
    unsigned char x, y;
    shared_ptr<ENF_Data> data;

    NPC();
};

#endif // NPC_HPP_INCLUDED
