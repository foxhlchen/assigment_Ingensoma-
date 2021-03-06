#include "job.h"
#include "jobctrl.h"
#include "datadef.h"

bool Job::Init(JobCtrl* control) {
    ctrl_ = control;

    return true;
}

void Job::Subscribe(std::string symbol) {
    if (ctrl_)
        return ctrl_->Subscribe(symbol);
}

void Job::CancelOrder(long order_sn) {
    if (ctrl_)
        return ctrl_->CancelOrder(order_sn);
}

long Job::Trade(std::string symbol, TradeDirection direct, long qty, double price) {
    if (ctrl_)
        return ctrl_->Trade(symbol, direct, qty, price);

    return -1;
}

void Job::GetOrders(std::string symbol, std::vector<Order>& orders, OrderStatus status /* = kAll */) {
    if (ctrl_)
        return ctrl_->GetOrders(symbol, orders, status);
}

std::map<std::string, Holding>& Job::GetPosition() {
    if (ctrl_)
        return ctrl_->GetPosition();
}

void Job::SetInitialBalance(long value) {
    if (ctrl_)
        return ctrl_->SetInitialBalance(value);
}