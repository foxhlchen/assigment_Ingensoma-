#include "strategy_factory.h"

StrategyFactor* StrategyFactor::inst_ = nullptr;

StrategyFactor* StrategyFactor::Singleton() {
    if (inst_ == nullptr)
        inst_ = new StrategyFactor();

    return inst_;
}

void StrategyFactor::RegisterStrategy(std::string strategy_name, StrategyCreator* creator) 
{
    std::cout << "register " << strategy_name << std::endl;
    register_book_[strategy_name] = creator;
}

Job* StrategyFactor::GetStrategy(std::string strategy_name) {
    std::cout << "create " << strategy_name << std::endl;
    if (register_book_.find(strategy_name) != register_book_.end()) {
        return register_book_[strategy_name]->Create();
    }

    std::cout << "create " << strategy_name << "failed" << std::endl;
    return nullptr;
}