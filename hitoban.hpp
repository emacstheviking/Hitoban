#pragma once

#include "htb_includes.hpp"
#include "htb_functions.hpp"
#include "htb_cell.hpp"
#include "htb_stdlib.hpp"
#include "htb_version.hpp"
#include "htb_binding.hpp"

// write a message to std::cout if value != expected_value
#define TEST_EQUAL(value, expected_value, litteral_value) test_equal_(value, expected_value, litteral_value, __FILE__, __LINE__)
// evaluate the given Hitoban expression and compare the result against the given expected_result
#define TEST(expr, expected_result) TEST_EQUAL(to_string(eval(read(expr), &global_env)), expected_result, expr)

namespace htb
{

void add_globals(environment& env);
void set_strict(bool s=true);
void set_tracking(bool t=true);
environment init_environment();
cell eval(cell x, environment* env);
void raise_tokenizing_error(const std::string& str, const std::string& s);
std::list<std::string> tokenize(const std::string& str);
cell atom(const std::string& token);
cell read_from(std::list<std::string>& tokens);
cell read(const std::string& s);
cell run_string(const std::string& code, environment* env);
void repl(const std::string& prompt, environment* env);
void print_shell_headers();

namespace internal {

cell read_htb_file(cell name, environment* baseenv, bool ns=true);
std::string load_htb_file(const std::string& name, environment* baseenv);

}  // namespace internal

template <typename Fun>
void register_function(Fun& f, const std::string& name, environment* env)
{
    env->register_function(f, name);
}

///////////////////////////////////////////////////// tests
static unsigned g_test_count;      // count of number of unit tests executed
static unsigned g_fault_count;     // count of number of unit tests that fail
template <typename T1, typename T2>
void test_equal_(const T1& value, const T2& expected_value, const char* litteral_val, const char* file, int line)
{
    ++g_test_count;
    if (value != expected_value)
    {
        std::cout
            << file << "(" << line << ") : "
            << " expected " << expected_value
            << ", got `" << value
            << "`, with " << litteral_val
            << std::endl;
        ++g_fault_count;
    }
}
int tests();

}  // namespace htb
