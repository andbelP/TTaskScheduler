#pragma once

#include "stdd/any.hpp"

class ITask{
public:
    virtual void Execute() = 0;
    virtual stdd::any& GetResultRef() = 0;
    virtual stdd::any&& GetResultRValueRef() = 0;
    virtual stdd::any GetResult() = 0;
    virtual std::shared_ptr<ITask> Clone() const = 0;
    virtual ~ITask() = default;
};