#pragma once

#include <any>
#include <functional>
#include <type_traits>

#include "ITask.hpp"
#include "stdd/metafunctions.hpp"
#include "stdd/tuple.hpp"
#include "stdd/utils.hpp"
#include "stdd/apply_unpack.hpp"

template <typename Func, typename... Args>
class TTaskImpl : public ITask {
    Func f_;
    stdd::Tuple<std::decay_t<Args>...> args_;
    std::any result_{};
    bool executed_ = false;

   public:
    template <typename TFunc, typename... TArgs>
    TTaskImpl(TFunc&& f, TArgs&&... args)
        : f_(stdd::forward<TFunc>(f)), args_(stdd::forward<TArgs>(args)...) {};

    std::any& GetResultRef() override {
        if (!executed_) {
            Execute();
        }

        return result_;
    }

    std::any GetResult() override {
        if (!executed_) {
            Execute();
        }

        return std::move(result_);
    }

    void Execute() override {
        if constexpr(std::is_same_v<void, decltype(stdd::ApplyAndUnpack(f_, args_))>) {
            stdd::ApplyAndUnpack(f_, args_);
        }
        else {
            result_ = stdd::ApplyAndUnpack(f_, args_);
        }
        executed_ = true;
    }

    std::shared_ptr<ITask> Clone() const override {
        return std::make_shared<TTaskImpl<Func, Args...>>(*this);
    }
};
