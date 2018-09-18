#include <iostream>
#include "strategy_factory.h"
#include "runner.h"

int main() {
    std::vector<std::string>  strategy_jobs{"Strategy2"};
    std::cout << "starting..." << std::endl;

    Runner runner;
    runner.Init(strategy_jobs);
    runner.Run();

    return 0;
}