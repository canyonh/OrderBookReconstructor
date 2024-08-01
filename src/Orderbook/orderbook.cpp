#include "src/Orderbook/orderbook.h"

namespace OrderbookReconstructor
{

bool Orderbook::AddOrder(const Order& order_in)
{
    if (!orders.Insert(order_in))
        return false;

    GetSidebook(order_in.side).AddOrder(order_in);
    return true;
}

bool Orderbook::DeleteOrder(Order::OrderId order_id_in)
{
    // Need to search for order so we can efficiently delete the order
    auto* order = orders.Find(order_id_in);
    if (!order)
        return false;
    assert(order->order_id == order_id_in);

    DeleteOrder(*order);
    return true;
}

void Orderbook::DeleteOrder(const Order& order_in)
{
    GetSidebook(order_in.side).DeleteOrder(order_in.order_id, order_in.price, order_in.time_stamp);
    orders.Remove(order_in.order_id);
}

bool Orderbook::CancelOrder(Order::OrderId order_id_in, Order::Volume vol_in)
{
    auto* order = orders.Find(order_id_in);
    if (!order)
        return false;
    assert(order->order_id == order_id_in);

    if (vol_in == order->volume)
        return DeleteOrder(order_id_in);

    GetSidebook(order->side).CancelOrder(order_id_in, order->price, order->time_stamp, order->volume);
    return true;
}

bool Orderbook::ReplaceOrder(Order::OrderId old_order_id, Order::OrderId new_order_id, Order::Volume volume, Order::Price price)
{
    // @todo error handling and (optionally) transaction behavior if not affecting performance?
    // @todo we could do it more optimally later
    Order* removed = orders.Find(old_order_id);
    if (removed == nullptr)
        return false;

    Order new_order(*removed);
    DeleteOrder(new_order);
    new_order.order_id = new_order_id;
    new_order.volume = volume;
    new_order.price = price;

    return AddOrder(new_order);
}

bool Orderbook::ExecuteOrder(Order::OrderId order_id, Order::Volume volume)
{
    assert(!"@writeme");
    return false;
}

bool Orderbook::ExecuteOrderWithPrice(Order::OrderId order_id, Order::Volume volume, Order::Price price)
{
    assert(!"@writeme");
    return false;
}

}
