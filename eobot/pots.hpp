// Supress unused variable warning
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"


#ifndef POTS_HPP_INCLUDED
#define POTS_HPP_INCLUDED

#include <SFML/System.hpp>

struct Pots
{
    bool run;
    short item_id = 4;
    int item_amount;
    short gameworld_id;
    sf::Clock clock;
    sf::Clock reminder_clock;

    Pots(short gameworld_id, short item_amount);
    Pots();
    void Run(short gameworld_id);
};

#endif // POTS_HPP_INCLUDED
