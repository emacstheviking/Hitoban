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

#define EXITSUCCESS 0x0
#define EXITFAILURE 0x1

#define RAISE_IF(cd, msg) if (cd) { \
        std::stringstream ss; ss << msg; \
        return cell(Exception, ss.str()); \
    }
#define COPY(base, dest) cell dest(base.type); \
        if (dest.type == List) \
            dest.list = base.list; \
        else if (dest.type == Dict) \
            dest.dict = base.dict; \
        else \
            dest.val = base.val;
#define HANDLE_EXCEPTION(c) if (c.type == Exception) { \
        std::stringstream ss; ss << c.val; \
        return cell(Exception, ss.str()); \
    }
