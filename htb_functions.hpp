#pragma once

#include "htb_includes.hpp"
#include "htb_types.hpp"

#define FILE_NOT_FOUND std::string("<file not found>")
#define LOAD_FILE(name) std::string content = ""; \
    if (check_if_file_exists(name)) { \
        content = read_file(name); \
    } /*else if (check_if_file_exists(STDLIB + "/" + name)) {}*/ \
    else { \
        content = FILE_NOT_FOUND; \
    }

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
std::string get_filename(const std::string& path);

} // namespace htb
