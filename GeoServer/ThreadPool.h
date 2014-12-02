#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <deque>
#include <string>

#include "Work.h"

class ThreadPool
{
public:
    void add(Work work) {
        workQueue_.push_front(work);
    }

    bool hasWork() {
        return !workQueue_.empty();
    }

    Work pullWork() {
        auto work = workQueue_.back();
        workQueue_.pop_back();
        return work;
    }

private:
    std::deque<Work> workQueue_;

};

#endif // THREADPOOL_H
