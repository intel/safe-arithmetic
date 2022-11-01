#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <safe.hpp>


using ::testing::_;
using ::testing::Return;
using ::testing::InSequence;


using namespace safe::interval_types;
using namespace safe::int_types;
using namespace safe::literals;

using safe::ival;

struct mock_function {
    MOCK_METHOD(void, two_safe_vars, (int, int));
};

mock_function * mock_function_ptr{};

class safe_invoke_test : public ::testing::Test {
protected:
    mock_function my_mock_function;

    void SetUp() override {
        mock_function_ptr = &my_mock_function;
    }
};

void two_safe_vars(
    ival_s32<0, 10> a,
    ival_s32<0, 10> b
) {
    mock_function_ptr->two_safe_vars(a.unsafe_value(), b.unsafe_value());
}

TEST_F(safe_invoke_test, simple_pass) {
    EXPECT_CALL(my_mock_function, two_safe_vars(2, 5)).Times(1);

    bool const pass = safe::invoke(two_safe_vars, 2, 5);
    EXPECT_TRUE(pass);
}

TEST_F(safe_invoke_test, pass_with_an_input_var) {
    EXPECT_CALL(my_mock_function, two_safe_vars(9, 3)).Times(1);

    bool const pass = safe::invoke(two_safe_vars, 9, 3_s32);
    EXPECT_TRUE(pass);
}

TEST_F(safe_invoke_test, pass_with_both_input_vars) {
    EXPECT_CALL(my_mock_function, two_safe_vars(0, 4)).Times(1);

    bool const pass = safe::invoke(two_safe_vars, 0_s32, 4_s32);
    EXPECT_TRUE(pass);
}

TEST_F(safe_invoke_test, simple_fail) {
    EXPECT_CALL(my_mock_function, two_safe_vars(_, _)).Times(0);

    bool const fail = safe::invoke(two_safe_vars, 12, 5);
    EXPECT_FALSE(fail);
}

TEST_F(safe_invoke_test, fail_with_an_input_var) {
    EXPECT_CALL(my_mock_function, two_safe_vars(_, _)).Times(0);

    bool const fail = safe::invoke(two_safe_vars, 11, 9_s32);
    EXPECT_FALSE(fail);
}

