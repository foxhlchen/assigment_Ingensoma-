#ifndef DATAFEED_H_
#define DATAFEED_H_

#include <string>
#include "datadef.h"
#include <fstream>

class DataFeed {
public:
    bool Init(std::string symbol);
    bool GetCurrentTick(TickData& tickdata);
    bool LoadNextTick();
    std::string& get_symbol() { return symbol_; }

private:
    TickData curr_;
    std::string symbol_;
    std::ifstream infile_;
    bool have_tick_ = false;
}; // class datafeed

#endif DATAFEED_H_