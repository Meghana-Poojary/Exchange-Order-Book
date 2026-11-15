
#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <cstdint>

#include "OrderBook.hpp"
#include "MatchingEngine.hpp"

class QComboBox;
class QDoubleSpinBox;
class QSpinBox;
class QTableWidget;
class QLabel;
class QPushButton;

class MainWindow : public QMainWindow {
public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    OrderBook orderBook;
    MatchingEngine matchingEngine;

    std::uint64_t nextOrderId = 101;

    QComboBox *sideInput;
    QDoubleSpinBox *priceInput;
    QSpinBox *quantityInput;
    QSpinBox *cancelIdInput;

    QTableWidget *orderBookTable;
    QTableWidget *tradeTable;

    QLabel *statusLabel;

    void setupUI();
    void placeOrder();
    void cancelOrder();
    void matchOrders();
    void refreshOrderBook();
    void refreshTradeHistory();
};

#endif
