#include <gtest/gtest.h>
#include "src/Orderbook/orderbook.h" 

using namespace OrderbookReconstructor;

class OrderbookTest : public ::testing::Test {
protected:
    OrderbookReconstructor::Orderbook book;

    // Helper function to create a sample order
    Order createSampleOrder(Order::OrderId id, Order::Side side, Order::Price price, Order::Volume volume) {
        Order order;
        order.order_id = id;
        order.side = side;
        order.price = price;
        order.volume = volume;
        return order;
    }
};

// Test adding an order
TEST_F(OrderbookTest, AddOrder) {
    Order order = createSampleOrder(1, Order::Side::Buy, 10000, 100);
    EXPECT_TRUE(book.AddOrder(order));
    
    // Try to add the same order again
    EXPECT_FALSE(book.AddOrder(order));
}

// Test deleting an order
TEST_F(OrderbookTest, DeleteOrder) {
    Order order = createSampleOrder(1, Order::Side::Buy, 10000, 100);
    book.AddOrder(order);
    
    EXPECT_TRUE(book.DeleteOrder(1));
    EXPECT_FALSE(book.DeleteOrder(1));  // Order no longer exists
}

// Test cancelling part of an order
TEST_F(OrderbookTest, CancelOrder) {
    Order order = createSampleOrder(1, Order::Side::Buy, 10000, 100);
    book.AddOrder(order);
    
    EXPECT_TRUE(book.CancelOrder(1, 50));
    EXPECT_TRUE(book.CancelOrder(1, 50));  // Cancel remaining volume
    EXPECT_FALSE(book.CancelOrder(2, 1));  // Order should be fully cancelled now
}

// Test replacing an order
TEST_F(OrderbookTest, ReplaceOrder) {
    Order order = createSampleOrder(1, Order::Side::Buy, 10000, 100);
    book.AddOrder(order);
    
    EXPECT_TRUE(book.ReplaceOrder(1, 2, 150, 10100));
    EXPECT_FALSE(book.ReplaceOrder(1, 3, 200, 10200));  // Old order no longer exists
}

// Test executing an order
TEST_F(OrderbookTest, ExecuteOrder) {
    Order order = createSampleOrder(1, Order::Side::Buy, 10000, 100);
    book.AddOrder(order);
    
    EXPECT_TRUE(book.ExecuteOrder(1, 50));
    EXPECT_TRUE(book.ExecuteOrder(1, 50));
    EXPECT_FALSE(book.ExecuteOrder(1, 1));  // Order should be fully executed now
}

// Test executing an order with price
TEST_F(OrderbookTest, ExecuteOrderWithPrice) {
    Order order = createSampleOrder(1, Order::Side::Buy, 10000, 100);
    book.AddOrder(order);
    
    EXPECT_TRUE(book.ExecuteOrderWithPrice(1, 50, 10000));
    EXPECT_FALSE(book.ExecuteOrderWithPrice(1, 51, 10000));  // Not enough volume left
    EXPECT_FALSE(book.ExecuteOrderWithPrice(1, 50, 10001));  // Price mismatch
}

// Test multiple operations on the same order
TEST_F(OrderbookTest, MultipleOperations) {
    Order order = createSampleOrder(1, Order::Side::Buy, 10000, 100);
    book.AddOrder(order);
    
    EXPECT_TRUE(book.CancelOrder(1, 20));
    EXPECT_TRUE(book.ExecuteOrder(1, 30));
    EXPECT_TRUE(book.ReplaceOrder(1, 2, 100, 10100));
    EXPECT_FALSE(book.DeleteOrder(1));  // Old order no longer exists
    EXPECT_TRUE(book.DeleteOrder(2));   // New order should exist
}

// Test operations on non-existent orders
TEST_F(OrderbookTest, NonExistentOrders) {
    EXPECT_FALSE(book.DeleteOrder(1));
    EXPECT_FALSE(book.CancelOrder(1, 50));
    EXPECT_FALSE(book.ReplaceOrder(1, 2, 100, 10000));
    EXPECT_FALSE(book.ExecuteOrder(1, 50));
    EXPECT_FALSE(book.ExecuteOrderWithPrice(1, 50, 10000));
}

// Test adding orders to both sides of the book
TEST_F(OrderbookTest, BothSides) {
    Order buy_order = createSampleOrder(1, Order::Side::Buy, 10000, 100);
    Order sell_order = createSampleOrder(2, Order::Side::Sell, 10100, 100);
    
    EXPECT_TRUE(book.AddOrder(buy_order));
    EXPECT_TRUE(book.AddOrder(sell_order));
    
    EXPECT_TRUE(book.ExecuteOrder(1, 50));
    EXPECT_TRUE(book.ExecuteOrder(2, 50));
}

// Test replacing order with same id
TEST_F(OrderbookTest, ReplaceOrderSameId) {
    Order order = createSampleOrder(1, Order::Side::Buy, 10000, 100);
    book.AddOrder(order);
    
    EXPECT_TRUE(book.ReplaceOrder(1, 1, 150, 10100));
    EXPECT_TRUE(book.ExecuteOrder(1, 150));
    EXPECT_FALSE(book.ExecuteOrder(1, 1));  // Order should be fully executed now
}
