#pragma once

namespace htb
{

enum cell_type {
    Symbol,
    Number,
    String,
    List,
    Dict,
    Proc,
    Lambda,
    Facultative,
    Exception
};

} // namespace htb
