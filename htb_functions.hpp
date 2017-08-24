#pragma once

#include "htb_includes.hpp"
#include "htb_types.hpp"
#include "htb_cell.hpp"

namespace htb
{

// simple tools
template <typename T>
std::string str(T v)
{
    std::ostringstream os;
    os << v;

    return os.str();
}
long to_long(const std::string& s);
std::vector<std::string> split_string(const std::string& str, const std::string& delimiter);

// related to the eval loop
bool isdig(char c);
std::string convert_htbtype(cell_type type);

// related to the tokenizer
bool contains_only(const std::string& s, char t);
std::string to_string(const cell& exp, bool from_htb=false);

// about files
bool check_if_file_exists(const std::string& filename);
std::string read_file(const std::string& filename);
std::string get_filename(const std::string& path);
std::string get_fullpath(const std::string& name, environment* base);

} // namespace htb
