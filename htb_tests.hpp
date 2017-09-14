/*
* Code by Folaefolc
* A Lisp-like done just to concurrence Lisp itself (kind of crazy game for me)
* Interpreted programming language, C++14 ; main purpose is for video games
* License MIT
*/

#pragma once

#include "hitoban.hpp"
#include "htb_defines.hpp"

// write a message to std::cout if value != expected_value
#define TEST_EQUAL(value, expected_value, litteral_value) test_equal_(value, expected_value, litteral_value, __FILE__, __LINE__)
// evaluate the given Hitoban expression and compare the result against the given expected_result
#define TEST(expr, expected_result) TEST_EQUAL(to_string(eval(internal::read(expr), &global_env)), expected_result, expr)

namespace htb
{

    namespace tests
    {

        ///////////////////////////////////////////////////// tests

        static unsigned g_test_count;  // count of number of unit tests executed
        static unsigned g_fault_count;  // count of number of unit tests that fail
        template <typename T1, typename T2>
        void test_equal_(const T1& value, const T2& expected_value, const char* litteral_val, const char* file, int line)
        {
            ++g_test_count;
            if (value != expected_value)
            {
                std::cout << file << "(" << line << ") : "
                                << log(termcolor::red,
                                       "expected " << expected_value << ", got `" << value << "`, with " << litteral_val)
                                << std::endl;
                ++g_fault_count;
            }
        }
        int tests();

    } // namespace tests

}  // namespace htb
