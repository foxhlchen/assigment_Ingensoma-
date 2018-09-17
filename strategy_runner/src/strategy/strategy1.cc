#include "strategy1.h"
#include "../datadef.h"
#include "../strategy_factory.h"
#include <deque>
#include "../util.h"

REG_STRATEGY(Strategy1);

using util::TimeCalculation;

const char* symbol = "60000344";

static double CalculateAverage(std::deque<TickData>& tickcache) {
    double midavg = 0;

    for (auto &tick : tickcache) {
        midavg += (tick.ask_price + tick.bid_price) / 2;
    }

    return midavg / tickcache.size();
}


// span format hhmmss  e.g. 2min = 200
static void CacheData(std::deque<TickData>& tickcache, long span, TickData& tickdata) {
    tickcache.push_back(tickdata);

    for ( ;; ) {
        tickcache.front().data_time < TimeCalculation(tickdata.data_time, -span);
        tickcache.pop_front();
    }
}

void Strategy1::OnStart() {
    Subscribe(symbol);
}

void Strategy1::OnTick(TickData& tickdata) {
    CacheData(tickcache_, 200, tickdata);

    // wait 120 second to collect sufficient price data
    if (tickdata.data_time <= 90200) {
        return;
    }

    double avg = CalculateAverage(tickcache_);
    double mid = (tickdata.bid_price + tickdata.ask_price) / 2;

    long pos = GetPosition()["60000344"].GetPosition();

    if (tickdata.data_time > stopbuy_until_) {
        if (mid > avg && pos + 10 <= 100) {
            BuyMarket(symbol, 10);
            stopbuy_until_ = TimeCalculation(tickdata.data_time, 2 * 60);
        }
    }

    if (tickdata.data_time > stopsell_until_) {
        if (mid < avg && pos - 10 <= -100) {
            SellMarket(symbol, 10);
            stopbuy_until_ = TimeCalculation(tickdata.data_time, 2 * 60);
        }
    }



}

void Strategy1::OnFinished() {

}