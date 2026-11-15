#include "OrderBook.hpp"
#include <iostream>
using namespace std;

void OrderBook::addOrder(const Order& order) {
    if (order.getSide() == OrderSide::Buy) {
        bids[order.getPrice()].push_back(order);
    } else {
        asks[order.getPrice()].push_back(order);
    }
}

void OrderBook::printBook() const {
    cout << "Bids:" << endl;
    for (const auto& [price, orders] : bids) {
        cout << "Price: " << price << " ||";
        for (const auto& order : orders) {
            cout << " Order ID: " << order.getId() << " | Quantity: " << order.getQuantity() << " | Remaining: " << order.getRemainingQuantity();
            cout << " ||";
        }
        cout << endl;
    }

    cout << "Asks:" << endl;
    for (const auto& [price, orders] : asks) {
        cout << "Price: " << price << " ||";
        for (const auto& order : orders) {
            cout << " Order ID: " << order.getId() << " | Quantity: " << order.getQuantity() << " | Remaining: " << order.getRemainingQuantity();
            cout << " ||";
        }
        cout << endl;
    }
}

bool OrderBook::cancelOrder(uint64_t orderId){
    for (auto it = bids.begin(); it != bids.end(); it++){
        auto& orders = it->second;
        for (auto orderIt = orders.begin(); orderIt != orders.end(); orderIt++){
            if (orderIt->getId() == orderId){
                orders.erase(orderIt);
                if (orders.empty()){
                    bids.erase(it);
                }
                return true;
            }
        }
    }
    for (auto it = asks.begin(); it != asks.end(); it++){
        auto& orders = it->second;
        for (auto orderIt = orders.begin(); orderIt != orders.end(); orderIt++){
            if (orderIt->getId() == orderId){
                orders.erase(orderIt);
                if (orders.empty()){
                    asks.erase(it);
                }
                return true;
            }
        }
    }
    cout << "Order ID " << orderId << " not found for cancellation." << endl;
    return false;
}