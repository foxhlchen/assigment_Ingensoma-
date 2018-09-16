#include "job.h"
#include "jobctrl.h"
#include "datadef.h"

bool Job::Init(JobCtrl* control) {
    ctrl_ = control;

    return true;
}

void Job::Subscribe(std::string symbol) {
    return ctrl_->Subscribe(symbol);
}

long Job::Trade(TradeDirection direct, long qty, double price) {
    return ctrl_->Trade(direct, qty, price);
}