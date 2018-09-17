#ifndef STRATEGY1_H_
#define STRATEGY1_H_

#include "../job.h"

class Strategy1 : public Job {
    virtual void OnStart();
    virtual void OnTick(TickData tickdata);
    virtual void OnFinished();
};

#endif //STRATEGY1_H_