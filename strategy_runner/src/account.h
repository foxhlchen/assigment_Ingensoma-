#ifndef ACCOUNT_H_
#define ACCOUNT_H_

#include "datadef.h"

class Account {
public:
    long Trade(TradeDirection direct, long qty, double price);

}; // class Account

#endif ACCOUNT_H_