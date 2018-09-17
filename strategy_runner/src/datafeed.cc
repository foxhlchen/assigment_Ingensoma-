#include "datafeed.h"

using namespace std;

bool DataFeed::Init(std::string symbol) {
    symbol_ = symbol;
    string srcfile = "./data/" + symbol + ".csv";
    string line;

    infile_.open(srcfile.c_str());
    return (bool) getline(infile_, line); // strip header
}

bool DataFeed::GetCurrentTick(TickData& tickdata) {
    tickdata = curr_;

    return have_tick_;
}

bool DataFeed::LoadNextTick() {
    string line;
    have_tick_ = false;

    curr_ = TickData();
    curr_.symbol = symbol_;
    if (getline(infile_, line)) {
        have_tick_ = curr_.Feed(line);
    }

    return have_tick_;
}