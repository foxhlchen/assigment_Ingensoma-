#include "strategy1.h"
#include "../datadef.h"
#include "../strategy_factory.h"
#include <deque>

REG_STRATEGY(Strategy1);


static double CalculateAverage(std::deque<TickData>& tickcache) {
    
}

// offset format hhmmss  e.g. 2min = 200
static void CacheData(std::deque<TickData>& tickcache, long offset, TickData& tickdata) {
    tickcache.push_back(tickdata);

    for ( ;; ) {
        tickcache.front().data_time < tickdata.data_time - offset;
        tickcache.pop_front();
    }
}

void Strategy1::OnStart() {
    Subscribe("60000344");
}

void Strategy1::OnTick(TickData& tickdata) {
    CacheData(tickcache_, 200, tickdata);

    // wait 120 second to collect sufficient price data
    if (tickdata.data_time <= 90200) {
        return;
    }

    
}

void Strategy1::OnFinished() {

}