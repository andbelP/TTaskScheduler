#pragma once

#include <memory>
#include <type_traits>

#include "ITask.hpp"
#include "TFuture.hpp"

class TTaskScheduler;

class TTask {
    std::shared_ptr<ITask> task_;
    TTaskScheduler& scheduler_;
    std::weak_ptr<int> scheduler_lifetime_controller_;

   public:
    TTask(std::shared_ptr<ITask> task, TTaskScheduler& scheduler);

    template <typename ResultType>
    ResultType getResultSync() {
        if constexpr (std::is_reference_v<ResultType>) {
            return stdd::any_cast<ResultType>(task_->GetResultRef());
        } else {
            return stdd::any_cast<ResultType>(task_->GetResult());
        }
    }

    template <typename ResultType>
    TFuture<ResultType> getFutureResult() {
        return TFuture<ResultType>(task_);
    }

    template <typename Func>
    TTask apply(Func&& f);
};