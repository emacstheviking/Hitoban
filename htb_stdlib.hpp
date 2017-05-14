#pragma once

#include "htb_types.hpp"
#include "htb_includes.hpp"
#include "htb_functions.hpp"
#include "htb_cell.hpp"

#define COPY(base, dest) cell dest(base.type); \
        if (dest.type == List) \
            dest.list = base.list; \
        else if (dest.type == Dict) \
            dest.dict = base.dict; \
        else \
            dest.val = base.val;

namespace htb
{

///////////////////////////////////////////////////// built-in primitive procedures

cell proc_add(const cells& c);
cell proc_sub(const cells& c);
cell proc_mul(const cells& c);
cell proc_div(const cells& c);

cell proc_greater(const cells& c);
cell proc_less(const cells& c);
cell proc_less_equal(const cells& c);
cell proc_eq(const cells& c);

cell proc_nullp(const cells& c);
cell proc_length(const cells& c);
cell proc_car(const cells& c);
cell proc_cdr(const cells& c);
cell proc_append(const cells& c);
cell proc_cons(const cells& c);
cell proc_list(const cells& c);
cell proc_nth(const cells& c);

cell proc_dict(const cells& c);
cell proc_keys(const cells& c);
cell proc_values(const cells& c);

std::map<std::string, cell> get_builtin();

} // namespace htb
