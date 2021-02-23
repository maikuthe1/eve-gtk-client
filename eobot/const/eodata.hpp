

#ifndef CONST_EODATA_HPP_INCLUDED
#define CONST_EODATA_HPP_INCLUDED

enum class MapEffect : unsigned char
{
    Quake = 1
};

enum class FileType : unsigned char
{
    Map = 1,
    Item = 2,
    NPC = 3,
    Spell = 4,
    Class = 5
};

enum class ChatType : unsigned char
{
	Local = 1,
	Global = 2,
	Guild = 3,
	Party = 4,
	Admin = 5,
	Private = 6
};

#endif // CONST_EODATA_HPP_INCLUDED
