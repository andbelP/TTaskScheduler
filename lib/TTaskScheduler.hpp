#pragma once

#include <memory>
#include <vector>

#include "ITask.hpp"
#include "TFuture.hpp"
#include "TTaskImpl.hpp"
#include "stdd/metafunctions.hpp"
#include "stdd/utils.hpp"

class TTask;

class TTaskScheduler {
    friend class TTask;

    std::shared_ptr<int> lifetime_controller_;
    std::vector<std::shared_ptr<ITask>> tasks_;

   public:
    TTaskScheduler() : lifetime_controller_(std::make_shared<int>(1)) {}

    TTaskScheduler(const TTaskScheduler& other)
        : lifetime_controller_(std::make_shared<int>(1)) {
        tasks_.reserve(other.tasks_.size());
        for (const auto& task : other.tasks_) {
            tasks_.push_back(task->Clone());
        }
    }

    TTaskScheduler& operator=(const TTaskScheduler& other) = delete; // TODO

    template <typename Func, typename... Args>
    TTask add(Func&& f, Args&&... args);

    void executeAll() {
        for (const auto& task : tasks_) {
            task->Execute();
        }
    }
};

class TTask {
    std::shared_ptr<ITask> task_;
    TTaskScheduler& scheduler_;
    std::weak_ptr<int> scheduler_lifetime_controller_;

   public:
    TTask(std::shared_ptr<ITask> task, TTaskScheduler& scheduler)
        : task_(task),
          scheduler_(scheduler),
          scheduler_lifetime_controller_(scheduler.lifetime_controller_) {};

    template <typename ResultType>
    ResultType getResultSync() {
        if constexpr (std::is_reference_v<ResultType>) {
            return std::any_cast<ResultType>(task_->GetResultRef());
        } else {
            return std::any_cast<ResultType>(task_->GetResult());
        }
    }

    template <typename ResultType>
    TFuture<ResultType> getFutureResult() {
        return TFuture<ResultType>(task_);
    }

    template <typename Func>
    TTask apply(Func&& f);
};


template <typename Func, typename... Args>
TTask TTaskScheduler::add(Func&& f, Args&&... args) {
    tasks_.emplace_back(
        std::make_shared<TTaskImpl<std::remove_cvref_t<Func>,
                                   std::remove_reference_t<Args>...>>(
            stdd::forward<Func>(f), stdd::forward<Args>(args)...));
    return TTask{*(tasks_.end() - 1), *this};
}

template <typename Func>
TTask TTask::apply(Func&& f) {
    if (scheduler_lifetime_controller_.expired()) {
        throw std::runtime_error(
            "Can't execute TTask method apply(Func&& f). TTaskScheduler is "
            "already dead");
    }
    return scheduler_.add(stdd::forward<Func>(f), TFuture<stdd::first_func_argument_t<Func>>{task_});
}
