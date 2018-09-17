#ifndef STRATEGY_FACTORY_H_
#define STRATEGY_FACTORY_H_

#include "job.h"
#include <string>
#include <map>

class StrategyCreator {
public:
    Job* Create();
};

class StrategyFactor {
public:
    static Job* GetStrategy(std::string strategy_name) {
        if (register_book_.find(strategy_name) != register_book_.end()) {
            return register_book_[strategy_name]->Create();
        }
    }

    static void RegisterStrategy(std::string strategy_name, StrategyCreator* creator) {
        register_book_[strategy_name] = creator;
    }

private:
    static std::map<std::string, StrategyCreator*> register_book_;
};

#endif //STRATEGY_FACTORY_H_