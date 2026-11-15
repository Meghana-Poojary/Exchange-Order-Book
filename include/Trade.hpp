#ifndef TRADE_HPP
#define TRADE_HPP

#include <cstdint>
#include <chrono>

using namespace std;

class Trade {

private:
    uint64_t tradeId;
    uint64_t buyOrderId;
    uint64_t sellOrderId;
    int64_t price;
    int quantity;
    chrono::system_clock::time_point timestamp;

public:
    Trade(
        uint64_t tradeId,
        uint64_t buyOrderId,
        uint64_t sellOrderId,
        int64_t price,
        int quantity
    );

    uint64_t getTradeId() const;
    uint64_t getBuyOrderId() const;
    uint64_t getSellOrderId() const;
    int64_t getPrice() const;
    int getQuantity() const;
    chrono::system_clock::time_point getTimestamp() const;
};


#endif