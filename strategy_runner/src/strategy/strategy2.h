#ifndef STRATEGY2_H_
#define STRATEGY2_H_

#include "../job.h"
#include <deque>

class Strategy2 : public Job {
public:
    virtual void OnStart();
    virtual void OnTick(TickData& tickdata);
    virtual void OnFinished();

private:
    std::deque<TickData> tickcache_;
};

#endif //STRATEGY2_H_