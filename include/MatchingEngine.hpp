#ifndef MATCHINGENGINE_HPP
#define MATCHINGENGINE_HPP

#include "OrderBook.hpp"
#include "Trade.hpp"

#include <vector>

class MatchingEngine {

private:
    std::vector<Trade> tradeHistory;
    uint64_t nextTradeId = 1;

public:
    void match(OrderBook& book);
    void printTradeHistory() const;
    const std::vector<Trade>& getTradeHistory() const {
        return tradeHistory;
    }
};

#endif