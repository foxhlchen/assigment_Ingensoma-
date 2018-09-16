#include "datafeed.h"

using namespace std;

bool DataFeed::Init(std::string symbol) {
    symbol_ = symbol;
    string srcfile = "./data/" + symbol + ".csv";
    string line;

    infile_.open(srcfile.c_str());
    return (bool) getline(infile_, line); // strip header
}

bool DataFeed::GetNextTick(TickData& tickdata) {
    string line;

    tickdata.symbol = symbol_;
    if (! getline(infile_, line))
        return false;

    return tickdata.Feed(line);
}