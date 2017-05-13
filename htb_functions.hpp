#pragma once

#include "htb_includes.hpp"
#include "htb_types.hpp"

namespace htb
{

// simple tools
std::string str(long n);
long to_long(const std::string& s);
std::vector<std::string> split_string(const std::string& str, const std::string& delimiter);

// related to the eval loop
bool isdig(char c);
std::string convert_htbtype(cell_type type);

// related to the tokenizer
bool contains_only(const std::string& s, char t);

// about files
bool check_if_file_exists(const std::string& filename);
std::string read_file(const std::string& filename);

} // namespace htb
