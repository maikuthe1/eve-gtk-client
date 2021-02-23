#include "inventory.hpp"
#include "singleton.hpp"

void Inventory::AddItem(short id, int amount)
{
	S &s = S::GetInstance();
	for(unsigned int i = 0; i < s.inventory.items.size(); ++i)
	{
		if(this->items[i].first == id)
		{
			this->items[i].second += amount;
			s.signalInventoryChange.emit(id, this->items[i].second);
			return;
		}
	}

	this->items.push_back(std::make_pair(id, amount));
	s.signalInventoryAdd.emit(id, amount);
}

void Inventory::DelItem(short id, int amount)
{
	S &s = S::GetInstance();
	for(unsigned int i = 0; i < this->items.size(); ++i)
	{
		if(this->items[i].first == id)
		{
			if(amount >= this->items[i].second)
			{
				this->items.erase(this->items.begin() + i);
				return;
			}
			else if(amount < this->items[i].second)
			{
				this->items[i].second -= amount;
				s.signalInventoryChange.emit(id, this->items[i].second);
				return;
			}
		}
	}
}

void Inventory::SetAmount(short id, int amount)
{
	S &s = S::GetInstance();
	for(unsigned int i = 0; i < this->items.size(); ++i)
	{
		if(this->items[i].first == id)
		{
			if(amount <= 0)
			{
				this->items.erase(this->items.begin() + i);
				s.signalInventoryChange.emit(id, amount);
				return;
			}
			this->items[i].second = amount;
			s.signalInventoryChange.emit(id, amount);
		}
	}
}

bool Inventory::FindItem(short id, int amount)
{
	for(unsigned int i = 0; i < this->items.size(); ++i)
	{
		if(this->items[i].first == id && this->items[i].second >= amount)
		{
			return true;
		}
	}

	return false;
}

int Inventory::GetItemAmount(short id)
{
	for(unsigned int i = 0; i < this->items.size(); ++i)
	{
		if(this->items[i].first == id)
		{
			return this->items[i].second;
		}
	}

	return 0;
}

void Inventory::Clear()
{
	this->items.clear();
}
