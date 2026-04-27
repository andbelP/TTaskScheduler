#include <gtest/gtest.h>

#include "stdd/any.hpp"
#include <string>

TEST(Any, Int) {
    stdd::any value = 42;

    ASSERT_EQ(stdd::any_cast<int>(value), 42);
}

TEST(Any, String) {
    stdd::any value = std::string("hello");

    ASSERT_EQ(stdd::any_cast<std::string>(value), "hello");
}

TEST(Any, BadCast) {
    stdd::any value = 3.14;

    ASSERT_THROW(stdd::any_cast<int>(value), std::bad_cast);
}

TEST(Any, CopyMove) {
    stdd::any first = std::string("abc");
    stdd::any second = first;
    stdd::any third = std::move(second);

    ASSERT_EQ(stdd::any_cast<std::string>(first), "abc");
    ASSERT_EQ(stdd::any_cast<std::string>(third), "abc");
}
