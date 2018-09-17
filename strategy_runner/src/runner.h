#ifndef RUNNER_H_
#define RUNNER_H_

#include <vector>
#include <string>
#include <thread>
#include "jobctrl.h"

class Runner {
public:
    bool Init(std::vector<std::string> jobs_to_run);
    void Run();

private:
    std::vector<JobCtrl*> jobctrls_;
    std::vector<std::thread *> threads_;

}; // class Runner

#endif RUNNER_H_