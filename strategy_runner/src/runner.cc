#include "runner.h"
#include <thread>
#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

using namespace std;

void Runner::InitPlot() {
    // clear figure, but leaves window opened.
    plt::clf();
    // Set the size of output image = 1200x780 pixels
    plt::figure_size(1200, 780);
}

void Runner::Plot() {
    // Set x-axis to interval [0,1000000]
    plt::xlim(90000, 150000);
    // Enable legend.
    plt::legend();
    // Show plot
    plt::show();
}

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

    // plot each strategies respectively.
    for (auto& jobctrl : jobctrls_) {
        InitPlot();
        jobctrl.Plot();
        Plot();
    }

    // plot each strategies all together.
    InitPlot();
    for (auto& jobctrl : jobctrls_) {
        jobctrl.Plot();
    }
    Plot();

    for (auto& jobctrl : jobctrls_) {
        delete jobctrl;
    }
}
