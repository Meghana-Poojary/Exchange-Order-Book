#include "Trade.hpp"
using namespace std;

Trade::Trade(
    uint64_t tradeId,
    uint64_t buyOrderId,
    uint64_t sellOrderId,
    int64_t price,
    int quantity
)
    : tradeId(tradeId),
      buyOrderId(buyOrderId),
      sellOrderId(sellOrderId),
      price(price),
      quantity(quantity),
      timestamp(chrono::system_clock::now())
{
}

uint64_t Trade::getTradeId() const {
    return tradeId;
}

uint64_t Trade::getBuyOrderId() const {
    return buyOrderId;
}

uint64_t Trade::getSellOrderId() const {
    return sellOrderId;
}

int64_t Trade::getPrice() const {
    return price;
}

int Trade::getQuantity() const {
    return quantity;
}

chrono::system_clock::time_point Trade::getTimestamp() const {
    return timestamp;
}