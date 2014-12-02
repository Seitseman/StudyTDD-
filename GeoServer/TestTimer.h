#ifndef TESTTIMER_H
#define TESTTIMER_H

#include <string>
#include <chrono>

struct TestTimer
{
public:
    explicit TestTimer();
    TestTimer(const std::string& text);
    ~TestTimer();

    std::chrono::time_point<std::chrono::system_clock> Start;
    std::chrono::time_point<std::chrono::system_clock> Stop;
    std::chrono::microseconds Elapsed;
    std::string Text;
};

#endif // TESTTIMER_H
