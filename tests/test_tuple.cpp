#include <gtest/gtest.h>

#include "TTaskScheduler/stdd/tuple.hpp"

TEST(Tuple, Get) {
    stdd::tuple<int, double, std::string> tuple(1, 2.5, "hi");

    ASSERT_EQ(stdd::get<0>(tuple), 1);
    ASSERT_EQ(stdd::get<1>(tuple), 2.5);
    ASSERT_EQ(stdd::get<2>(tuple), "hi");
}

TEST(Tuple, Equal) {
    stdd::tuple<int, int> first(1, 2);
    stdd::tuple<int, int> second(1, 2);

    ASSERT_EQ(first, second);
}

TEST(Tuple, Move) {
    stdd::tuple<std::string, int> first("abc", 5);
    stdd::tuple<std::string, int> second = std::move(first);

    ASSERT_EQ(stdd::get<1>(second), 5);
}

TEST(Tuple, Reference) {
    int value = 10;
    stdd::tuple<int&> tuple(value);

    stdd::get<0>(tuple) = 20;

    ASSERT_EQ(value, 20);
}

TEST(Tuple, Empty) {
    stdd::tuple<> tuple;

    SUCCEED();
}
