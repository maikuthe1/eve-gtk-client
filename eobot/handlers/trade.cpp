#include "handlers.hpp"
#include "../singleton.hpp"
#include <string>
#include <sstream>
template <typename T>
std::string std::to_string(T value)
{
    std::ostringstream os ;
    os << value ;
    return os.str() ;
}
void Trade_Request(PacketReader reader)
{
    S &s = S::GetInstance();

}

void Trade_Open(PacketReader reader)
{
    S &s = S::GetInstance();

}

void Trade_Close(PacketReader reader) // other player closed trade
{
    S &s = S::GetInstance();

}

void Trade_Reply(PacketReader reader) // update of trade items
{
    S &s = S::GetInstance();

}

void Trade_Spec(PacketReader reader) // trade agree status update
{
    S &s = S::GetInstance();

}

void Trade_Agree(PacketReader reader) // trade agree status update
{
    S &s = S::GetInstance();

}

void Trade_Use(PacketReader reader) // trade finished
{
    S &s = S::GetInstance();

}
