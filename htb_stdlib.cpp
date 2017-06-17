#include "htb_stdlib.hpp"

namespace htb
{

cell proc_add(const cells& c)
{
    RAISE_IF(c.size() < 2, "'add' needs at least two arguments")
    HANDLE_EXCEPTION(c[0])
    long n = to_long(c[0].val);
    for (cellit i = c.begin()+1; i != c.end(); ++i)
    {
        HANDLE_EXCEPTION((*i))
        RAISE_IF(i->type != Number, "Not a number : " << i->val)
        n += to_long(i->val);
    }

    return cell(Number, str(n));
}

cell proc_sub(const cells& c)
{
    RAISE_IF(c.size() < 1, "'sub' needs at least one argument")
    HANDLE_EXCEPTION(c[0])
    long n = to_long(c[0].val);
    if (c.size() > 1)
    {
        for (cellit i = c.begin()+1; i != c.end(); ++i)
        {
            HANDLE_EXCEPTION((*i))
            RAISE_IF(i->type != Number, "Not a number : " << i->val)
            n -= to_long(i->val);
        }
    }
    else
        n = -n;

    return cell(Number, str(n));
}

cell proc_mul(const cells& c)
{
    RAISE_IF(c.size() < 2, "'mul' needs at least two arguments")
    long n(1);
    for (cellit i = c.begin(); i != c.end(); ++i)
    {
        HANDLE_EXCEPTION((*i))
        RAISE_IF(i->type != Number, "Not a number : " << i->val)
        n *= to_long(i->val);
    }

    return cell(Number, str(n));
}

cell proc_div(const cells& c)
{
    RAISE_IF(c.size() < 2, "'div' needs at least two arguments")
    HANDLE_EXCEPTION(c[0])
    long n = to_long(c[0].val);
    for (cellit i = c.begin()+1; i != c.end(); ++i)
    {
        HANDLE_EXCEPTION((*i))
        RAISE_IF(i->type != Number, "Not a number : " << i->val)
        n /= to_long(i->val);
    }

    return cell(Number, str(n));
}

cell proc_and(const cells& c)
{
    RAISE_IF(c.size() < 2, "'and' needs at least two arguments")
    for (unsigned int i = 0; i < c.size(); ++i)
    {
        HANDLE_EXCEPTION(c[i])
        if (c[i].type == Symbol && c[i].val == false_sym.val)
            return false_sym;
    }
    return true_sym;
}

cell proc_or(const cells& c)
{
    RAISE_IF(c.size() < 2, "'or' needs at least two arguments")
    for (unsigned int i = 0; i < c.size(); ++i)
    {
        HANDLE_EXCEPTION(c[i])
        if (c[i].type == Symbol && c[i].val == true_sym.val)
            return true_sym;
    }
    return false_sym;
}

cell proc_pow(const cells& c)
{
    RAISE_IF(c.size() != 2, "'pow' needs only two arguments")
    HANDLE_EXCEPTION(c[0])
    HANDLE_EXCEPTION(c[1])
    return cell(Number, str(std::pow(to_long(c[0].val), to_long(c[1].val))));
}

cell proc_greater(const cells& c)
{
    RAISE_IF(c.size() < 2, "'gt' needs at least two arguments")
    HANDLE_EXCEPTION(c[0])
    long n = to_long(c[0].val);
    for (cellit i = c.begin()+1; i != c.end(); ++i)
    {
        HANDLE_EXCEPTION((*i))
        RAISE_IF(i->type != Number, "Not a number : " << i->val)
        if (n <= to_long(i->val))
            return false_sym;
    }
    return true_sym;
}

cell proc_less(const cells& c)
{
    RAISE_IF(c.size() < 2, "'lt' needs at least two arguments")
    HANDLE_EXCEPTION(c[0])
    long n = to_long(c[0].val);
    for (cellit i = c.begin()+1; i != c.end(); ++i)
    {
        HANDLE_EXCEPTION((*i))
        RAISE_IF(i->type != Number, "Not a number : " << i->val)
        if (n >= to_long(i->val))
            return false_sym;
    }
    return true_sym;
}

cell proc_less_equal(const cells& c)
{
    RAISE_IF(c.size() < 2, "'le' needs at least two arguments")
    HANDLE_EXCEPTION(c[0])
    long n = to_long(c[0].val);
    for (cellit i = c.begin()+1; i != c.end(); ++i)
    {
        HANDLE_EXCEPTION((*i))
        RAISE_IF(i->type != Number, "Not a number : " << i->val)
        if (n > to_long(i->val))
            return false_sym;
    }
    return true_sym;
}

cell proc_eq(const cells& c)
{
    RAISE_IF(c.size() < 2, "'eq' needs at least two arguments")
    HANDLE_EXCEPTION(c[0])
    long n = to_long(c[0].val);
    for (cellit i = c.begin()+1; i != c.end(); ++i)
    {
        HANDLE_EXCEPTION((*i))
        RAISE_IF(i->type != Number, "Not a number : " << i->val)
        long b = to_long(i->val);
        if (n < b || n > b)
            return false_sym;
    }
    return true_sym;
}

cell proc_cond(const cells& c)
{
    RAISE_IF(c.size() < 1, "'cond' need at least one argument")
    cell result(Symbol);

    for (cellit i = c.begin()+1; i != c.end(); ++i)
    {
        HANDLE_EXCEPTION((*i))
        if (i->list[0].val == nil.val)
        {
            COPY((*i).list[1], result)
            return result;
        }
        if (i->list[0].val == true_sym.val)
        {
            COPY((*i).list[1], result)
            return result;
        }
    }

    return nil;
}

cell proc_length(const cells& c)
{
    RAISE_IF(c.size() != 1, "'length' needs only one argument")
    HANDLE_EXCEPTION(c[0])
    if (c[0].type == String)
        return cell(Number, str(c[0].val.size()));
    return cell(Number, str(c[0].list.size()));
}

cell proc_nullp(const cells& c)
{
    RAISE_IF(c.size() != 1, "'null?' needs only one argument")
    HANDLE_EXCEPTION(c[0])
    return c[0].list.empty() ? true_sym : false_sym;
}

cell proc_car(const cells& c)
{
    RAISE_IF(c.size() != 1, "'car' needs only one argument")
    RAISE_IF(c[0].list.empty(), "List empty")
    HANDLE_EXCEPTION(c[0])
    return c[0].list[0];
}

cell proc_cdr(const cells& c)
{
    RAISE_IF(c.size() != 1, "'cdr' needs only one argument")
    HANDLE_EXCEPTION(c[0])
    if (c[0].list.size() < 2)
        return nil;

    cell result(c[0]);
    result.list.erase(result.list.begin());

    return result;
}

cell proc_append(const cells& c)
{
    RAISE_IF(c.size() < 2, "'append' needs at least two arguments")
    HANDLE_EXCEPTION(c[0])
    cell result(List);
    result.list = c[0].list;
    for (cellit i = c[1].list.begin(); i != c[1].list.end(); ++i)
    {
        HANDLE_EXCEPTION((*i))
        result.list.push_back(*i);
    }

    return result;
}

cell proc_cons(const cells& c)
{
    RAISE_IF(c.size() < 2, "'cons' needs at least two arguments")
    HANDLE_EXCEPTION(c[0])
    cell result(List);
    result.list.push_back(c[0]);
    for (cellit i = c[1].list.begin(); i != c[1].list.end(); ++i)
    {
        HANDLE_EXCEPTION((*i))
        result.list.push_back(*i);
    }

    return result;
}

cell proc_list(const cells& c)
{
    cell result(List);
    for (unsigned int i = 0; i < c.size(); ++i)
    {
        HANDLE_EXCEPTION(c[i])
    }

    result.list = c;

    return result;
}

cell proc_nth(const cells& c)
{
    RAISE_IF(c.size() != 2, "'nth' needs only two arguments")
    HANDLE_EXCEPTION(c[0])
    HANDLE_EXCEPTION(c[1])
    if (c[1].type == List)
    {
        long n = to_long(c[0].val);
        cell tmp = c[1];
        const cell temp = tmp.get_in(n);

        COPY(temp, result)

        return result;
    }
    else if (c[1].type == Dict)
    {
        cell tmp = c[1];
        RAISE_IF(c[0].type != String, "Keys in dict are of type string, not of type " << convert_htbtype(c[0].type))
        const cell temp = tmp.get_in(c[0].val);

        COPY(temp, result)

        return result;
    }
    // we should not be there, so raise if true because before we must have returned immediately
    RAISE_IF(true, "The object should be of type list or dict, not of type " << convert_htbtype(c[1].type))
}

cell proc_dict(const cells& c)
{
    cell result(Dict);

    for (unsigned int i = 0; i < c.size(); ++i)
    {
        HANDLE_EXCEPTION(c[i])
        COPY(c[i], temp)

        RAISE_IF(temp.type != List, "Arguments of 'dict' should be of type list, not of type " << convert_htbtype(temp.type))
        HANDLE_EXCEPTION(temp.list[0])
        RAISE_IF(temp.list[0].type != String, "Keys for 'dict' should only be of type string, not of type " << convert_htbtype(temp.list[0].type))
        std::string key(temp.list[0].val);
        RAISE_IF(temp.list.size() > 2,"Lists to define (key value) in dict should not be of size " << temp.list.size())  // we have more than 2 elements, not normal
        COPY(temp.list[1], v)
        result.dict[key] = v;
    }

    return result;
}

cell proc_keys(const cells& c)
{
    RAISE_IF(c.size() != 1, "'keys' needs only one parameter : an object of type dict")
    HANDLE_EXCEPTION(c[0])
    RAISE_IF(c[0].type != Dict, "'keys' argument's should be of type dict, not of type " << convert_htbtype(c[0].type))
    cell result(List);

    for (auto kv: c[0].dict)
    {
        cell k(String);
        k.val = kv.first;

        result.list.push_back(k);
    }

    return result;
}

cell proc_values(const cells& c)
{
    RAISE_IF(c.size() != 1, "'values' needs only one argument : an object of type dict")
    HANDLE_EXCEPTION(c[0])
    RAISE_IF(c[0].type != Dict, "'values' argument's should be of type dict, not of type " << convert_htbtype(c[0].type))
    cell result(List);

    for (auto kv: c[0].dict)
    {
        COPY(kv.second, v)
        result.list.push_back(v);
    }

    return result;
}

cell proc_print(const cells& c)
{
    for (cellit i = c.begin(); i != c.end(); ++i)
    {
        HANDLE_EXCEPTION((*i))
        std::cout << to_string(*i, true) << " ";
    }
    std::cout << std::endl;
    return nil;
}

cell proc_prin1(const cells& c)
{
    RAISE_IF(c.size() != 1, "'prin1' needs only one argument")
    HANDLE_EXCEPTION(c[0])
    RAISE_IF(c[0].type != String, "'prin1' argument's should be of type string, not of type " << convert_htbtype(c[0].type))
    std::cout << c[0].val;
    return nil;
}

cell proc_system(const cells& c)
{
    RAISE_IF(c.size() != 1, "'system' needs only one argument")
    HANDLE_EXCEPTION(c[0])
    RAISE_IF(c[0].type != String, "'system' argument's should of type string, not of type " << convert_htbtype(c[0].type))

    return cell(Number, str(system(c[0].val.c_str())));
}

///////////////////////////////////////////////////// built-in functions
std::map<std::string, cell> get_builtin()
{
    std::map<std::string, cell> builtin;

    /* list */
    builtin["append"] = cell(&proc_append);
    builtin["car"] = cell(&proc_car);
    builtin["cdr"]  = cell(&proc_cdr);
    builtin["cons"] = cell(&proc_cons);
    builtin["length"] = cell(&proc_length);
    builtin["list"] = cell(&proc_list);
    builtin["null?"] = cell(&proc_nullp);
    builtin["nth"] = cell(&proc_nth);
    /* operators */
    builtin["+"] = cell(&proc_add);
    builtin["-"] = cell(&proc_sub);
    builtin["*"] = cell(&proc_mul);
    builtin["/"] = cell(&proc_div);
    builtin["&"] = cell(&proc_and);
    builtin["|"] = cell(&proc_or);
    builtin["^"] = cell(&proc_pow);
    /* conditionals operators */
    builtin[">"]  = cell(&proc_greater);
    builtin["<"] = cell(&proc_less);
    builtin["<="] = cell(&proc_less_equal);
    builtin["="] = cell(&proc_eq);
    builtin["cond"] = cell(&proc_cond);
    /* dictionary */
    builtin["dict"] = cell(&proc_dict);
    builtin["keys"] = cell(&proc_keys);
    builtin["values"] = cell(&proc_values);
    /* IO */
    builtin["print"] = cell(&proc_print);
    builtin["prin1"] = cell(&proc_prin1);
    /* other */
    builtin["system"] = cell(&proc_system);

    return builtin;
}

} // namespace htb
