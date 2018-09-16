#ifndef JOB_H_
#define JOB_H_

#include <vector>
#include "datadef.h"


class JobCtrl;

class Job {
public:

    bool Init(JobCtrl* control);

    // Interface for strategies
    virtual void OnStart() = 0;
    virtual void OnTick(TickData tickdata) = 0;
    virtual void OnFinished() = 0;

    // market data related functions
    void Subscribe(std::string symbol);

    // trade related functions
    long Trade(TradeDirection direct, long qty, double price);
    long Buy(long qty, double price) { Trade(STOCK_BUY, qty, price); }
    long Sell(long qty, double price) { Trade(STOCK_SELL, qty, price); }
    

private:
    JobCtrl* ctrl_;

}; // class Job

#endif JOB_H_