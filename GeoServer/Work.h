#ifndef WORK_H
#define WORK_H

#include <functional>

class Work
{
public:
    static const int DefaultId{0};
    Work(int id = DefaultId)
        : id_{id}
        , executeFunction_{[]{}} {}
    Work(std::function<void()> executeFunction, int id = DefaultId)
        : id_{id}
        , executeFunction_{executeFunction}
    {}

    void execute() {
        executeFunction_();
    }

    int id() const { return id_; }

private:
    int id_;
    std::function<void()> executeFunction_;
};

#endif // WORK_H
