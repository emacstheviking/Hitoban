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

#include <tuple>
#include <memory>
#include <typeindex>
#include <functional>
#include <iterator>

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

