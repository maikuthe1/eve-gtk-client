// Supress unused variable warning
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"


#ifndef HANDLERS_HPP_INCLUDED
#define HANDLERS_HPP_INCLUDED

#include "../packet.hpp"

void INIT_INIT(PacketReader reader);
void Connection_Player(PacketReader reader);
void Login_Reply(PacketReader reader);
void Welcome_Reply(PacketReader reader);

void Avatar_Remove(PacketReader reader); // Character disappears
void Players_Agree(PacketReader reader); // Characters appears in range
void Walk_Player(PacketReader reader);
void Face_Player(PacketReader reader);
void Walk_Reply(PacketReader reader);

void Talk_Player(PacketReader reader); // public message
void Talk_Tell(PacketReader reader); // private message
void Talk_Message(PacketReader reader); // global message
void Talk_Party(PacketReader reader); // global message
void Talk_Guild(PacketReader reader);

void Sit_Player(PacketReader reader);
void Sit_Close(PacketReader reader);

void Trade_Request(PacketReader reader);
void Trade_Open(PacketReader reader);
void Trade_Close(PacketReader reader);
void Trade_Reply(PacketReader reader);
void Trade_Spec(PacketReader reader);
void Trade_Agree(PacketReader reader);
void Trade_Use(PacketReader reader);

void Appear_Reply(PacketReader reader); // NPC appears in range
void NPC_Spec(PacketReader reader);
void NPC_Junk(PacketReader reader);
void NPC_Player(PacketReader reader);

void Refresh_Reply(PacketReader reader);

void Item_Drop(PacketReader reader);
void Item_Get(PacketReader reader);
void Emote_Player(PacketReader reader);
void Item_Add(PacketReader reader);
void Item_Remove(PacketReader reader);
void Item_Reply(PacketReader reader);
void Warp_Request(PacketReader reader);
void Effect_Spec(PacketReader reader);
void Shop_Buy(PacketReader reader);
void Talk_Reply(PacketReader reader);
void Party_Request(PacketReader reader);
void Party_Create(PacketReader reader);
void Party_Add(PacketReader reader);
void Party_Remove(PacketReader reader);
void Party_Close(PacketReader reader);
void Party_List(PacketReader reader);
void Party_Agree(PacketReader reader);
void Paperdoll_Agree(PacketReader reader);
void Paperdoll_Remove(PacketReader reader);
void Message_Box(PacketReader reader);

#endif // HANDLERS_HPP_INCLUDED
