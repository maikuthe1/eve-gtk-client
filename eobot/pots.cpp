#include "pots.hpp"
#include "singleton.hpp"
#include <string>
#include <sstream>

Pots::Pots(short gameworld_id, short item_amount){
    this->run = false;
    this->item_amount = item_amount;
    this->gameworld_id = gameworld_id;
    this->clock.restart();
}

Pots::Pots(){
    this->run = false;
    this->item_amount = 0;
    this->gameworld_id = -1;
    this->clock.restart();
    this->reminder_clock.restart();
}

void Pots::Run(short gameworld_id){
    this->run = true;
    this->item_amount = 0;
    this->gameworld_id = gameworld_id;
    this->clock.restart();
    this->reminder_clock.restart();

    S::GetInstance().eoclient.TradeRequest(this->gameworld_id);
}
