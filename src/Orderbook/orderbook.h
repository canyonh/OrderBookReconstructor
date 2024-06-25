#pragma once

#include "side_book.h"
#include "orders.h"

namespace Orderbook
{

class Orderbook
{
public:


private:
    Orders orders;
    Sidebook sell_book;
    Sidebook buy_book;
};

};
