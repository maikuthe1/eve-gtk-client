#include "character.hpp"
#include "singleton.hpp"

Character::Character() : paperdoll{{}}, cosmetic_paperdoll{{}}, preview_paperdoll{{}}
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

	this->paperdoll_data = Paperdoll();
}

short Character::GetHP(){
	return hp;
}
void Character::SetHP(short newHP){
	S &s = S::GetInstance();
	hp = newHP;
	s.signalCharacterChanged.emit(this, ChangeType::HP);
	signalChanged.emit(ChangeType::HP);
}

short Character::GetMaxHP(){
	return max_hp;
}
void Character::SetMaxHP(short newHP){
	S &s = S::GetInstance();
	max_hp = newHP;
	s.signalCharacterChanged.emit(this, ChangeType::MaxHP);
	signalChanged.emit(ChangeType::MaxHP);
}

int Character::GetEXP(){
	return exp;
}
void Character::SetEXP(int newVal){
	S &s = S::GetInstance();
	exp = newVal;
	s.signalCharacterChanged.emit(this, ChangeType::EXP);
	signalChanged.emit(ChangeType::EXP);
}

int Character::GetUsage(){
	return usage;
}
void Character::SetUsage(int newVal){
	S &s = S::GetInstance();
	usage = newVal;
	s.signalCharacterChanged.emit(this, ChangeType::Usage);
	signalChanged.emit(ChangeType::Usage);
}

short Character::GetMaxTP(){
	return max_tp;
}
void Character::SetMaxTP(short newVal){
	S &s = S::GetInstance();
	max_tp = newVal;
	s.signalCharacterChanged.emit(this, ChangeType::MaxTP);
	signalChanged.emit(ChangeType::MaxTP);
}

short Character::GetTP(){
	return tp;
}
void Character::SetTP(short newVal){
	S &s = S::GetInstance();
	tp = newVal;
	s.signalCharacterChanged.emit(this, ChangeType::TP);
	signalChanged.emit(ChangeType::TP);
}

short Character::GetMaxSP(){
	return max_sp;
}
void Character::SetMaxSP(short newVal){
	S &s = S::GetInstance();
	max_sp = newVal;
	s.signalCharacterChanged.emit(this, ChangeType::SP);
	signalChanged.emit(ChangeType::SP);
}

short Character::GetStatPoints(){
	return stat_points;
}
void Character::SetStatPoints(short newVal){
	S &s = S::GetInstance();
	stat_points = newVal;
	s.signalCharacterChanged.emit(this, ChangeType::StatPoints);
	signalChanged.emit(ChangeType::StatPoints);
}

short Character::GetSkillPoints(){
	return skill_points;
}
void Character::SetSkillPoints(short newVal){
	S &s = S::GetInstance();
	skill_points = newVal;
	s.signalCharacterChanged.emit(this, ChangeType::SkillPoints);
	signalChanged.emit(ChangeType::SkillPoints);
}

short Character::GetKarma(){
	return karma;
}
void Character::SetKarma(short newVal){
	S &s = S::GetInstance();
	karma = newVal;
	s.signalCharacterChanged.emit(this, ChangeType::Karma);
	signalChanged.emit(ChangeType::Karma);
}

short Character::GetMinDamage(){
	return min_dam;
}
void Character::SetMinDamage(short newVal){
	S &s = S::GetInstance();
	min_dam = newVal;
	s.signalCharacterChanged.emit(this, ChangeType::MinDamage);
	signalChanged.emit(ChangeType::MinDamage);
}

short Character::GetMaxDamnage(){
	return max_dam;
}
void Character::SetMaxDamage(short newVal){
	S &s = S::GetInstance();
	max_dam = newVal;
	s.signalCharacterChanged.emit(this, ChangeType::MaxDamage);
	signalChanged.emit(ChangeType::MaxDamage);
}

short Character::GetAccuracy(){
	return accuracy;
}
void Character::SetAccuracy(short newVal){
	S &s = S::GetInstance();
	accuracy = newVal;
	s.signalCharacterChanged.emit(this, ChangeType::Accuracy);
	signalChanged.emit(ChangeType::Accuracy);
}

short Character::GetEvade(){
	return evade;
}
void Character::SetEvade(short newVal){
	S &s = S::GetInstance();
	evade = newVal;
	s.signalCharacterChanged.emit(this, ChangeType::Evade);
	signalChanged.emit(ChangeType::Evade);
}

short Character::GetArmor(){
	return armor;
}
void Character::SetArmor(short newVal){
	S &s = S::GetInstance();
	armor = newVal;
	s.signalCharacterChanged.emit(this, ChangeType::Armor);
	signalChanged.emit(ChangeType::Armor);
}

short Character::GetStr(){
	return evade;
}
void Character::SetStr(short newVal){
	S &s = S::GetInstance();
	str = newVal;
	s.signalCharacterChanged.emit(this, ChangeType::Str);
	signalChanged.emit(ChangeType::Str);
}

short Character::GetWis(){
	return wis;
}
void Character::SetWis(short newVal){
	S &s = S::GetInstance();
	wis = newVal;
	s.signalCharacterChanged.emit(this, ChangeType::Wis);
	signalChanged.emit(ChangeType::Wis);
}

short Character::GetInt(){
	return intl;
}
void Character::SetInt(short newVal){
	S &s = S::GetInstance();
	intl = newVal;
	s.signalCharacterChanged.emit(this, ChangeType::Int);
	signalChanged.emit(ChangeType::Int);
}

short Character::GetAgi(){
	return agi;
}
void Character::SetAgi(short newVal){
	S &s = S::GetInstance();
	agi = newVal;
	s.signalCharacterChanged.emit(this, ChangeType::Agi);
	signalChanged.emit(ChangeType::Agi);
}

short Character::GetCon(){
	return con;
}
void Character::SetCon(short newVal){
	S &s = S::GetInstance();
	con = newVal;
	s.signalCharacterChanged.emit(this, ChangeType::Con);
	signalChanged.emit(ChangeType::Con);
}

short Character::GetClass(){
	return clas;
}
void Character::SetClass(short newVal){
	S &s = S::GetInstance();
	clas = newVal;
	s.signalCharacterChanged.emit(this, ChangeType::Clas);
	signalChanged.emit(ChangeType::Clas);
}

Gender Character::GetGender(){
	return gender;
}
void Character::SetGender(char newVal){
	S &s = S::GetInstance();
	gender = static_cast<Gender>(newVal);
	s.signalCharacterChanged.emit(this, ChangeType::Gender);
	signalChanged.emit(ChangeType::Gender);
}

short Character::GetCha(){
	return cha;
}
void Character::SetCha(short newVal){
	S &s = S::GetInstance();
	cha = newVal;
	s.signalCharacterChanged.emit(this, ChangeType::Cha);
	signalChanged.emit(ChangeType::Cha);
}

unsigned char Character::GetHPPercent(){
	return hp_percent;
}

void Character::SetHPPercent(unsigned char newPercent){
	S &s = S::GetInstance();
	hp_percent = newPercent;
	signalChanged.emit(ChangeType::HP);
	s.signalCharacterChanged.emit(this, ChangeType::HP);
}

void Character::SetGuild(std::string newVal){
	S &s = S::GetInstance();
	guild = newVal;
	s.signalCharacterChanged.emit(this, ChangeType::Guild);
	signalChanged.emit(ChangeType::Guild);
}

std::string Character::GetGuild(){
	return guild;
}

void Character::SetGuildRank(std::string newVal){
	S &s = S::GetInstance();
	guild_rank = newVal;
	s.signalCharacterChanged.emit(this, ChangeType::GuildRank);
	signalChanged.emit(ChangeType::GuildRank);
}

std::string Character::GetGuildRank(){
	return guild_rank;
}

void Character::SetHome(std::string newVal){
	S &s = S::GetInstance();
	home = newVal;
	s.signalCharacterChanged.emit(this, ChangeType::Home);
	signalChanged.emit(ChangeType::Home);
}

std::string Character::GetHome(){
	return home;
}

void Character::SetTitle(std::string newVal){
	S &s = S::GetInstance();
	title = newVal;
	s.signalCharacterChanged.emit(this, ChangeType::Title);
	signalChanged.emit(ChangeType::Title);
}

std::string Character::GetTitle(){
	return title;
}

void Character::SetPartner(std::string newVal){
	S &s = S::GetInstance();
	partner = newVal;
	signalChanged.emit(ChangeType::Partner);
	s.signalCharacterChanged.emit(this, ChangeType::Partner);
}

std::string Character::GetPartner(){
	return partner;
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
	signalChanged.emit(ChangeType::Level);
	s.signalCharacterChanged.emit(this, ChangeType::Level);
}