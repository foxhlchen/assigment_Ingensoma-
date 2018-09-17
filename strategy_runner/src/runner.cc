#include "runner.h"
#include <thread>

using namespace std;

bool Runner::Init(std::vector<std::string> jobs_to_run) {
    for (auto& strategy_name: jobs_to_run) {
        JobCtrl* jobctrl = new JobCtrl;
        if (! jobctrl->Init(strategy_name)) {
            delete jobctrl;
            continue;
        }

        jobctrls_.push_back(jobctrl);
    }
}
void Runner::Run() {
    for (auto& jobctrl : jobctrls_) {     
        thread* t = new thread(&JobCtrl::Run, ref(jobctrl));
        threads_.push_back(t);
    }

    for (auto& t : threads_) {
        t->join();
        delete t;
    }

    for (auto& jobctrl : jobctrls_) {
        delete jobctrl;
    }
}
