// Supress unused variable warning
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"


#ifndef CHARACTER_HPP_INCLUDED
#define CHARACTER_HPP_INCLUDED

#include "const/character.hpp"
#include <SFML/System.hpp>
#include "sigc++-2.0/sigc++/sigc++.h"
#include <string>

class Character
{
protected:
	unsigned short hp;
	unsigned short max_hp;
	unsigned char hp_percent;
	bool partyLeader;
    int exp;
    int usage;

    short tp;
    short max_tp;
    short max_sp;

    short stat_points;
    short skill_points;

    short karma;

    short min_dam;
    short max_dam;

    short accuracy;
    short evade;
    short armor;

    short str;
    short wis;
    short intl;
    short agi;
    short con;
    short cha;
	
	PaperdollIcon icon;
    unsigned char level;
	
public:

    std::string name;
    unsigned int id;
    short gameworld_id;

    std::string title;
    std::string guild;
    std::string guild_rank;
    std::string guild_tag;
    std::string paperdoll;
	sigc::signal<void, ChangeType> signalChanged;

    unsigned char clas;

    unsigned int attacks = 0;
    unsigned int maxAttacks = 250;

    AdminLevel admin_level;


    short weight;
    short max_weight;

    unsigned char x;
    unsigned char newX;
    unsigned char y;
    unsigned char newY;
    Direction direction;
    Gender gender;
    unsigned char hair_style;
    unsigned char hair_color;
    Skin race;

    SitState sitting;
    unsigned char visibility;

    sf::Clock command_clock;

	Character();
	short GetHP();
	void SetHP(short newHP);
	short GetMaxHP();
	void SetMaxHP(short newHP);
	int GetEXP();
	void SetEXP(int newVal);
	int GetUsage();
	void SetUsage(int newVal);
	short GetTP();
	void SetTP(short newVal);
	short GetMaxTP();
	void SetMaxTP(short newVal);
	short GetMaxSP();
	void SetMaxSP(short newVal);
	short GetStatPoints();
	void SetStatPoints(short newVal);
	short GetSkillPoints();
	void SetSkillPoints(short newVal);
	short GetKarma();
	void SetKarma(short newVal);
	short GetMinDamage();
	void SetMinDamage(short newVal);
	short GetMaxDamnage();
	void SetMaxDamage(short newVal);
	short GetAccuracy();
	void SetAccuracy(short newVal);
	short GetEvade();
	void SetEvade(short newVal);
	short GetArmor();
	void SetArmor(short newVal);
	short GetStr();
	void SetStr(short newVal);
	short GetWis();
	void SetWis(short newVal);
	short GetInt();
	void SetInt(short newVal);
	short GetAgi();
	void SetAgi(short newVal);
	short GetCon();
	void SetCon(short newVal);
	short GetCha();
	void SetCha(short newVal);
	PaperdollIcon GetIcon() { return icon; };
	void SetIcon(PaperdollIcon newIcon){ icon = newIcon; };
	unsigned char GetHPPercent();
	void SetHPPercent(unsigned char newPercent );
	bool GetPartyLeader();
	void SetPartyLeader(bool newPartyLeader);
	void SetLevel(unsigned char newLevel);
	unsigned char GetLevel();
};

#endif // CHARACTER_HPP_INCLUDED
