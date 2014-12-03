#include "gmock/gmock.h"

#include "ThreadPool.h"
#include "Work.h"
#include <condition_variable>
#include <mutex>
#include <chrono>
#include <thread>
#include <vector>
#include <set>

using namespace std;
using std::chrono::milliseconds;
using namespace ::testing;

class AThreadPool : public Test
{
public:
    AThreadPool() {}
    mutex m;
    ThreadPool pool;
};

TEST_F(AThreadPool, Has_no_work_on_creation)
{
    ASSERT_FALSE(pool.hasWork());
}

TEST_F(AThreadPool, HasWorkAfterAdd)
{
    pool.add(Work{});
    ASSERT_TRUE(pool.hasWork());
}

TEST_F(AThreadPool, AnswersWorkAddedOnPull)
{
    pool.add(Work{1});
    auto work = pool.pullWork();

    ASSERT_EQ(1, work.id());
}

TEST_F(AThreadPool, Pulls_elements_in_FIFO_order)
{
    pool.add(Work{1});
    pool.add(Work{2});
    auto work = pool.pullWork();

    ASSERT_EQ(1, work.id());
}

TEST_F(AThreadPool, Has_no_work_after_Last_element_removed)
{
    pool.add(Work{1});
    pool.pullWork();
    ASSERT_FALSE(pool.hasWork());
}

TEST_F(AThreadPool, Has_work_after_work_removed_but_work_remains)
{
    pool.add(Work{});
    pool.add(Work{});
    pool.pullWork();
    ASSERT_TRUE(pool.hasWork());
}


class AThreadPool_AddRequest : public Test
{
public:
    mutex m;
    ThreadPool pool;
    condition_variable wasExecuted;
    unsigned int count{0};
    vector<shared_ptr<thread>> threads;

    void SetUp() override {
        pool.start();
    }

    void TearDown() override {
        for (auto& t : threads) {
            t->join();
        }
    }

    void incrementCountAndNotify() {
        unique_lock<mutex> lock(m);
        ++count;
        wasExecuted.notify_all();
    }

    void waitForCountAndFailOnTimeout(unsigned int expectedCount,
                                      const milliseconds& time = milliseconds(100)) {
        unique_lock<mutex> lock(m);
        ASSERT_TRUE(wasExecuted.wait_for(lock, time, [&] {return expectedCount == count;}));
    }
};

TEST_F(AThreadPool_AddRequest, Pulls_work_in_a_thread)
{
    Work work{[&] {incrementCountAndNotify(); }};
    unsigned int numberOfWorkItems{1};
    pool.add(work);
    waitForCountAndFailOnTimeout(numberOfWorkItems);
}

TEST_F(AThreadPool_AddRequest, Executes_all_work)
{
    Work work{[&] { incrementCountAndNotify(); }};
    unsigned int numberOfWorkItems{3};
    for(unsigned int i{0}; i < numberOfWorkItems; ++i)
    {
        pool.add(work);
    }

    waitForCountAndFailOnTimeout(numberOfWorkItems);
}

TEST_F(AThreadPool_AddRequest, HoldsUp_under_client_stress)
{
    Work work{[&] {incrementCountAndNotify(); }};
    unsigned int numberOfWorkItems{100};
    unsigned int numberOfThreads{100};

    for (unsigned int i{0}; i < numberOfThreads; ++i)
    {
        threads.push_back(make_shared<thread>([&] {
            for(unsigned int j{0}; j < numberOfWorkItems; ++j)
            {
                pool.add(work);
            }
        }));
    }

    waitForCountAndFailOnTimeout(numberOfThreads*numberOfWorkItems);
}

class AThreadPoolWithMultipleThreads : public AThreadPool_AddRequest
{
public:
    set<thread::id> threads;

    void SetUp() override {

    }

    void addThreadIfUnique(const thread::id& id) {
        std::unique_lock<std::mutex> lock(m);
        threads.insert(id);
    }

    size_t numberOfThreadsProcessed() {
        return threads.size();
    }
};

TEST_F(AThreadPoolWithMultipleThreads, Dispatches_work_to_multiple_threads)
{
    unsigned int numberOfThreads{2};
    pool.start(numberOfThreads);
    Work work{ [&] {
            addThreadIfUnique(this_thread::get_id());
            incrementCountAndNotify();
    }};

    unsigned int numberOfWorkItems{500};

    for (unsigned int i{0}; i < numberOfWorkItems; ++i)
    {
        pool.add(work);
    }

    waitForCountAndFailOnTimeout(numberOfWorkItems);
    ASSERT_EQ(numberOfThreads, numberOfThreadsProcessed());
}
