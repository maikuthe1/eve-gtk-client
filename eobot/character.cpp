#include "character.hpp"
#include "singleton.hpp"

Character::Character()
{
    this->id = 0;
    this->gameworld_id = 0;
    this->clas = 0;
    this->admin_level = static_cast<AdminLevel>(0);
    this->level = 0;
    this->exp = 0;
    this->usage = 0;
    this->hp = 0;
    this->max_hp = 0;
    this->tp = 0;
    this->max_tp = 0;
    this->max_sp = 0;
    this->stat_points = 0;
    this->skill_points = 0;
    this->karma = 0;
    this->min_dam = 0;
    this->max_dam = 0;
    this->accuracy = 0;
    this->evade = 0;
    this->armor = 0;
    this->wis = 0;
    this->intl = 0;
    this->agi = 0;
    this->con = 0;
    this->cha = 0;

    this->weight = 0;
    this->max_weight = 0;

    this->x = 0;
    this->y = 0;
    this->direction = static_cast<Direction>(0);
    this->gender = static_cast<Gender>(0);
    this->hair_style = 0;
    this->hair_color = 0;
    this->race = static_cast<Skin>(0);

    this->sitting = static_cast<SitState>(0);
    this->visibility = 0;

    this->command_clock.restart();
}

short Character::GetHP(){
	return hp;
}
void Character::SetHP(short newHP){
	S &s = S::GetInstance();
	hp = newHP;
	s.signalCharacterChanged(this, ChangeType::HP);
	signalChanged(ChangeType::HP);
}

short Character::GetMaxHP(){
	return max_hp;
}
void Character::SetMaxHP(short newHP){
	S &s = S::GetInstance();
	max_hp = newHP;
	s.signalCharacterChanged(this, ChangeType::MaxHP);
	signalChanged(ChangeType::MaxHP);
}

int Character::GetEXP(){
	return exp;
}
void Character::SetEXP(int newVal){
	S &s = S::GetInstance();
	exp = newVal;
	s.signalCharacterChanged(this, ChangeType::EXP);
	signalChanged(ChangeType::EXP);
}

int Character::GetUsage(){
	return usage;
}
void Character::SetUsage(int newVal){
	S &s = S::GetInstance();
	usage = newVal;
	s.signalCharacterChanged(this, ChangeType::Usage);
	signalChanged(ChangeType::Usage);
}

short Character::GetMaxTP(){
	return max_tp;
}
void Character::SetMaxTP(short newVal){
	S &s = S::GetInstance();
	max_tp = newVal;
	s.signalCharacterChanged(this, ChangeType::MaxTP);
	signalChanged(ChangeType::MaxTP);
}

short Character::GetTP(){
	return tp;
}
void Character::SetTP(short newVal){
	S &s = S::GetInstance();
	tp = newVal;
	s.signalCharacterChanged(this, ChangeType::TP);
	signalChanged(ChangeType::TP);
}

short Character::GetMaxSP(){
	return max_sp;
}
void Character::SetMaxSP(short newVal){
	S &s = S::GetInstance();
	max_sp = newVal;
	s.signalCharacterChanged(this, ChangeType::SP);
	signalChanged(ChangeType::SP);
}

short Character::GetStatPoints(){
	return stat_points;
}
void Character::SetStatPoints(short newVal){
	S &s = S::GetInstance();
	stat_points = newVal;
	s.signalCharacterChanged(this, ChangeType::StatPoints);
	signalChanged(ChangeType::StatPoints);
}

short Character::GetSkillPoints(){
	return skill_points;
}
void Character::SetSkillPoints(short newVal){
	S &s = S::GetInstance();
	skill_points = newVal;
	s.signalCharacterChanged(this, ChangeType::SkillPoints);
	signalChanged(ChangeType::SkillPoints);
}

short Character::GetKarma(){
	return karma;
}
void Character::SetKarma(short newVal){
	S &s = S::GetInstance();
	karma = newVal;
	s.signalCharacterChanged(this, ChangeType::Karma);
	signalChanged(ChangeType::Karma);
}

short Character::GetMinDamage(){
	return min_dam;
}
void Character::SetMinDamage(short newVal){
	S &s = S::GetInstance();
	min_dam = newVal;
	s.signalCharacterChanged(this, ChangeType::MinDamage);
	signalChanged(ChangeType::MinDamage);
}

short Character::GetMaxDamnage(){
	return max_dam;
}
void Character::SetMaxDamage(short newVal){
	S &s = S::GetInstance();
	max_dam = newVal;
	s.signalCharacterChanged(this, ChangeType::MaxDamage);
	signalChanged(ChangeType::MaxDamage);
}

short Character::GetAccuracy(){
	return accuracy;
}
void Character::SetAccuracy(short newVal){
	S &s = S::GetInstance();
	accuracy = newVal;
	s.signalCharacterChanged(this, ChangeType::Accuracy);
	signalChanged(ChangeType::Accuracy);
}

short Character::GetEvade(){
	return evade;
}
void Character::SetEvade(short newVal){
	S &s = S::GetInstance();
	evade = newVal;
	s.signalCharacterChanged(this, ChangeType::Evade);
	signalChanged(ChangeType::Evade);
}

short Character::GetArmor(){
	return armor;
}
void Character::SetArmor(short newVal){
	S &s = S::GetInstance();
	armor = newVal;
	s.signalCharacterChanged(this, ChangeType::Armor);
	signalChanged(ChangeType::Armor);
}

short Character::GetStr(){
	return evade;
}
void Character::SetStr(short newVal){
	S &s = S::GetInstance();
	str = newVal;
	s.signalCharacterChanged(this, ChangeType::Str);
	signalChanged(ChangeType::Str);
}

short Character::GetWis(){
	return wis;
}
void Character::SetWis(short newVal){
	S &s = S::GetInstance();
	wis = newVal;
	s.signalCharacterChanged(this, ChangeType::Wis);
	signalChanged(ChangeType::Wis);
}

short Character::GetInt(){
	return intl;
}
void Character::SetInt(short newVal){
	S &s = S::GetInstance();
	intl = newVal;
	s.signalCharacterChanged(this, ChangeType::Int);
	signalChanged(ChangeType::Int);
}

short Character::GetAgi(){
	return agi;
}
void Character::SetAgi(short newVal){
	S &s = S::GetInstance();
	agi = newVal;
	s.signalCharacterChanged(this, ChangeType::Agi);
	signalChanged(ChangeType::Agi);
}

short Character::GetCon(){
	return con;
}
void Character::SetCon(short newVal){
	S &s = S::GetInstance();
	con = newVal;
	s.signalCharacterChanged(this, ChangeType::Con);
	signalChanged(ChangeType::Con);
}

short Character::GetCha(){
	return cha;
}
void Character::SetCha(short newVal){
	S &s = S::GetInstance();
	cha = newVal;
	s.signalCharacterChanged(this, ChangeType::Cha);
	signalChanged(ChangeType::Cha);
}

unsigned char Character::GetHPPercent(){
	return hp_percent;
}

void Character::SetHPPercent(unsigned char newPercent){
	S &s = S::GetInstance();
	hp_percent = newPercent;
	signalChanged(ChangeType::HP);
	s.signalCharacterChanged(this, ChangeType::HP);
}

bool Character::GetPartyLeader(){
	return partyLeader;
}

void Character::SetPartyLeader(bool newPartyLeader){
	partyLeader = newPartyLeader;
}

unsigned char Character::GetLevel(){
	return level;
}

void Character::SetLevel(unsigned char newLevel){
	S &s = S::GetInstance();
	level = newLevel;
	signalChanged(ChangeType::Level);
	s.signalCharacterChanged(this, ChangeType::Level);
}