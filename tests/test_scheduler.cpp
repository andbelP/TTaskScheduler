#include <gtest/gtest.h>

#include <memory>
#include <stdexcept>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>

#include "TTaskScheduler/TTaskScheduler.hpp"
#include "TTaskScheduler/stdd/tuple.hpp"

TEST(Scheduler, Value) {
    TTaskScheduler scheduler;

    auto task = scheduler.add([] { return 42; });

    scheduler.executeAll();

    ASSERT_EQ(task.getResultSync<int>(), 42);
}

TEST(Scheduler, Arg) {
    TTaskScheduler scheduler;

    auto task = scheduler.add([](int x) { return x + 1; }, 5);

    scheduler.executeAll();

    ASSERT_EQ(task.getResultSync<int>(), 6);
}

TEST(Scheduler, Apply) {
    TTaskScheduler scheduler;

    auto first = scheduler.add([] { return 10; });
    auto second = first.apply([](int x) { return x * 2; });

    scheduler.executeAll();

    ASSERT_EQ(second.getResultSync<int>(), 20);
}

TEST(Scheduler, Chain) {
    TTaskScheduler scheduler;

    auto first = scheduler.add([] { return 2; });
    auto second = first.apply([](int x) { return x + 3; });
    auto third = second.apply([](int x) { return x * x; });

    scheduler.executeAll();

    ASSERT_EQ(third.getResultSync<int>(), 25);
}

TEST(Scheduler, VectorArg) {
    TTaskScheduler scheduler;

    std::vector<int> values = {1, 2, 3};
    auto task = scheduler.add(
        [](const std::vector<int>& v) {
            return v[0] + v[1] + v[2];
        },
        values);

    scheduler.executeAll();

    ASSERT_EQ(task.getResultSync<int>(), 6);
}


TEST(Scheduler, StringArg) {
    TTaskScheduler scheduler;

    auto task = scheduler.add(
        [](const std::string& s) {
            return s + "!";
        },
        std::string("hello"));

    scheduler.executeAll();

    ASSERT_EQ(task.getResultSync<std::string>(), "hello!");
}

TEST(Scheduler, BoolArg) {
    TTaskScheduler scheduler;

    auto task = scheduler.add([](bool value) { return !value; }, true);

    scheduler.executeAll();

    ASSERT_FALSE(task.getResultSync<bool>());
}

TEST(Scheduler, MoveOnlyArg) {
    TTaskScheduler scheduler;

    auto task = scheduler.add(
        [](std::unique_ptr<int> value) {
            return *value + 1;
        },
        std::make_unique<int>(7));

    scheduler.executeAll();

    ASSERT_EQ(task.getResultSync<int>(), 8);
}

TEST(Scheduler, TwoArgs) {
    TTaskScheduler scheduler;

    auto task = scheduler.add([](int a, int b) { return a + b; }, 3, 4);

    scheduler.executeAll();

    ASSERT_EQ(task.getResultSync<int>(), 7);
}

TEST(Scheduler, MapResult) {
    TTaskScheduler scheduler;

    auto task = scheduler.add([] {
        return std::unordered_map<int, int>{{1, 2}};
    });

    scheduler.executeAll();

    auto result = task.getResultSync<std::unordered_map<int, int>>();

    ASSERT_EQ(result.at(1), 2);
}

TEST(Scheduler, VectorRefChain) {
    TTaskScheduler scheduler;

    auto first = scheduler.add([] {
        return std::vector<int>{1, 2, 3};
    });
    auto second = first.apply([](std::vector<int>& values) {
        return values.size();
    });

    scheduler.executeAll();

    ASSERT_EQ(second.getResultSync<std::size_t>(), 3);
}

TEST(Scheduler, StringChain) {
    TTaskScheduler scheduler;

    auto first = scheduler.add([] {
        return std::string("abc");
    });
    auto second = first.apply([](const std::string& s) {
        return s + "d";
    });

    scheduler.executeAll();

    ASSERT_EQ(second.getResultSync<std::string>(), "abcd");
}

TEST(Scheduler, FutureMove) {
    TTaskScheduler scheduler;

    auto task = scheduler.add([] {
        return std::vector<int>{1, 2, 3};
    });
    auto future = task.getFutureResult<std::vector<int>>();

    scheduler.executeAll();

    auto result = future.get();

    ASSERT_EQ(result.size(), 3);
    ASSERT_THROW(future.get(), std::runtime_error);
}

TEST(Scheduler, FutureReference) {
    TTaskScheduler scheduler;
    int val = 5;
    auto task = scheduler.add([&val] -> int& {
        return val;
    });
    auto future = task.getFutureResult<int&>();

    scheduler.executeAll();

    ASSERT_EQ(future.get(), 5);
    ASSERT_EQ(future.get(), 5);
}

TEST(Scheduler, Empty) {
    TTaskScheduler scheduler;

    scheduler.executeAll();
}

TEST(Scheduler, LambdaByValue) {
    TTaskScheduler scheduler;

    int value = 5;

    auto task = scheduler.add(
        [](int x) {
            x += 10;
            return x;
        },
        value
    );

    scheduler.executeAll();

    ASSERT_EQ(task.getResultSync<int>(), 15);
    ASSERT_EQ(value, 5);
}

TEST(Scheduler, LambdaByLvalueReference) {
    TTaskScheduler scheduler;

    int value = 5;

    auto task = scheduler.add(
        [](int& x) {
            x += 10;
            return x;
        },
        value
    );

    scheduler.executeAll();

    ASSERT_EQ(task.getResultSync<int>(), 15);
    ASSERT_EQ(value, 5);
}


TEST(Scheduler, LambdaByConstLvalueReference) {
    TTaskScheduler scheduler;

    std::string value = "abc";

    auto task = scheduler.add(
        [](const std::string& s) {
            return s + "d";
        },
        value
    );

    scheduler.executeAll();

    ASSERT_EQ(task.getResultSync<std::string>(), "abcd");
    ASSERT_EQ(value, "abc");
}


TEST(Scheduler, LambdaByRvalueReference) {
    TTaskScheduler scheduler;

    auto task = scheduler.add(
        [](std::string&& s) {
            s += "!";
            return s;
        },
        std::string("hello")
    );

    scheduler.executeAll();

    ASSERT_EQ(task.getResultSync<std::string>(), "hello!");
}

TEST(Scheduler, LvalueRefAndMoveOnlyByValue) {
    TTaskScheduler scheduler;

    int value = 3;

    auto task = scheduler.add(
        [](int& x, std::unique_ptr<int> p) {
            x += *p;
            return x;
        },
        value,
        std::make_unique<int>(7)
    );

    scheduler.executeAll();

    ASSERT_EQ(task.getResultSync<int>(), 10);
    ASSERT_EQ(value, 3);
}

TEST(Scheduler, ConstRefAndRvalueRef) {
    TTaskScheduler scheduler;

    std::string prefix = "hello";

    auto task = scheduler.add(
        [](const std::string& a, std::string&& b) {
            return a + " " + b;
        },
        prefix,
        std::string("world")
    );

    scheduler.executeAll();

    ASSERT_EQ(task.getResultSync<std::string>(), "hello world");
    ASSERT_EQ(prefix, "hello");
}

TEST(Scheduler, TemplateLambdaByValue) {
    TTaskScheduler scheduler;

    auto task = scheduler.add(
        [](auto x) {
            return x + 1;
        },
        41
    );

    scheduler.executeAll();

    ASSERT_EQ(task.getResultSync<int>(), 42);
}


TEST(Scheduler, TemplateLambdaByLvalueReference) {
    TTaskScheduler scheduler;

    int value = 10;

    auto task = scheduler.add(
        [](auto& x) {
            x += 5;
            return x;
        },
        value
    );

    scheduler.executeAll();

    ASSERT_EQ(task.getResultSync<int>(), 15);
    ASSERT_EQ(value, 10);
}


TEST(Scheduler, templateLambdaMixedReferenceAndMoveOnly) {
    TTaskScheduler scheduler;

    int value = 3;

    auto task = scheduler.add(
        [](auto& x, auto p) {
            x += *p;
            return x;
        },
        value,
        std::make_unique<int>(7)
    );

    scheduler.executeAll();

    ASSERT_EQ(task.getResultSync<int>(), 10);
    ASSERT_EQ(value, 3);
}


