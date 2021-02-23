// Supress unused variable warning
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"



#ifndef MAP_ITEM_HPP_INCLUDED
#define MAP_ITEM_HPP_INCLUDED

//#include "const/character.hpp"
#include "eodata.hpp"

#include <memory>

using std::shared_ptr;

class Map_Item
{
public:
    unsigned short uid;
    unsigned short owner;
    unsigned short id;
    unsigned int amount;
    unsigned char x, y;
    shared_ptr<EIF_Data> data;

    Map_Item(unsigned short uid, unsigned short id, unsigned int amount, unsigned char x, unsigned char y);
};

#endif // ITEM_ADD_HPP_INCLUDED
