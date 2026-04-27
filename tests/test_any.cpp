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

TEST(Any, EmptyThrows) {
    stdd::any a;
    ASSERT_THROW(stdd::any_cast<int>(a), std::bad_cast);
}

TEST(Any, Bool) {
    stdd::any a = true;
    ASSERT_EQ(stdd::any_cast<bool>(a), true);
}

struct MyStruct { int v; };

TEST(Any, Struct) {
    stdd::any a = MyStruct{5};
    auto s = stdd::any_cast<MyStruct>(a);
    ASSERT_EQ(s.v, 5);
}

TEST(Any, MoveFromRvalue) {
    stdd::any a = std::string("moved");
    auto s = stdd::any_cast<std::string>(std::move(a));
    ASSERT_EQ(s, "moved");
}

TEST(Any, AssignOverwrite) {
    stdd::any a = 10;
    a = std::string("now");
    ASSERT_EQ(stdd::any_cast<std::string>(a), "now");
}

TEST(Any, ModifyThroughRef) {
    stdd::any a = 10;
    int& r = stdd::any_cast<int&>(a);
    r = 20;
    ASSERT_EQ(stdd::any_cast<int>(a), 20);
}

TEST(Any, ConstAnyConstRef) {
    stdd::any a = 7;
    const stdd::any ca = a;
    const int& r = stdd::any_cast<const int&>(ca);
    ASSERT_EQ(r, 7);
}

TEST(Any, CopyIndependence) {
    std::string s = "orig";
    stdd::any a = s;
    s = "changed";
    ASSERT_EQ(stdd::any_cast<std::string>(a), "orig");
}

TEST(Any, MoveAssignLeavesEmpty) {
    stdd::any a = std::string("one");
    stdd::any b;
    b = std::move(a);
    ASSERT_EQ(stdd::any_cast<std::string>(b), "one");
    ASSERT_THROW(stdd::any_cast<std::string>(a), std::bad_cast);
}

TEST(Any, DoubleValue) {
    stdd::any a = 3.14;
    ASSERT_DOUBLE_EQ(stdd::any_cast<double>(a), 3.14);
}
