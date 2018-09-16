#include "jobctrl.h"
#include "job.h"

bool JobCtrl::Init(std::string jobname) {

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
}