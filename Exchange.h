#pragma once
#include <string>
#include <vector>
#include <map>

// Order structure
struct Order {
    std::string clientId;
    std::string orderId;
    std::string instrument;
    int side;      // 1=Buy, 2=Sell
    double price;
    int qty;
    long long seq; // FIFO
};

// Execution report structure
struct Report {
    std::string clientId;
    std::string orderId;
    std::string instrument;
    int side;
    double price;
    int qty;
    int status;          // 0 New, 1 Rejected, 2 Fill, 3 Pfill
    std::string reason;
    std::string time;
};

// One OrderBook = buy list + sell list
struct OrderBook {
    std::vector<Order> buyBook;
    std::vector<Order> sellBook;
};

class Exchange {
public:
    Exchange();

    // Process one order
    std::vector<Report> process(Order o);

private:
    std::map<std::string, OrderBook> books;

    long long orderCounter;
    long long seqCounter;

    std::string newOrderId();

    void sortBooks(std::string ins);
    void addToBook(Order o);

    void matchBuy(Order &buy, std::vector<Order> &sellBook, std::vector<Report> &out);
    void matchSell(Order &sell, std::vector<Order> &buyBook, std::vector<Report> &out);

    Report makeReport(Order o, double price, int qty, int status, std::string reason);
};