#include "Work.h"

#include "gmock/gmock.h"
#include <functional>
#include <vector>
#include <sstream>

using namespace std;
using namespace ::testing;

class AWorkObject : public Test
{
};


TEST_F(AWorkObject, DefaultsFunctionToNullObject)
{
    Work work;
    try {
        work.execute();
    }
    catch (...) {
        FAIL() <<"unable to execute function";
    }
}

TEST_F(AWorkObject,  DefaultsFunctionToNullObjectWhenConstructedWithId)
{
    Work work(1);
    try {
        work.execute();
    }
    catch(...) {
        FAIL() << "unable to execute function";
    }
}

TEST_F(AWorkObject, CanbeConstructedWithAnID)
{
    Work work(1);
    ASSERT_EQ(1, work.id());
}

TEST_F(AWorkObject, Defaults_ID_To_Zero)
{
    Work work;
    ASSERT_EQ(0, work.id());
}

TEST_F(AWorkObject, Defaults_id_to_0_when_function_specified)
{
    Work work{[]{}};
    ASSERT_EQ(0, work.id());
}

TEST_F(AWorkObject, Can_be_constructed_With_a_function_and_ID)
{
    Work work{[]{}, 1};
    ASSERT_EQ(1, work.id());
}

TEST_F(AWorkObject, ExecutesFunctionStored)
{
    bool wasExecuted{false};
    auto executeFunction = [&](){wasExecuted = true;};
    Work work(executeFunction);
    work.execute();
    ASSERT_TRUE(wasExecuted);
}

TEST_F(AWorkObject, Can_execute_on_data_captured_with_function)
{
    vector<string> data{"a", "b"};
    string result;
    auto callBackFunction = [&](string s) {
        result.append(s);
    };

    auto executFunction = [&]() {
        stringstream s;
        s << data[0] << data[1];
        callBackFunction(s.str());
    };
    Work work(executFunction);
    work.execute();
    ASSERT_EQ("ab", result);
}
