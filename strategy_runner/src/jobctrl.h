#ifndef JOBCTRL_H_
#define JOBCTRL_H_

#include "datadef.h"

class JobCtrl {
public:
    bool Init();
    bool Run();

    long Trade(TradeDirection direct, long qty, double price);
    void Subscribe(std::string symbol);
    long GetOrders(OrderStatus = kAll);

}; // class JobCtrl

#endif JOBCTRL_H_