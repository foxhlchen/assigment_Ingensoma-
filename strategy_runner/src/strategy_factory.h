#ifndef STRATEGY_FACTORY_H_
#define STRATEGY_FACTORY_H_

#include "job.h"
#include <string>
#include <map>
#include <iostream>

class StrategyCreator {
public:
    virtual Job* Create() = 0;
};

class StrategyFactor {
public:
    Job* GetStrategy(std::string strategy_name);

    void RegisterStrategy(std::string strategy_name, StrategyCreator* creator);

    static StrategyFactor* Singleton();

private:
    std::map<std::string, StrategyCreator*> register_book_;
    static StrategyFactor* inst_;
};

#endif //STRATEGY_FACTORY_H_