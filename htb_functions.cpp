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

// convert given cell to a Hitoban-readable string
std::string to_string(const cell& exp, bool from_htb)
{
    if (exp.type == List)
    {
        std::string s("(");
        for (cell::iter e = exp.list.begin(); e != exp.list.end(); ++e)
            s += to_string(*e) + ' ';
        if (s[s.size() - 1] == ' ')
            s.erase(s.size() - 1);
        return s + ')';
    }
    else if (exp.type == Lambda)
        return "<Lambda>";
    else if (exp.type == Proc)
        return "<Proc>";
    else if (exp.type == Exception)
        return "<Exception> " + exp.val;
    else if (exp.type == Dict && !from_htb)
        return "<Dict>";
    else if (exp.type == Dict && from_htb)
        return "{}";
    else if (exp.type == String && !from_htb)
        return "\"" + exp.val + "\"";
    else if (exp.type == String && from_htb)
        return exp.val;
    return exp.val;
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

std::string get_filename(const std::string& path)
{
    std::size_t found = path.find_last_of("/\\");
    return path.substr(found + 1);
}

} // namespace htb
