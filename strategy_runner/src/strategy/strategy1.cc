#include "strategy1.h"
#include "../datadef.h"
#include "../strategy_factory.h"
#include <deque>
#include "../util.h"
#include <vector>
#include <iostream>

REG_STRATEGY(Strategy1);

using util::TimeCalculation;
using std::vector;
using std::cout;
using std::endl;

static const char* kSymbol = "60000344";
static const double kBalance = 100000;

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
        if (tickcache.front().data_time > TimeCalculation(tickdata.data_time, -span))
            break;
        
        tickcache.pop_front();
    }
}

void Strategy1::ClearPosition() {
    Holding &hold = GetPosition()[kSymbol];
    vector<Order> orders;
    GetOrders(kSymbol, orders, OrderStatus::kSubmitted);
    GetOrders(kSymbol, orders, OrderStatus::kPartial);

    for (auto &order: orders) {
        order.Cancel();
    }

    // close buy order
    if (hold.exposure > 0) {
        SellMarket(kSymbol, hold.exposure);
    }

    // close sell order
    if (hold.exposure < 0) {
        BuyMarket(kSymbol, -hold.exposure);
    }
}

void Strategy1::OnStart() {
    Subscribe(kSymbol);
    balance_ = kBalance;
    SetInitialBalance(balance_);
}

void Strategy1::OnTick(TickData& tickdata) {
    if (balance_ < 0) {
        return; // congratulations! you go bankrupt.
    }


    CacheData(tickcache_, 200, tickdata);

    // wait 120 second to collect sufficient price data
    if (tickdata.data_time <= 90200) {
        return;
    }

    double avg = CalculateAverage(tickcache_);
    double mid = (tickdata.bid_price + tickdata.ask_price) / 2;

    //cout << LOGVAR(avg) << LOGVAR(mid) << LOGVAR((mid > avg)) << endl;

    Holding &hold = GetPosition()[kSymbol];
    long pos = hold.exposure;

    if (safelimit3) {
        if (tickdata.data_time <= stopbuy_until_)
            return;

        balance_ = hold.equity;
        safelimit3 = false;

        cout << "lift safelimit3" <<endl;
        cout << LOGVAR(hold.equity) << LOGVAR(balance_) << endl;
    }

    if (hold.equity / balance_ < 0.98) {
        cout << LOGVAR(hold.equity) << LOGVAR(balance_) << endl;
        cout << "trigger safelimit3" <<endl;
        
        safelimit3 = true;
        stopbuy_until_ = TimeCalculation(tickdata.data_time, 10 * 60);
        stopsell_until_ = TimeCalculation(tickdata.data_time, 10 * 60);
        ClearPosition();
    }

    if (tickdata.data_time > stopbuy_until_) {
        if (mid > avg && pos + 10 <= 100) {
            BuyMarket(kSymbol, 10);
            stopbuy_until_ = TimeCalculation(tickdata.data_time, 2 * 60);
        }
    }

    if (tickdata.data_time > stopsell_until_) {
        if (mid < avg && pos - 10 >= -100) {
            SellMarket(kSymbol, 10);
            stopsell_until_ = TimeCalculation(tickdata.data_time, 2 * 60);
        }
    }
}

void Strategy1::OnFinished() {

}