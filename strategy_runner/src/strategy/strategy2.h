#ifndef STRATEGY2_H_
#define STRATEGY2_H_

#include "../job.h"

class Strategy2 : public Job {
    virtual void OnStart();
    virtual void OnTick(TickData tickdata);
    virtual void OnFinished();
};

#endif //STRATEGY2_H_