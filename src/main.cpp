#include <iostream>

#include "Order.hpp"
#include "OrderBook.hpp"
#include "MatchingEngine.hpp"

int main() {

    OrderBook book;

    Order order1(101, OrderSide::Buy, 15025, 10);
    Order order2(102, OrderSide::Buy, 15000, 20);
    Order order3(103, OrderSide::Buy, 15025, 5);

    Order order4(104, OrderSide::Sell, 15100, 8);
    Order order5(105, OrderSide::Sell, 15000, 21);

    // New order that crosses the best ask
    Order order6(106, OrderSide::Buy, 15050, 7);

    book.addOrder(order1);
    book.addOrder(order2);
    book.addOrder(order3);
    book.addOrder(order4);
    book.addOrder(order5);
    book.addOrder(order6);

    std::cout << "\n========== ORIGINAL ORDER BOOK ==========\n";
    book.printBook();

    MatchingEngine engine;

    engine.match(book);

    std::cout << "\n========== UPDATED ORDER BOOK ==========\n";
    book.printBook();

    std::cout << "\n========== CANCELLATION ==========\n";

    bool cancelled = book.cancelOrder(102);
    if (cancelled) {
        std::cout << "Order cancelled successfully." << std::endl;
    } 
    
    std::cout << "\n========== ORDER BOOK AFTER CANCELLATION ==========\n";

    book.printBook();

    engine.printTradeHistory();

    return 0;
}