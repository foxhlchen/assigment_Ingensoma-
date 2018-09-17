#include "jobctrl.h"
#include "job.h"
#include "strategy_factory.h"

bool JobCtrl::Init(std::string jobname) {
    job_ = StrategyFactor::GetStrategy(jobname);
    job_->Init(this);

    job_->OnStart();
}

bool JobCtrl::Run() {
    if (job_ == nullptr) 
        return;

    for (;;) {
        TickData tickdata;
        if ( !datafeed_.GetNextTick(tickdata) ) {
            break;
        }

        job_->OnTick(tickdata);
    }

    job_->OnFinished();
}

void JobCtrl::Subscribe(std::string symbol) {
    datafeed_.Init(symbol);
}

long JobCtrl::Trade(TradeDirection direct, long qty, double price) {
    Order order;

    order.order_sn = orderseq++;
    order.symbol = datafeed_.get_symbol();
    order.order_price = price;
    order.order_qty = qty;
    order.direct = direct;
    order.order_time = _time_hhmmss_;

    orders_[order.order_sn] = order;

    return order.order_sn;
}


void JobCtrl::CancelOrder(long order_sn) {
    if (orders_.find(order_sn) != orders_.end()) {
        orders_[order_sn].Cancel();
    }
}

void JobCtrl::GetOrders(std::vector<Order>& orders, OrderStatus status = kAll) {
    for (auto& it: orders_) {
        Order &order = it.second;

        if (status != kAll && order.GetStatus() != status) {
            continue;
        }

        orders.push_back(order);
    }
}