#ifndef STRATEGY1_H_
#define STRATEGY1_H_

#include "../job.h"
#include <deque>

class Strategy1 : public Job {
public:
    virtual void OnStart();
    virtual void OnTick(TickData& tickdata);
    virtual void OnFinished();

    void ClearPosition();
private:
    std::deque<TickData> tickcache_;
    long stopbuy_until_ = 0;
    long stopsell_until_ = 0;
    double balance_ = 0;
    bool safelimit3 = false;
};

#endif //STRATEGY1_H_