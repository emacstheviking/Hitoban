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
    Fun,
    Lambda,
    Exception
};

} // namespace htb
