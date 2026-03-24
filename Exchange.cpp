#include "Exchange.h"
#include "Utils.h"
#include <algorithm>

Exchange::Exchange() {
    orderCounter = 1;
    seqCounter = 1;
}

std::string Exchange::newOrderId() {
    return "ord" + std::to_string(orderCounter++);
}

Report Exchange::makeReport(Order o, double price, int qty, int status, std::string reason) {
    Report r;
    r.clientId = o.clientId;
    r.orderId = o.orderId;
    r.instrument = o.instrument;
    r.side = o.side;
    r.price = price;
    r.qty = qty;
    r.status = status;
    r.reason = reason;
    r.time = getTimeStamp();
    return r;
}

void Exchange::sortBooks(std::string ins) {
    if (books.find(ins) == books.end()) {
        books[ins] = OrderBook();
    }

    auto &buyBook = books[ins].buyBook;
    auto &sellBook = books[ins].sellBook;

    // Buy: highest price first
    std::sort(buyBook.begin(), buyBook.end(), [](Order a, Order b) {
        if (a.price != b.price) return a.price > b.price;
        return a.seq < b.seq;
    });

    // Sell: lowest price first
    std::sort(sellBook.begin(), sellBook.end(), [](Order a, Order b) {
        if (a.price != b.price) return a.price < b.price;
        return a.seq < b.seq;
    });
}

void Exchange::addToBook(Order o) {
    if (books.find(o.instrument) == books.end()) {
        books[o.instrument] = OrderBook();
    }

    if (o.side == 1) books[o.instrument].buyBook.push_back(o);
    else books[o.instrument].sellBook.push_back(o);

    sortBooks(o.instrument);
}

void Exchange::matchBuy(Order &buy, std::vector<Order> &sellBook, std::vector<Report> &out) {
    while (buy.qty > 0 && !sellBook.empty()) {
        Order &bestSell = sellBook[0];

        if (buy.price < bestSell.price) break;

        int tradeQty = std::min(buy.qty, bestSell.qty);
        double tradePrice = bestSell.price;

        buy.qty -= tradeQty;
        bestSell.qty -= tradeQty;

        int buyStatus = (buy.qty == 0) ? 2 : 3;
        int sellStatus = (bestSell.qty == 0) ? 2 : 3;

        out.push_back(makeReport(buy, tradePrice, tradeQty, buyStatus, ""));
        out.push_back(makeReport(bestSell, tradePrice, tradeQty, sellStatus, ""));

        if (bestSell.qty == 0) {
            sellBook.erase(sellBook.begin());
        }
    }
}

void Exchange::matchSell(Order &sell, std::vector<Order> &buyBook, std::vector<Report> &out) {
    while (sell.qty > 0 && !buyBook.empty()) {
        Order &bestBuy = buyBook[0];

        if (sell.price > bestBuy.price) break;

        int tradeQty = std::min(sell.qty, bestBuy.qty);
        double tradePrice = bestBuy.price;

        sell.qty -= tradeQty;
        bestBuy.qty -= tradeQty;

        int sellStatus = (sell.qty == 0) ? 2 : 3;
        int buyStatus = (bestBuy.qty == 0) ? 2 : 3;

        out.push_back(makeReport(sell, tradePrice, tradeQty, sellStatus, ""));
        out.push_back(makeReport(bestBuy, tradePrice, tradeQty, buyStatus, ""));

        if (bestBuy.qty == 0) {
            buyBook.erase(buyBook.begin());
        }
    }
}

std::vector<Report> Exchange::process(Order o) {
    std::vector<Report> out;

    // Give every order an order ID first
    o.orderId = newOrderId();
    o.seq = seqCounter++;

    // Validate AFTER assigning order ID
    std::string reason;
    bool valid = validateOrder(o.clientId, o.instrument, o.side, o.price, o.qty, reason);

    if (!valid) {
        out.push_back(makeReport(o, o.price, o.qty, 1, reason));
        return out;
    }

    sortBooks(o.instrument);

    auto &buyBook = books[o.instrument].buyBook;
    auto &sellBook = books[o.instrument].sellBook;

    int originalQty = o.qty;

    if (o.side == 1) matchBuy(o, sellBook, out);
    else matchSell(o, buyBook, out);

    if (o.qty > 0) {
        if (o.qty == originalQty) {
            out.push_back(makeReport(o, o.price, o.qty, 0, ""));
        }
        addToBook(o);
    }

    return out;
}