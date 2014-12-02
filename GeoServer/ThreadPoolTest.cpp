#include "gmock/gmock.h"
#include "ThreadPool.h"
#include "Work.h"

using namespace std;
using namespace ::testing;

class AThreadPool : public Test
{
public:
    AThreadPool() {}
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
