#include "jobctrl.h"
#include "job.h"
#include "strategy_factory.h"
#include "datadef.h"

bool JobCtrl::Init(std::string jobname) {
    job_ = StrategyFactor::GetStrategy(jobname);

    if (job_ == nullptr)
        return false;

    job_->Init(this);

    job_->OnStart();

    return true;
}

void JobCtrl::Run() {
    if (job_ == nullptr) 
        return;

    for (;;) {
        TickData tickdata;
        if ( !datafeed_.GetNextTick(tickdata) ) {
            break;
        }

        _time_hhmmss_ = tickdata.data_time;

        MatchOrders(tickdata);
        RecordProfitLoss(tickdata);
        job_->OnTick(tickdata);
    }

    job_->OnFinished();
}

void JobCtrl::Subscribe(std::string symbol) {
    datafeed_.Init(symbol);
}

void JobCtrl::RecordProfitLoss(TickData& tick) {
    Equity equity;
    Holding& hold = position_[datafeed_.get_symbol()];

    double balance = hold.sell_amount - hold.buy_amount;
    long exposure = hold.buy_qty - hold.sell_qty;
    // exposure > 0, means in order to close position, you have to sell exposure qty.
    // for exposure < 0 you have to buy stock.
    double profitloss = exposure > 0 ? exposure * tick.bid_price : exposure * tick.ask_price;
    equity.equity = balance + profitloss;

    equity.data_time = tick.data_time;
    equity_book_.push_back(equity);
}

void JobCtrl::MatchOrders(TickData& tick) {
    for (auto& it: orders_) {
        Order &order = it.second;
        if (order.IsCancelled() || order.IsDone())
            continue;

        if (order.direct == STOCK_BUY && order.order_price >= tick.ask_price) {
            long remain_tofill = order.order_qty - order.filled_qty;
            long turnover_vol = tick.ask_qty > remain_tofill ? remain_tofill : tick.ask_qty;
            double amount = tick.ask_price * turnover_vol;

            order.filled_qty += turnover_vol;
            order.total_amount += amount;
            order.avg_filled_price = order.total_amount / order.filled_qty;

            Trasaction trans;
            trans.direct = order.direct;
            trans.price = tick.ask_price;
            trans.qty = turnover_vol;
            trans.symbol = datafeed_.get_symbol();
            trans.time = tick.data_time;
            trans.trans_sn = transeq_++;
            
            order.transactions.push_back(trans);

            position_[datafeed_.get_symbol()].symbol = datafeed_.get_symbol();
            position_[datafeed_.get_symbol()].buy_qty += turnover_vol;
            position_[datafeed_.get_symbol()].buy_amount += amount;
        }
        else if (order.direct == STOCK_SELL && order.order_price <= tick.bid_price) {
            long remain_tofill = order.order_qty - order.filled_qty;
            long turnover_vol = tick.bid_qty > remain_tofill ? remain_tofill : tick.bid_qty;
            double amount = tick.bid_price * turnover_vol;

            order.filled_qty += turnover_vol;
            order.total_amount += amount;
            order.avg_filled_price = order.total_amount / order.filled_qty;

            Trasaction trans;
            trans.direct = order.direct;
            trans.price = tick.bid_price;
            trans.qty = turnover_vol;
            trans.symbol = datafeed_.get_symbol();
            trans.time = tick.data_time;
            trans.trans_sn = transeq_++;
            
            order.transactions.push_back(trans);

            position_[datafeed_.get_symbol()].symbol = datafeed_.get_symbol();
            position_[datafeed_.get_symbol()].sell_qty += turnover_vol;
            position_[datafeed_.get_symbol()].sell_amount += amount;
        }
    }
}

long JobCtrl::Trade(TradeDirection direct, long qty, double price) {
    Order order;

    order.order_sn = orderseq_++;
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


std::map<std::string, Holding>& JobCtrl::GetPosition() {
    return position_;
}