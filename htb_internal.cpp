#include "hitoban.hpp"

namespace htb {

namespace internal {

cell _read_file(cell* name, environment* baseenv, bool ns)
{
    RAISE_IF(name->type != String, "'require' needs strings, not " << convert_htbtype(name->type))
    std::string content = _load_file(name->val, baseenv);
    RAISE_IF(content == FILE_NOT_FOUND, "Can not find the required file '" << name->val << "'")

    baseenv->isfile = true;
    baseenv->fname = fullname;

    std::string _filename = to_string(*name, true);
    if (ns)
    {
        /// if lib in filename extended, do not add sub env
        if (_filename.length() > 3 && _filename.substr(0, 3) == "lib")
            run_string(content, baseenv);
        else
        {
            environment* sub = env->get_namespace(get_filename(_filename));
            run_string(content, sub);
        }  /// use sub to run the file
    }
    else
        run_string(content, baseenv);

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
