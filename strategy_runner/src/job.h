#ifndef JOB_H_
#define JOB_H_

#include <vector>
#include "datadef.h"
#include <climits>

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
    long Trade(TradeDirection direct, long qty, double price);
    void CancelOrder(long order_sn);
    long BuyMarket(long qty) { Buy(qty, DBL_MAX); }
    long SellMarket(long qty) { Sell(qty, DBL_MIN); }
    long Buy(long qty, double price) { Trade(STOCK_BUY, qty, price); }
    long Sell(long qty, double price) { Trade(STOCK_SELL, qty, price); }
    void GetOrders(std::vector<Order>& orders, OrderStatus status = kAll);
    std::map<std::string, Holding>& GetPosition();

private:
    JobCtrl* ctrl_ = nullptr;

}; // class Job

#endif JOB_H_