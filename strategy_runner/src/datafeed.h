#ifndef DATAFEED_H_
#define DATAFEED_H_

#include <string>
#include "datadef.h"
#include <fstream>

class DataFeed {
public:
    bool Init(std::string symbol);
    bool GetNextTick(TickData& tickdata);
    std::string& get_symbol() { return symbol_; }

private:
    std::string symbol_;
    std::ifstream infile_;
}; // class datafeed

#endif DATAFEED_H_