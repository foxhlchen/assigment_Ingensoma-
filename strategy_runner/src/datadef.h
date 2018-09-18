#ifndef DATADEF_H
#define DATADEF_H

#include <string>
#include <vector>
#include <sstream>


#define REG_STRATEGY(strategy)\
class Creator_##strategy : public StrategyCreator \
{\
public: \
Creator_##strategy() { StrategyFactor::RegisterStrategy(#strategy, this); } \
Job* Create() { return new strategy(); }\
};\
Creator_##strategy strategy##_creator_instance;



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

struct Equity{
    long data_time = 0;
    double profitloss = 0;
    double equity = 0;
};

struct Holding {
    std::string symbol;
    long buy_qty = 0;
    long sell_qty = 0;
    double buy_amount = 0;
    double sell_amount = 0;

    //calculation result
    double balance = 0;
    long exposure = 0;
    double unclosed_value = 0;
    double profitloss = 0;
    double equity = 0;
};

struct Trasaction {
    long trans_sn = 0;

    std::string symbol = 0;
    TradeDirection direct;
    long time = 0;
    long qty = 0;
    double price = 0;
};

struct Order {
    long order_sn = 0;
    long order_time = 0;

    std::string symbol;
    TradeDirection direct;
    long order_qty = 0;
    double order_price = 0;

    long filled_qty = 0;
    double total_amount = 0;
    double avg_filled_price = 0;

    std::vector<Trasaction> transactions;

    void Cancel() {
        is_cancelled = true;
    }

    bool IsCancelled() {
        return is_cancelled;
    }

    bool IsDone() {
        return filled_qty == order_qty;
    }

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

    long data_time = 0;

    long bid_qty = 0;
    long ask_qty = 0;

    double bid_price = 0;
    double ask_price = 0;

    bool Feed(std::string& line) {
        std::istringstream iss(line);

        //return iss >> data_time >> ask_qty >> bid_qty >> bid_price >> ask_price;
        if(iss >> data_time >> ask_qty >> bid_qty >> bid_price >> ask_price)
            return true;

        return false;
    }
};



#endif DATADEF_H_