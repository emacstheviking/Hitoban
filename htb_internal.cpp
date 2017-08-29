#include "hitoban.hpp"

namespace htb {

namespace internal {

cell _read_file(cell name, environment* baseenv, bool ns)
{
    RAISE_IF(name.type != String, "'require' needs strings, not " << convert_htbtype(name.type))
    std::string content = _load_file(name.val, baseenv);
    RAISE_IF(content == FILE_NOT_FOUND, "Can not find the required file '" << name.val << "'")

    std::string _filename = to_string(name, true);
    std::string _fullname = get_fullpath(name.val, baseenv);
    baseenv->isfile = true;
    baseenv->fname = _fullname;

    if (ns)
    {
        // if lib in filename, do not add sub env, the lib is creating its own namespaces
        if (_fullname.length() > 3 && _fullname.substr(0, 3) == "lib")
        {
            HANDLE_EXCEPTION(run_string(content, baseenv))
        }
        else
        {
            environment* sub = baseenv->get_namespace(get_filename(_filename));
            // use sub to run the file because it isn't a file from the lib
            HANDLE_EXCEPTION(run_string(content, sub))
        }
    }
    else
    {
        HANDLE_EXCEPTION(run_string(content, baseenv))
    }

    return nil;
}

std::string _load_file(const std::string& name, environment* baseenv)
{
    std::string content = "";
    // extend path regarding to the environment
    std::string fullname = get_fullpath(name, baseenv);
    // check in user code and in the lib
    bool exists = check_if_file_exists(fullname) | check_if_file_exists(name);
    if (exists)
    {
        if (name.length() > 3 && name.substr(0, 3) == "lib")
            content = read_file(name);
        else
            content = read_file(fullname);
    }
    else
        content = FILE_NOT_FOUND;

    return content;
}

}  // namespace internal

}  // namespace htb
