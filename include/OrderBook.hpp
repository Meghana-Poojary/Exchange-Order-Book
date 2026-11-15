#ifndef ORDERBOOK_HPP
#define ORDERBOOK_HPP

#include "order.hpp"

#include <map>
#include <deque>
#include <functional>

using namespace std;

class OrderBook {
private:
    map<int64_t, deque<Order>, greater<int64_t>> bids;
    map<int64_t, deque<Order>> asks;
    friend class MatchingEngine;

public:
    void addOrder(const Order& order);
    void printBook() const;
    bool cancelOrder(uint64_t orderId);
        // Read-only access for the frontend
    const auto& getBids() const {
        return bids;
    }

    const auto& getAsks() const {
        return asks;
    }
};

#endif