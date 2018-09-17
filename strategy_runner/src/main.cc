#include <iostream>
#include "runner.h"

int main() {
    std::vector<std::string>  strategy_jobs{"Strategy1", "Strategy2"};
    
    Runner runner;
    runner.Init(strategy_jobs);
    runner.Run();

    return 0;
}