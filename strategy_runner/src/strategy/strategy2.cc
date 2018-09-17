#include "strategy2.h"
#include "../datadef.h"
#include "../strategy_factory.h"

REG_STRATEGY(Strategy2);

void Strategy2::OnStart() {
    Subscribe("60000364");
}

void Strategy2::OnTick(TickData& tickdata) {

}

void Strategy2::OnFinished() {

}