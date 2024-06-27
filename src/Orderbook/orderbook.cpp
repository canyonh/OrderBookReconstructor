#include "Orderbook/orderbook.h"

namespace Orderbook
{

bool Orderbook::AddOrder(const Order& order_in)
{
    if (!orders.Insert(order_in))
        return false;

    GetSidebook(order_in.side).AddOrder(order_in);
    return true;
}

bool Orderbook::DeleteOrder(Order::OrderId order_id)
{
    assert(!"@writeme");
}

bool Orderbook::CancelOrder(Order::OrderId order_id)
{
    assert(!"@writeme");
}

bool Orderbook::ReplaceOrder(Order::OrderId old_order_id, Order::OrderId new_order_id, Order::Volume volume, Order::Price price)
{
    assert(!"@writeme");
}

bool Orderbook::ExecuteOrder(Order::OrderId order_id, Order::Volume volume)
{
    assert(!"@writeme");
}

bool Orderbook::ExecuteOrderWithPrice(Order::OrderId order_id, Order::Volume volume, Order::Price price)
{
    assert(!"@writeme");
}

}
