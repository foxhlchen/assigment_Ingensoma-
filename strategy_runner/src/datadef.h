#ifndef DATADEF_H
#define DATADEF_H

#include <string>
#include <vector>
#include <sstream>

enum TradeDirection {
    STOCK_BUY = 0,
    STOCK_SELL = 1,
};

enum OrderStatus {
    kAll = 0,
    kSubmitted = 1,
    kPartial = 2,
    kDone = 3,
    kCancelled = 4,
};

struct Holding {
    std::string symbol;
    long qty;

    double avg_price;
};

struct Trasaction {
    long trans_sn;

    std::string symbol;
    TradeDirection direct;
    long time;
    long qty;
    double price;
};

struct Order {
    long order_sn;
    long order_time;

    std::string symbol;
    TradeDirection direct;
    long order_qty;
    double order_price;

    long filled_qty;
    double avg_filled_price;

    std::vector<Trasaction> transactions;

    OrderStatus GetStatus() {
        if (is_cancelled) 
            return kCancelled;

        if (filled_qty == 0) {
            return kSubmitted;
        }

        if (filled_qty == order_qty) {
            return kDone;
        }

        if (filled_qty != order_qty) {
            return kPartial;
        }
    }

private:
    bool is_cancelled = false;
};

struct TickData {
    std::string symbol;

    long data_time;

    long bid_qty;
    long ask_qty;

    double bid_price;
    double ask_price;

    bool Feed(std::string& line) {
        std::istringstream iss(line);

        //return iss >> data_time >> ask_qty >> bid_qty >> bid_price >> ask_price;
        if(iss >> data_time >> ask_qty >> bid_qty >> bid_price >> ask_price)
            return true;

        return false;
    }
};

#endif DATADEF_H_