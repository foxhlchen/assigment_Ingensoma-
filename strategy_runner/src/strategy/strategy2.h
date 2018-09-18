#ifndef STRATEGY2_H_
#define STRATEGY2_H_

#include "../job.h"
#include <deque>

class Strategy2 : public Job {
public:
    virtual void OnStart();
    virtual void OnTick(TickData& tickdata);
    virtual void OnFinished();

    void ClearPosition();
private:
    std::deque<TickData> tickcache_;
    long stopbuy_until_;
    long stopsell_until_;
    double balance_;
    bool safelimit3 = false;
};

#endif //STRATEGY2_H_



