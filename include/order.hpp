
#ifndef ORDER_HPP
#define ORDER_HPP

#include <cstdint>
using namespace std;
enum class OrderSide {
    Buy,
    Sell
};

class Order {
private:
    uint64_t id;
    OrderSide side;
    int64_t price;
    int quantity;
    int remainingQuantity;

public:
    Order(
        uint64_t id,
        OrderSide side,
        int64_t price,
        int quantity
    );

    uint64_t getId() const;
    OrderSide getSide() const;
    int64_t getPrice() const;
    int getQuantity() const;
    int getRemainingQuantity() const;
    void fill(int quantity);
};

#endif
