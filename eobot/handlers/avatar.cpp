

#include "handlers.hpp"
#include "../singleton.hpp"

void Avatar_Remove(PacketReader reader)
{
    S &s = S::GetInstance();

    short gameworld_id = reader.GetShort();

    if(reader.Remaining() > 0)
    {
        reader.GetByte(); // admin warp animation ID
    }

	s.signalPlayerRemove.emit(s.map.characters[s.map.GetCharacterIndex(gameworld_id)]);
    s.map.RemoveCharacter(gameworld_id);
}
