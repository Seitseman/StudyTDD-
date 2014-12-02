#include "TestTimer.h"

#include "gmock/gmock.h"
#include <iostream>

using namespace std;

TestTimer::TestTimer()
    :TestTimer(::testing::UnitTest::GetInstance()->current_test_info()->test_case_name())
{
}

TestTimer::TestTimer(const std::string& text)
    : Start{chrono::system_clock::now()}
    , Text{text}
{
}

TestTimer::~TestTimer()
{
    Stop = chrono::system_clock::now();
    Elapsed = chrono::duration_cast<chrono::microseconds>(Stop - Start);
    cout << endl << Text << " elapsed time= " << Elapsed.count() * 0.001 << "ms" << endl;
}
