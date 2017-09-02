/*
* Code by Folaefolc
* A Lisp-like done just to concurrence Lisp itself (kind of crazy game for me)
* Interpreted programming language, C++14 ; main purpose is for video games
* License MIT
*/

#pragma once

namespace htb
{

    enum cell_type
    {
        Symbol,
        Number,
        String,
        List,
        Dict,
        Proc,
        Lambda,
        Exception
    };

} // namespace htb
