#pragma once
#include "itch.h"

namespace Orderbook
{

class Sidebook
{
public:


private:
    float  best_price = 0;
    Itch::Order::Volume volume_at_best_price = 0;
};

}
