
# Exchange Order Book

A C++20-based Exchange Order Book and Matching Engine with a Qt GUI.

## Features

- Buy and Sell order placement
- Price-Time Priority matching
- Partial order execution
- Order cancellation
- Trade history with timestamps
- Live order book visualization
- Qt-based graphical interface

## Tech Stack

- C++20
- Qt 6 Widgets
- STL (map, deque, vector)
- CMake
- MinGW / GCC

## Matching Algorithm

The matching engine follows Price-Time Priority:

1. Highest buy price gets priority.
2. Lowest sell price gets priority.
3. Earlier orders get priority at the same price.
4. Orders execute when the highest bid is greater than or equal to the lowest ask.

## Build Instructions

```bash
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build --target exchange_gui
```

## Run

```bash
./build/exchange_gui.exe
```

## Future Improvements

- Persistent order and trade storage
- Market depth visualization
- Improved trading terminal UI
- Real-time market simulation
- Automated unit testing
