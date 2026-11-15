#include "order.hpp"
#include <iostream>

using namespace std;

Order::Order(
    uint64_t id,
    OrderSide side,
    int64_t price,
    int quantity
)
    : id(id),
      side(side),
      price(price),
      quantity(quantity),
      remainingQuantity(quantity)
{
}

uint64_t Order::getId() const {
    return id;
}

OrderSide Order::getSide() const {
    return side;
}

int64_t Order::getPrice() const {
    return price;
}

int Order::getQuantity() const {
    return quantity;
}

int Order::getRemainingQuantity() const {
    return remainingQuantity;
}

void Order::fill(int quantity) {
    if (quantity <= 0 || quantity > remainingQuantity) {
        cout << "Invalid fill quantity: " << quantity << ". Remaining quantity: " << remainingQuantity << endl;
        return;
    }
    remainingQuantity -= quantity;
}
