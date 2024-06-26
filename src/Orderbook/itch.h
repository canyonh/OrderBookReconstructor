#pragma once

#include <cstdint>

// type alias for Nasdaq ITCH protocol
namespace Itch
{

using MsgSizeType = uint8_t;
using Int2 = uint16_t;
using Int4 = uint32_t;
using Int6 = uint64_t;
using Int8 = uint64_t;
using Alpha = char;

struct Order
{
    using OrderId = Int8;
    using StockId = Int4;
    using Timestamp = Int8;
    using Volume = Int4;
    using Price = float;
    using UserRef = Int4;

    enum class Side : Alpha {Buy = 'B', Sell = 'S'};

    StockId stock_id;
    OrderId order_id;
    Timestamp time_stamp;
    Side side;
    Price price;
    Volume volume;
    UserRef user_ref;

    bool operator==(const Order& rhs) const = default;
};



};
