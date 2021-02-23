#include "mapitem.hpp"

Map_Item::Map_Item(unsigned short uid, unsigned short id, unsigned int amount, unsigned char x, unsigned char y) :
    uid(uid),
    id(id),
    amount(amount),
    x(x), y(y)
{
    //data = shared_ptr<EIF_Data>(new EIF_Data(s.eif->Get(id)));
}
