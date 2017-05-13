#include "htb_functions.hpp"

namespace htb
{

std::string str(long n)
{
    std::ostringstream os;
    os << n;

    return os.str();
}

long to_long(const std::string& s)
{
    std::stringstream b(s);
    long out;
    b >> out;

    return out;
}

std::vector<std::string> split_string(const std::string& str, const std::string& delimiter)
{
    std::vector<std::string> strings;

    std::string::size_type pos = 0;
    std::string::size_type prev = 0;
    while ((pos = str.find(delimiter, prev)) != std::string::npos)
    {
        strings.push_back(str.substr(prev, pos - prev));
        prev = pos + 1;
    }

    // to get the last substring (or only, if delimiter is not found)
    strings.push_back(str.substr(prev));

    return strings;
}

bool isdig(char c)
{
    return isdigit(static_cast<unsigned char>(c)) != 0;
}

std::string convert_htbtype(cell_type type)
{
    std::string out = "";

    if (type == Symbol)
        out = "Symbol";
    else if (type == Number)
        out = "Number";
    else if (type == String)
        out = "String";
    else if (type == List)
        out = "List";
    else if (type == Dict)
        out = "Dict";
    else if (type == Proc)
        out = "Proc";
    else if (type == Lambda)
        out = "Lambda";
    else if (type == Exception)
        out = "Exception";

    return out;
}

bool contains_only(const std::string& s, char t)
{
    bool ok = true;
    for (auto& c: s)
    {
        if (c != t)
        {
            ok = false;
            break;
        }
    }
    return ok;
}

bool check_if_file_exists(const std::string& filename)
{
    std::ifstream f(filename.c_str());
    return f.good();
}

std::string read_file(const std::string& filename)
{
    std::ifstream file(filename.c_str());
    std::stringstream ss;

    ss << file.rdbuf();

    return ss.str();
}

} // namespace htb
