#include "jobctrl.h"
#include "job.h"
#include "strategy_factory.h"
#include "datadef.h"
#include "util.h"
#include <iostream>

using util::TimeCalculation;

bool JobCtrl::Init(std::string jobname) {
    std::cout << "Initiallize " << jobname << std::endl;
    job_ = StrategyFactor::Singleton()->GetStrategy(jobname);

    if (job_ == nullptr)
        return false;

    job_->Init(this);

    job_->OnStart();

    return true;
}

void JobCtrl::Run() {
    if (job_ == nullptr) 
        return;

    // load first tick
    for (auto &it: datafeed_) {
        it.second.LoadNextTick();
    }

    std::cout << "Running... " << std::endl;
    for (;;) {
        /* This design allows strategies to subsribe multiple market symbols, 
        *  so we have to aligh multiple data sources by time.
        *  Although It's not necessary for this assginment,
        *  But I want it to be implemented in a more universal way.
        */

        // find the next earliest tick;
        _time_hhmmss_ = 150000;
        for (auto &it: datafeed_) {
            TickData tickdata;
            if ( !it.second.GetCurrentTick(tickdata) ) {
                continue;
            }

            if (_time_hhmmss_ > tickdata.data_time)
                _time_hhmmss_ = tickdata.data_time;
        }

        //std::cout << _time_hhmmss_ << std::endl;
        if (_time_hhmmss_ == 150000)
            break; // market closed
        

        for (auto &it: datafeed_) {
            TickData tickdata;
            if ( !it.second.GetCurrentTick(tickdata) ) {
                continue;
            }

            if (tickdata.data_time != _time_hhmmss_)
                continue;

            MatchOrders(tickdata);
            RecordProfitLoss(tickdata);
            job_->OnTick(tickdata);

            it.second.LoadNextTick();
        }
    }

    job_->OnFinished();
}

void JobCtrl::Subscribe(std::string symbol) {
    datafeed_[symbol].Init(symbol);
}

void JobCtrl::RecordProfitLoss(TickData& tick) {
    Equity equity_entry;
    Holding& hold = position_[tick.symbol];

    double balance = hold.sell_amount - hold.buy_amount;
    long exposure = hold.buy_qty - hold.sell_qty;

    // exposure > 0, means in order to close position, you have to sell exposure qty.
    // for exposure < 0 you have to buy stock.
    double unclosed_value = exposure > 0 ? exposure * tick.bid_price : exposure * tick.ask_price;

    double profitloss = balance + unclosed_value;

    hold.balance = balance;
    hold.exposure = exposure;
    hold.unclosed_value = unclosed_value;
    hold.profitloss = profitloss;
    hold.equity = profitloss + initial_balance_;

    equity_entry.profitloss = profitloss;
    equity_entry.equity = profitloss + initial_balance_;

    equity_entry.data_time = tick.data_time;
    equity_history_[tick.symbol].push_back(equity_entry);

/*     if (equity_entry.profitloss > 0) {
        std::cout << LOGVAR(equity_entry.profitloss) << std::endl;
    } */
/*     std::cout << LOGVAR(hold.balance) << std::endl;
    std::cout << LOGVAR(hold.exposure) << std::endl;
    std::cout << LOGVAR(hold.unclosed_value) << std::endl;
    std::cout << LOGVAR(hold.profitloss) << std::endl;
    std::cout << LOGVAR(hold.equity) << std::endl;
    std::cout << "=====================" << std::endl; */
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
            trans.symbol = order.symbol;
            trans.time = tick.data_time;
            trans.trans_sn = transeq_++;
            
            order.transactions.push_back(trans);

            position_[order.symbol].symbol = order.symbol;
            position_[order.symbol].buy_qty += turnover_vol;
            position_[order.symbol].buy_amount += amount;
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
            trans.symbol = order.symbol;
            trans.time = tick.data_time;
            trans.trans_sn = transeq_++;
            
            order.transactions.push_back(trans);

            position_[order.symbol].symbol = order.symbol;
            position_[order.symbol].sell_qty += turnover_vol;
            position_[order.symbol].sell_amount += amount;
        }
    }
}

long JobCtrl::Trade(std::string symbol, TradeDirection direct, long qty, double price) {
    Order order;

    order.order_sn = orderseq_++;
    order.symbol = symbol;
    order.order_price = price;
    order.order_qty = qty;
    order.direct = direct;
    order.order_time = _time_hhmmss_;

    Holding& hold = position_[symbol];
/*  std::cout << "=====================" << std::endl;
    std::cout << LOGVAR(hold.balance) << std::endl;
    std::cout << LOGVAR(hold.exposure) << std::endl;
    std::cout << LOGVAR(hold.unclosed_value) << std::endl;
    std::cout << LOGVAR(hold.profitloss) << std::endl;
    std::cout << LOGVAR(hold.equity) << std::endl;
    std::cout << "*********************" << std::endl;
    std::cout << LOGVAR(order.order_sn) << std::endl;
    std::cout << LOGVAR(order.symbol) << std::endl;
    std::cout << LOGVAR(order.order_price) << std::endl;
    std::cout << LOGVAR(order.order_qty) << std::endl;
    std::cout << LOGVAR(order.direct) << std::endl;
    std::cout << LOGVAR(order.order_time) << std::endl;
    std::cout << "=====================" << std::endl; */

    std::cout << LOGVAR(hold.balance) << 
    LOGVAR(hold.exposure) << 
    LOGVAR(hold.unclosed_value) << LOGVAR(hold.profitloss) << LOGVAR(hold.equity) << std::endl;
    std::cout <<
    LOGVAR(order.order_sn) << LOGVAR(order.symbol) << LOGVAR(order.order_price) << 
    LOGVAR(order.order_qty) << LOGVAR(order.direct) << LOGVAR(order.order_time) << std::endl;

    std::cout << std::endl; 


    orders_[order.order_sn] = order;

    return order.order_sn;
}

void JobCtrl::CancelOrder(long order_sn) {
    if (orders_.find(order_sn) != orders_.end()) {
        orders_[order_sn].Cancel();
    }
}

void JobCtrl::GetOrders(std::string symbol, std::vector<Order>& orders, OrderStatus status /* = kAll*/) {
    // because rules of every stock exchanges are varied, 
    // To keep it simple, we don't consider if we have enough money to buy/sell stock here.

    for (auto& it: orders_) {
        Order &order = it.second;

        if (! symbol.empty() && order.symbol != symbol)
            continue;

        if (status != kAll && order.GetStatus() != status) {
            continue;
        }

        orders.push_back(order);
    }
}

std::map<std::string, Holding>& JobCtrl::GetPosition() {
    return position_;
}