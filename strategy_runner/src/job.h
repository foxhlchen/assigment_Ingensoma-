#ifndef JOB_H_
#define JOB_H_

#include <vector>
#include "datadef.h"
#include <cfloat>
#include <map>

class JobCtrl;

class Job {
public:

    bool Init(JobCtrl* control);

    // Interfaces for strategies to implement
    virtual void OnStart() = 0;
    virtual void OnTick(TickData& tickdata) = 0;
    virtual void OnFinished() = 0;

    // market data related functions
    void Subscribe(std::string symbol);

    // trade related functions
    long Trade(std::string symbol, TradeDirection direct, long qty, double price);
    void CancelOrder(long order_sn);
    long BuyMarket(std::string symbol, long qty) { Buy(symbol, qty, DBL_MAX); }
    long SellMarket(std::string symbol, long qty) { Sell(symbol, qty, DBL_MIN); }
    long Buy(std::string symbol, long qty, double price) { Trade(symbol, STOCK_BUY, qty, price); }
    long Sell(std::string symbol, long qty, double price) { Trade(symbol, STOCK_SELL, qty, price); }
    void GetOrders(std::string symbol, std::vector<Order>& orders, OrderStatus status = kAll);
    void SetInitialBalance(long value);
    std::map<std::string, Holding>& GetPosition();

private:
    JobCtrl* ctrl_ = nullptr;

}; // class Job

#endif //JOB_H_