#pragma once


#include <any>
#include <functional>

#include "ITask.hpp"
#include "stdd/metafunctions.hpp"



template<typename Func, typename... Args>
class TTaskImpl : public ITask{
    Func f_;
    std::tuple<Args...> args_;
    std::any result_;
    bool executed_ = false;

public:

    template<typename TFunc, typename... TArgs>
    TTaskImpl(TFunc&& f, TArgs&&... args) : f_(std::forward<TFunc>(f)), args_(std::forward<TArgs>(args)...) {};

    std::any& GetResultRef() override{

        if(!executed_){
            Execute();
        }

        return result_;
    }

    std::any GetResult() override{

        if(!executed_){
            Execute();
        }

        return std::move(result_);
    }

    void Execute() override{
        result_ = std::apply(f_, args_);
        executed_ = true;
    }

    std::shared_ptr<ITask> Clone() const override{
        return std::make_shared<TTaskImpl<Func, Args...>>(*this);
    }

};






template<typename T>
class TFuture;

class TTaskScheduler;



class TTask{
    std::shared_ptr<ITask> task_;
    TTaskScheduler& scheduler_;

public:

    TTask(std::shared_ptr<ITask> task, TTaskScheduler& scheduler) : task_(task), scheduler_(scheduler){};


    template<typename ResultType>
    ResultType getResultSync(){
        if constexpr (stdd::is_reference_v<ResultType>){
            return std::any_cast<ResultType>(task_->GetResultRef());
        }
        else{
            return std::any_cast<ResultType>(task_->GetResult());
        }
    }

    template<typename ResultType>
    TFuture<ResultType> getFutureResult(){
        return TFuture<ResultType>(task_);
    }

    template<typename Func>
    TTask apply(Func&& f);

};

// Допустим, у нас есть таска, TTaskScheduler умер. Наша таска требует зависимость от другой таски. 