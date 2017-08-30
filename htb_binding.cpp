#include "htb_binding.hpp"

#include "htb_functions.hpp"
#include "htb_types.hpp"
#include "htb_cell.hpp"

namespace htb
{

std::shared_ptr<internal::Property> convert_cell(const cell& c)
{
    cell_type t = c.type;
    std::string v = c.val;

    if (t == Number)
        return std::make_shared<internal::TypedProperty<long>>(to_long(v));
    else if (t == String)
        return std::make_shared<internal::TypedProperty<std::string>>(v);
    else
        throw std::runtime_error("Unsupported type for convert_cell : '" + convert_htbtype(t) + "'");
}

internal::propertyListType convert_cells_to_values(const cells& c)
{
    internal::propertyListType val;

    for (unsigned int i=0; i < c.size(); ++i)
    {
        cell_type t = c[i].type;
        std::string v = c[i].val;

        if (t == Number || t == String)
            val.push_back(convert_cell(c[i]));
        else if (t == List)
        {
            internal::propertyListType lst = convert_cells_to_values(c[i].list);
            val.push_back(std::make_shared<internal::TypedProperty<internal::propertyListType>>(lst));
        }
        else if (t == Dict)
        {
            internal::propertyMapType map_;
            for (auto kv : c[i].dict)
            {
                map_[kv.first] = convert_cell(kv.second);
            }
            val.push_back(std::make_shared<internal::TypedProperty<internal::propertyMapType>>(map_));
        }
        else
            throw std::runtime_error("Unsupported type for convert_cells_to_values : " + convert_htbtype(t));
    }
    return val;
}

}  // namespace htb
