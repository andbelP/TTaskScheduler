#pragma once

#include <vector>
#include <memory>


#include "TTask.hpp"
#include "TFuture.hpp"
#include "stdd/metafunctions.hpp"

class TTaskScheduler{

    friend class TTask;

    std::shared_ptr<int> lifetime_controller_;
    std::vector<std::shared_ptr<ITask>> tasks_;

public:

    TTaskScheduler() : lifetime_controller_(std::make_shared<int>(1)) {}

    TTaskScheduler(const TTaskScheduler& other) : lifetime_controller_(std::make_shared<int>(1)){

        tasks_.reserve(other.tasks_.size());
        for(const auto& task : other.tasks_){
            tasks_.push_back(task->Clone());
        }
        
    }

    TTaskScheduler& operator=(const TTaskScheduler& other) = delete;

    template<typename Func, typename... Args>
    TTask add(Func&& f, Args&&... args){
        tasks_.emplace_back(std::make_unique<TTaskImpl<stdd::remove_cvref_t<Func>, stdd::remove_cvref_t<Args>...>>(std::forward<Func>(f), std::forward<Args>(args)...));
        return TTask{*(tasks_.end()-1), *this};
    }

    void executeAll(){
        for(const auto& task : tasks_){
            task->Execute();
        }
    }
    
};


template<typename Func>
TTask TTask::apply(Func&& f) {
    if(scheduler_lifetime_controller_.expired()){
        throw std::runtime_error("Can't execute TTask method apply(Func&& f). TTaskScheduler is already dead");
    }
    return scheduler_.add(std::forward<Func>(f), TFuture<Func>{task_});
}

