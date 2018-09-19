#ifndef JOBCTRL_H_
#define JOBCTRL_H_

#include "datadef.h"
#include <vector>
#include <string>
#include "datafeed.h"
#include <map>
#include <list>

class Job;

class JobCtrl {
public:
    bool Init(std::string jobname);
    void Run();

    long Trade(std::string symbol, TradeDirection direct, long qty, double price);
    void CancelOrder(long order_sn);
    void Subscribe(std::string symbol);
    void GetOrders(std::string symbol, std::vector<Order>& orders, OrderStatus status = kAll);
    std::map<std::string, Holding>& GetPosition();

    void SetInitialBalance(long value) { initial_balance_ = value; }

    void Plot();

    void SaveDataTable();

private:
    void MatchOrders(TickData& tick);
    void RecordProfitLoss(TickData& tick);

    std::map<std::string, DataFeed> datafeed_;
    std::map<long, Order> orders_;
    std::map<std::string, Holding> position_;
    long orderseq_ = 0;
    long transeq_ = 0;
    long _time_hhmmss_;
    Job* job_ = nullptr;
    long initial_balance_ = 0;

    std::string strategy_name_;
    std::map<std::string, std::list<Equity>> equity_history_;
}; // class JobCtrl

#endif //JOBCTRL_H_