#pragma once

#include <any>

class ITask{
public:
    virtual void Execute() = 0;
    virtual std::any& GetResultRef() = 0;
    virtual std::any GetResult() = 0;
    virtual std::shared_ptr<ITask> Clone() const = 0;
    virtual ~ITask() = default;
};