#include "MatchingEngine.hpp"
#include <iostream>
#include <algorithm>
#include <ctime>
#include <iomanip>

using namespace std;

void MatchingEngine::match(OrderBook& book) {
    while (!book.bids.empty() && !book.asks.empty()){
        auto bidIt = book.bids.begin();
        auto askIt = book.asks.begin();
        if (bidIt->first< askIt->first) {
            break; // No match possible
        }
        Order& bidOrder = bidIt->second.front();
        Order& askOrder = askIt->second.front();

        int tradeQuantity = min(bidOrder.getRemainingQuantity(), askOrder.getRemainingQuantity());
        int64_t tradePrice = askOrder.getPrice(); // Trade at the ask price

        Trade trade(
            nextTradeId++,
            bidOrder.getId(),
            askOrder.getId(),
            tradePrice,
            tradeQuantity
        );

        tradeHistory.push_back(trade);

        cout << "\n========== TRADE EXECUTED ==========\n";
        cout << "Buy Order ID: " << bidOrder.getId() << endl;
        cout << "Sell Order ID: " << askOrder.getId() << endl;
        cout << "Price: " << tradePrice << endl;
        cout << "Quantity: " << tradeQuantity << endl;

        bidOrder.fill(tradeQuantity);
        askOrder.fill(tradeQuantity);

        if (bidOrder.getRemainingQuantity() == 0) {
            bidIt->second.pop_front();
            if (bidIt->second.empty()) {
                book.bids.erase(bidIt);
            }
        }
        if (askOrder.getRemainingQuantity() == 0) {
            askIt->second.pop_front();
            if (askIt->second.empty()) {
                book.asks.erase(askIt);
            }
        }
    }
}

void MatchingEngine::printTradeHistory() const {
    cout << "\n========== TRADE HISTORY ==========\n";
    if (tradeHistory.empty()) {
        cout << "No trades have been executed yet." << endl;
        return;
    }

    for (const auto& trade : tradeHistory) {
        auto time = chrono::system_clock::to_time_t(trade.getTimestamp());
        tm localTime{};
        localtime_s(&localTime, &time);
        cout << "Trade ID: " << trade.getTradeId() << " | ";
        cout << "Buy Order ID: " << trade.getBuyOrderId() << " | ";
        cout << "Sell Order ID: " << trade.getSellOrderId() << " | ";
        cout << "Price: " << trade.getPrice() << " | ";
        cout << "Quantity: " << trade.getQuantity() << " | ";
        cout << "Timestamp: " << put_time(&localTime, "%Y-%m-%d %H:%M:%S") << endl;
    }
}
