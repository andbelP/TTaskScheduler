#pragma once

#include <type_traits>

#include "ITask.hpp"
#include "stdd/metafunctions.hpp"
#include "stdd/tuple.hpp"
#include "stdd/utils.hpp"
#include "stdd/apply_unpack.hpp"
#include "stdd/any.hpp"
#include "stdd/argument.hpp"

template <typename Func, typename... Args>
class TTaskImpl : public ITask {
    Func f_;
    stdd::tuple<stdd::Argument<Args>...> args_;
    stdd::any result_{};
    bool executed_ = false;
    bool moved_ = false;

   public:
    template <typename TFunc, typename... TArgs>
    TTaskImpl(TFunc&& f, TArgs&&... args)
        : f_(stdd::forward<TFunc>(f)), args_(stdd::forward<TArgs>(args)...) {};

    stdd::any& GetResultRef() override {
        if (!executed_) {
            Execute();
        }
        if (moved_) {
            throw std::runtime_error("Can't get result. Result is already moved.");
        }
        return result_;
    }

    stdd::any&& GetResultRValueRef() override {
        if (!executed_) {
            Execute();
        }
        if (moved_) {
            throw std::runtime_error("Can't get result. Result is already moved.");
        }
        moved_=true;
        return stdd::move(result_);
    }

    stdd::any GetResult() override {
        if (!executed_) {
            Execute();
        }
        if (moved_) {
            throw std::runtime_error("Can't get result. Result is already moved.");
        }
        moved_=true;
        return stdd::move(result_);
    }

    void Execute() override {
        if(executed_){
            return;
        }
        if constexpr(std::is_same_v<void, decltype(stdd::InvokeAndUnpack(f_, args_))>) {
            stdd::InvokeAndUnpack(f_, args_);
        }
        else {
            result_ = stdd::InvokeAndUnpack(f_, args_);
        }
        executed_ = true;
    }
};
