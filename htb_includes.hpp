#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>

#include <exception>
#include <stdexcept>

#include <regex>
#include <algorithm>
#include <cctype>

#include <fstream>

#include <cmath>
#include <stdlib.h>
#include <cstdio>

#include <random>
#include "ext_lib/fmt/format.hpp"
#include <unordered_map>

#include "ext_lib/termcolor/termcolor.hpp"

#define EXITSUCCESS 0x0
#define EXITFAILURE 0x1

#define log(color, msg)  color << msg << termcolor::reset

#define RAISE(msg) {std::stringstream ss; ss << msg; return cell(Exception, ss.str());}
#define RAISE_IF(cd, msg) if (cd) { RAISE(msg) }
#define COPY(base, dest) cell dest(base.type); \
        if (dest.type == List) \
            dest.list = base.list; \
        else if (dest.type == Dict) \
            dest.dict = base.dict; \
        else \
            dest.val = base.val;
#define HANDLE_EXCEPTION(c) { if (c.type == Exception) return c; }
#define FILE_NOT_FOUND std::string("<file not found>")
#define LOAD_FILE(name, baseenv) std::string content = "", fullname = get_fullpath(name, baseenv); \
    bool exists = check_if_file_exists(fullname) | check_if_file_exists(name); \
    if (exists) { \
        if (name.length() > 3 && name.substr(0, 3) == "lib") content = read_file(name); \
        else content = read_file(fullname); \
    } else content = FILE_NOT_FOUND;
#define READ_FILE(name, env) HANDLE_EXCEPTION(name) \
        RAISE_IF(name.type != String, "'require' needs strings, not " << convert_htbtype(name.type)) \
        LOAD_FILE(name.val, env) \
        RAISE_IF(content == FILE_NOT_FOUND, "Can not find the required file '" << name.val << "'") \
        env->isfile = true; env->fname = fullname; \
        run_string(content, env);
