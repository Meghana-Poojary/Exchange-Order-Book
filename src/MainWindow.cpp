
#include "MainWindow.hpp"

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>
#include <QAbstractItemView>
#include <QDateTime>

#include <chrono>
#include <limits>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    refreshOrderBook();
    refreshTradeHistory();
}

void MainWindow::setupUI() {

    setWindowTitle("Exchange Order Book");
    resize(1200, 800);

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // Title
    QLabel *title = new QLabel("EXCHANGE ORDER BOOK");
    title->setStyleSheet(
        "font-size: 26px;"
        "font-weight: bold;"
        "padding: 10px;"
    );

    mainLayout->addWidget(title);

    // Status
    statusLabel = new QLabel("Market Status: Ready");
    mainLayout->addWidget(statusLabel);

    // Order entry form
    QLabel *orderTitle = new QLabel("PLACE ORDER");
    orderTitle->setStyleSheet("font-size: 18px; font-weight: bold;");

    mainLayout->addWidget(orderTitle);

    QHBoxLayout *orderLayout = new QHBoxLayout();

    sideInput = new QComboBox();
    sideInput->addItem("BUY");
    sideInput->addItem("SELL");

    priceInput = new QDoubleSpinBox();
    priceInput->setRange(0.01, 10000000.00);
    priceInput->setDecimals(2);
    priceInput->setValue(150.00);
    priceInput->setPrefix("₹ ");

    quantityInput = new QSpinBox();
    quantityInput->setRange(1, 1000000);
    quantityInput->setValue(10);

    QPushButton *placeButton = new QPushButton("Place Order");

    orderLayout->addWidget(new QLabel("Side:"));
    orderLayout->addWidget(sideInput);

    orderLayout->addWidget(new QLabel("Price:"));
    orderLayout->addWidget(priceInput);

    orderLayout->addWidget(new QLabel("Quantity:"));
    orderLayout->addWidget(quantityInput);

    orderLayout->addWidget(placeButton);

    mainLayout->addLayout(orderLayout);

    // Cancellation section
    QHBoxLayout *cancelLayout = new QHBoxLayout();

    cancelIdInput = new QSpinBox();
    cancelIdInput->setRange(1, std::numeric_limits<int>::max());

    QPushButton *cancelButton = new QPushButton("Cancel Order");
    QPushButton *matchButton = new QPushButton("Match Pending Orders");

    cancelLayout->addWidget(new QLabel("Order ID:"));
    cancelLayout->addWidget(cancelIdInput);
    cancelLayout->addWidget(cancelButton);
    cancelLayout->addWidget(matchButton);

    mainLayout->addLayout(cancelLayout);

    // Order book table
    QLabel *bookTitle = new QLabel("LIVE ORDER BOOK");
    bookTitle->setStyleSheet("font-size: 18px; font-weight: bold;");

    mainLayout->addWidget(bookTitle);

    orderBookTable = new QTableWidget(0, 5);

    orderBookTable->setHorizontalHeaderLabels({
        "Side",
        "Price",
        "Order ID",
        "Quantity",
        "Remaining"
    });

    orderBookTable->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch
    );

    orderBookTable->setEditTriggers(
        QAbstractItemView::NoEditTriggers
    );

    orderBookTable->setSelectionBehavior(
        QAbstractItemView::SelectRows
    );

    mainLayout->addWidget(orderBookTable);

    // Trade history table
    QLabel *tradeTitle = new QLabel("TRADE HISTORY");
    tradeTitle->setStyleSheet("font-size: 18px; font-weight: bold;");

    mainLayout->addWidget(tradeTitle);

    tradeTable = new QTableWidget(0, 6);

    tradeTable->setHorizontalHeaderLabels({
        "Trade ID",
        "Buy Order",
        "Sell Order",
        "Price",
        "Quantity",
        "Timestamp"
    });

    tradeTable->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch
    );

    tradeTable->setEditTriggers(
        QAbstractItemView::NoEditTriggers
    );

    mainLayout->addWidget(tradeTable);

    // Button connections
    connect(placeButton, &QPushButton::clicked,
            this, [this]() { placeOrder(); });

    connect(cancelButton, &QPushButton::clicked,
            this, [this]() { cancelOrder(); });

    connect(matchButton, &QPushButton::clicked,
            this, [this]() { matchOrders(); });
}


// PLACE ORDER
void MainWindow::placeOrder() {

    OrderSide side =
        (sideInput->currentIndex() == 0)
        ? OrderSide::Buy
        : OrderSide::Sell;

    // Convert rupees into paise
    std::int64_t pricePaise =
        static_cast<std::int64_t>(
            priceInput->value() * 100.0 + 0.5
        );

    int quantity = quantityInput->value();

    Order order(
        nextOrderId++,
        side,
        pricePaise,
        quantity
    );

    std::uint64_t placedId = order.getId();

    orderBook.addOrder(order);

    // Automatically execute matching orders
    matchingEngine.match(orderBook);

    statusLabel->setText(
        QString("Order #%1 placed successfully.")
            .arg(static_cast<qulonglong>(placedId))
    );
    statusLabel->setStyleSheet("color: green; font-weight: bold;");
    refreshOrderBook();
    refreshTradeHistory();
}


// CANCEL ORDER
void MainWindow::cancelOrder() {

    std::uint64_t orderId =
        static_cast<std::uint64_t>(cancelIdInput->value());

    bool cancelled = orderBook.cancelOrder(orderId);

    if (cancelled) {
        statusLabel->setText(
            QString("Order #%1 cancelled.")
                .arg(static_cast<qulonglong>(orderId))
        );

        statusLabel->setStyleSheet("color: red; font-weight: bold;");
    }
    else {
        statusLabel->setText(
            QString("Order #%1 not found.")
                .arg(static_cast<qulonglong>(orderId))
        );

        statusLabel->setStyleSheet("color: orange; font-weight: bold;");
    }

    refreshOrderBook();
}


// MANUAL MATCHING
void MainWindow::matchOrders() {

    matchingEngine.match(orderBook);

    statusLabel->setText("Matching completed.");

    refreshOrderBook();
    refreshTradeHistory();
}


// REFRESH ORDER BOOK
void MainWindow::refreshOrderBook() {

    orderBookTable->setRowCount(0);

    // Display BUY orders
    for (const auto& [price, orders] : orderBook.getBids()) {

        for (const auto& order : orders) {

            int row = orderBookTable->rowCount();
            orderBookTable->insertRow(row);

            orderBookTable->setItem(
                row, 0, new QTableWidgetItem("BUY")
            );

            orderBookTable->setItem(
                row, 1,
                new QTableWidgetItem(
                    QString::number(price / 100.0, 'f', 2)
                )
            );

            orderBookTable->setItem(
                row, 2,
                new QTableWidgetItem(
                    QString::number(
                        static_cast<qulonglong>(order.getId())
                    )
                )
            );

            orderBookTable->setItem(
                row, 3,
                new QTableWidgetItem(
                    QString::number(order.getQuantity())
                )
            );

            orderBookTable->setItem(
                row, 4,
                new QTableWidgetItem(
                    QString::number(order.getRemainingQuantity())
                )
            );
        }
    }

    // Display SELL orders
    for (const auto& [price, orders] : orderBook.getAsks()) {

        for (const auto& order : orders) {

            int row = orderBookTable->rowCount();
            orderBookTable->insertRow(row);

            orderBookTable->setItem(
                row, 0, new QTableWidgetItem("SELL")
            );

            orderBookTable->setItem(
                row, 1,
                new QTableWidgetItem(
                    QString::number(price / 100.0, 'f', 2)
                )
            );

            orderBookTable->setItem(
                row, 2,
                new QTableWidgetItem(
                    QString::number(
                        static_cast<qulonglong>(order.getId())
                    )
                )
            );

            orderBookTable->setItem(
                row, 3,
                new QTableWidgetItem(
                    QString::number(order.getQuantity())
                )
            );

            orderBookTable->setItem(
                row, 4,
                new QTableWidgetItem(
                    QString::number(order.getRemainingQuantity())
                )
            );
        }
    }
}


// REFRESH TRADE HISTORY
void MainWindow::refreshTradeHistory() {

    const auto& trades = matchingEngine.getTradeHistory();

    tradeTable->setRowCount(0);

    for (const auto& trade : trades) {

        int row = tradeTable->rowCount();
        tradeTable->insertRow(row);

        auto time = std::chrono::system_clock::to_time_t(
            trade.getTimestamp()
        );

        QDateTime dateTime =
            QDateTime::fromSecsSinceEpoch(
                static_cast<qint64>(time)
            );

        tradeTable->setItem(
            row, 0,
            new QTableWidgetItem(
                QString::number(
                    static_cast<qulonglong>(trade.getTradeId())
                )
            )
        );

        tradeTable->setItem(
            row, 1,
            new QTableWidgetItem(
                QString::number(
                    static_cast<qulonglong>(trade.getBuyOrderId())
                )
            )
        );

        tradeTable->setItem(
            row, 2,
            new QTableWidgetItem(
                QString::number(
                    static_cast<qulonglong>(trade.getSellOrderId())
                )
            )
        );

        tradeTable->setItem(
            row, 3,
            new QTableWidgetItem(
                QString("₹ %1").arg(
                    trade.getPrice() / 100.0, 0, 'f', 2
                )
            )
        );

        tradeTable->setItem(
            row, 4,
            new QTableWidgetItem(
                QString::number(trade.getQuantity())
            )
        );

        tradeTable->setItem(
            row, 5,
            new QTableWidgetItem(
                dateTime.toString("yyyy-MM-dd HH:mm:ss")
            )
        );
    }
}
