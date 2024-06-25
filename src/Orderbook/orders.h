#pragma once

#include "itch.h"
#include <unordered_map>

namespace Orderbook
{

class Orders
{
public:


private:
    std::unordered_map<Itch::Order::OrderId, Itch::Order> orders;
};

};
