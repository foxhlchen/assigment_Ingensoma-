#include "strategy1.h"
#include "../datadef.h"
#include "../strategy_factory.h"

REG_STRATEGY(Strategy1);


void Strategy1::OnStart() {
    Subscribe("60000344");
}

void Strategy1::OnTick(TickData tickdata) {
    
}

void Strategy1::OnFinished() {

}