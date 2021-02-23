

#include "map.hpp"
#include "singleton.hpp"

Map::Map()
{

}

void Map::Load()
{
    // construct everything needed for map render
}

void Map::Reset()
{
    this->characters.clear();
    this->npcs.clear();
}

void Map::Process()
{

}

void Map::Draw()
{

}

int Map::GetCharacterIndex(short gameworld_id)
{
    for(std::size_t i = 0; i < this->characters.size(); ++i)
    {
        if(this->characters[i].gameworld_id == gameworld_id)
        {
            return i;
        }
    }

    return -1;
}

int Map::GetCharacterIndex(std::string name)
{
    for(std::size_t i = 0; i < this->characters.size(); ++i)
    {
        if(this->characters[i].name == name)
        {
            return i;
        }
    }

    return -1;
}

void Map::RemoveCharacter(short gameworld_id)
{
    int i = this->GetCharacterIndex(gameworld_id);
    if(i != -1)
    {
        this->characters.erase(this->characters.begin() + i);
    }
}

int Map::GetNPCIndex(unsigned char gameworld_index)
{
    for(std::size_t i = 0; i < this->npcs.size(); ++i)
    {
        if(this->npcs[i].gameworld_index == gameworld_index)
        {
            return i;
        }
    }

    return -1;
}

void Map::RemoveNPC(unsigned char gameworld_index)
{
    int i = this->GetNPCIndex(gameworld_index);
    if(i != -1)
    {
        this->npcs.erase(this->npcs.begin() + i);
    }
}

bool Map::Occupied(unsigned char x, unsigned char y, bool skipNPC)
{
    for(unsigned int i = 0; i < this->characters.size(); ++i)
    {
        if(this->characters[i].x == x && this->characters[i].y == y)
        {
            return true;
        }
    }

	if(!skipNPC){
		for(unsigned int i = 0; i < this->npcs.size(); ++i)
		{
			if(this->npcs[i].x == x && this->npcs[i].y == y)
			{
				return true;
			}
		}
	}

    return false;
}

Map::WalkResult Map::Walk(Direction direction){
	S &s = S::GetInstance();

	unsigned char target_x = s.character.x;
	unsigned char target_y = s.character.y;

	switch (direction)
	{
		case Direction::Up:
			target_y -= 1;


			break;

		case Direction::Right:
			target_x += 1;


			break;

		case Direction::Down:
			target_y += 1;


			break;

		case Direction::Left:
			target_x -= 1;


			break;
	}

	if (!s.emf->Walkable(target_x, target_y) || this->Occupied(target_x, target_y))
	{
		return WalkFail;
	}
	return WalkOK;
}

