#ifndef JOBCTRL_H_
#define JOBCTRL_H_

#include "datadef.h"
#include <vector>
#include <string>
#include "datafeed.h"
#include <map>

class JobCtrl {
public:
    bool Init(std::string jobname);
    bool Run();

    long Trade(TradeDirection direct, long qty, double price);
    void Subscribe(std::string symbol);
    void GetOrders(std::vector<Order>& orders, OrderStatus status = kAll);
    void GetPosition(std::vector<Holding>& position);


private:
     DataFeed datafeed_;
     std::vector<Order> orders_;
     std::vector<Holding> positon_;
     long orderseq = 0;
     long trasseq = 0;

}; // class JobCtrl

#endif JOBCTRL_H_