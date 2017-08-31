#include "htb_binding.hpp"

#include "htb_types.hpp"
#include "htb_cell.hpp"
#include "htb_functions.hpp"

namespace htb
{

std::shared_ptr<internal::AbsObj> convert_cell(const cell& c)
{
    cell_type t = c.type;
    std::string v = c.val;

    if (t == Number)
        return std::make_shared<internal::TypedObj<long>>(to_long(v));
    else if (t == String)
        return std::make_shared<internal::TypedObj<std::string>>(v);
    else if (t == Symbol)
    {
        if (v == false_sym.val)
            return std::make_shared<internal::TypedObj<bool>>(false);
        else if (v == true_sym.val)
            return std::make_shared<internal::TypedObj<bool>>(true);
        else if (v == nil.val)
            return std::make_shared<internal::TypedObj<int>>(0);
        // any other types of Symbol are not supported, so we will just end up in the throw
        // catching both those kind of errors and types errors
    }
    throw std::runtime_error("Unsupported type for convert_cell : '" + convert_htbtype(t) + "'");
}

internal::absObjList convert_cells_to_values(const cells& c)
{
    internal::absObjList val;

    for (unsigned int i=0; i < c.size(); ++i)
    {
        cell_type t = c[i].type;
        std::string v = c[i].val;

        if (t == Number || t == String || t == Symbol)
            val.push_back(convert_cell(c[i]));
        else if (t == List)
        {
            internal::absObjList lst = convert_cells_to_values(c[i].list);
            val.push_back(std::make_shared<internal::TypedObj<internal::absObjList>>(lst));
        }
        else if (t == Dict)
        {
            /// make a true std::map, so check if the types of keys AND values are homogeneous
            internal::absObjMap map_;
            for (auto kv : c[i].dict)
            {
                map_[kv.first] = convert_cell(kv.second);
            }
            val.push_back(std::make_shared<internal::TypedObj<internal::absObjMap>>(map_));
        }
        else
            throw std::runtime_error("Unsupported type for convert_cells_to_values : " + convert_htbtype(t));
    }
    return val;
}

}  // namespace htb
