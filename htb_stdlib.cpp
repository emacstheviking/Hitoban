/*
* Code by Folaefolc
* A Lisp-like done just to concurrence Lisp itself (kind of crazy game for me)
* Interpreted programming language, C++14 ; main purpose is for video games
* License MIT
*/

#include "htb_stdlib.hpp"

namespace htb
{

    cell proc_add(const cells& c)
    {
        long n = internal::str_to<long>(c[0].val);
        for (cellit i = c.begin()+1; i != c.end(); ++i)
        {
            HTB_RAISE_IF(i->type != Number, "Not a number : " << i->val)
            n += internal::str_to<long>(i->val);
        }

        return cell(Number, n);
    }

    cell proc_sub(const cells& c)
    {
        long n = internal::str_to<long>(c[0].val);
        if (c.size() > 1)
        {
            for (cellit i = c.begin()+1; i != c.end(); ++i)
            {
                HTB_RAISE_IF(i->type != Number, "Not a number : " << i->val)
                n -= internal::str_to<long>(i->val);
            }
        }
        else
            n = -n;

        return cell(Number, n);
    }

    cell proc_mul(const cells& c)
    {
        long n(1);
        for (cellit i = c.begin(); i != c.end(); ++i)
        {
            HTB_RAISE_IF(i->type != Number, "Not a number : " << i->val)
            n *= internal::str_to<long>(i->val);
        }

        return cell(Number, n);
    }

    cell proc_div(const cells& c)
    {
        long n = internal::str_to<long>(c[0].val);
        for (cellit i = c.begin()+1; i != c.end(); ++i)
        {
            HTB_RAISE_IF(i->type != Number, "Not a number : " << i->val)
            n /= internal::str_to<long>(i->val);
        }

        return cell(Number, n);
    }

    cell proc_and(const cells& c)
    {
        for (cellit i = c.begin(); i != c.end(); ++i)
        {
            if (i->val == false_sym.val)
                return false_sym;
            if (i->type == Number && i->val == "0")
                return false_sym;
            if (i->type == String && i->val.size() == 0)
                return false_sym;
            if (i->type == List && i->list.size() == 0)
                return false_sym;
            if (i->type == Dict && i->dict.size() == 0)
                return false_sym;
        }
        return true_sym;
    }

    cell proc_or(const cells& c)
    {
        for (cellit i = c.begin(); i != c.end(); ++i)
        {
            if (i->val == true_sym.val)
                return true_sym;
            if (i->type == Number && i->val != "0")
                return true_sym;
            if (i->type == String && i->val.size() > 0)
                return true_sym;
            if (i->type == List && i->list.size() > 0)
                return true_sym;
            if (i->type == Dict && i->dict.size() > 0)
                return true_sym;
        }
        return false_sym;
    }

    cell proc_pow(const cells& c)
    {
        return cell(Number, std::pow(internal::str_to<long>(c[0].val), internal::str_to<long>(c[1].val)));
    }

    cell proc_modulo(const cells& c)
    {
        return cell(Number, internal::str_to<long>(c[0].val) % internal::str_to<long>(c[1].val));
    }

    cell proc_greater(const cells& c)
    {
        long n = internal::str_to<long>(c[0].val);
        for (cellit i = c.begin()+1; i != c.end(); ++i)
        {
            HTB_RAISE_IF(i->type != Number, "Not a number : " << i->val)
            if (n <= internal::str_to<long>(i->val))
                return false_sym;
        }
        return true_sym;
    }

    cell proc_great_equal(const cells& c)
    {
        long n = internal::str_to<long>(c[0].val);
        for (cellit i = c.begin()+1; i != c.end(); ++i)
        {
            HTB_RAISE_IF(i->type != Number, "Not a number : " << i->val)
            if (n < internal::str_to<long>(i->val))
                return false_sym;
        }
        return true_sym;
    }

    cell proc_less(const cells& c)
    {
        long n = internal::str_to<long>(c[0].val);
        for (cellit i = c.begin()+1; i != c.end(); ++i)
        {
            HTB_RAISE_IF(i->type != Number, "Not a number : " << i->val)
            if (n >= internal::str_to<long>(i->val))
                return false_sym;
        }
        return true_sym;
    }

    cell proc_less_equal(const cells& c)
    {
        long n = internal::str_to<long>(c[0].val);
        for (cellit i = c.begin()+1; i != c.end(); ++i)
        {
            HTB_RAISE_IF(i->type != Number, "Not a number : " << i->val)
            if (n > internal::str_to<long>(i->val))
                return false_sym;
        }
        return true_sym;
    }

    cell proc_eq(const cells& c)
    {
        long n = internal::str_to<long>(c[0].val);
        for (cellit i = c.begin()+1; i != c.end(); ++i)
        {
            HTB_RAISE_IF(i->type != Number, "Not a number : " << i->val)
            long b = internal::str_to<long>(i->val);
            if (n < b || n > b)
                return false_sym;
        }
        return true_sym;
    }

    cell proc_cond(const cells& c)
    {
        cell result(Symbol);

        for (cellit i = c.begin()+1; i != c.end(); ++i)
        {
            if (i->list[0].val == nil.val)
            {
                cell r(i->list[1]);
                return r;
            }
            if (i->list[0].val == true_sym.val)
            {
                cell r(i->list[1]);
                return r;
            }
        }

        return nil;
    }

    cell proc_length(const cells& c)
    {
        if (c[0].type == String)
            return cell(Number, c[0].val.size());
        return cell(Number, c[0].list.size());
    }

    cell proc_nullp(const cells& c)
    {
        return c[0].list.empty() ? true_sym : false_sym;
    }

    cell proc_car(const cells& c)
    {
        HTB_RAISE_IF(c[0].list.empty(), "List empty")
        return c[0].list[0];
    }

    cell proc_cdr(const cells& c)
    {
        if (c[0].list.size() < 2)
            return nil;

        cell result(c[0]);
        result.list.erase(result.list.begin());

        return result;
    }

    cell proc_append(const cells& c)
    {
        cell result(List);
        result.list = c[0].list;
        for (cellit i = c[1].list.begin(); i != c[1].list.end(); ++i)
        {
            HTB_HANDLE_EXCEPTION((*i))
            result.list.push_back(*i);
        }

        return result;
    }

    cell proc_cons(const cells& c)
    {
        cell result(List);
        result.list.push_back(c[0]);
        for (cellit i = c[1].list.begin(); i != c[1].list.end(); ++i)
        {
            HTB_HANDLE_EXCEPTION((*i))
            result.list.push_back(*i);
        }

        return result;
    }

    cell proc_list(const cells& c)
    {
        cell result(List);
        result.list = c;

        return result;
    }

    cell proc_nth(const cells& c)
    {
        if (c[1].type == List)
        {
            long n = internal::str_to<long>(c[0].val);
            cell temp(c[1]);
            HTB_COPY(temp.get_in(n), r)
            return r;
        }
        else if (c[1].type == Dict)
        {
            HTB_RAISE_IF(c[0].type != String, "Keys in dict are of type string, not of type " << internal::convert_htbtype(c[0].type))
            cell temp(c[1]);
            HTB_COPY(temp.get_in(c[0].val), r)
            return r;
        }
        else if (c[1].type == String)
        {
            long n = internal::str_to<long>(c[0].val);
            HTB_RAISE_IF(n >= long(c[1].val.size()), "'nth' can not get a character at pos " << n << " because it is outside the string")
            return cell(String, std::string(1, c[1].val[n]));
        }
        // we should not be there
        HTB_RAISE("The object should be of type list, dict or string, not of type " << internal::convert_htbtype(c[1].type))
    }

    cell proc_dict(const cells& c)
    {
        cell result(Dict);

        for (cellit i = c.begin(); i != c.end(); ++i)
        {
            HTB_RAISE_IF(i->type != List, "Arguments of 'dict' should be of type list, not of type " << internal::convert_htbtype(i->type))
            HTB_HANDLE_EXCEPTION(i->list[0])
            HTB_RAISE_IF(i->list[0].type != String, "Keys for 'dict' should only be of type string, not of type " << internal::convert_htbtype(i->list[0].type))
            std::string key(i->list[0].val);
            HTB_RAISE_IF(i->list.size() > 2,"Lists to define (key value) in dict should not be of size " << i->list.size())  // we have more than 2 elements, not normal
            cell v(i->list[1]);
            result.dict[key] = v;
        }

        return result;
    }

    cell proc_keys(const cells& c)
    {
        HTB_RAISE_IF(c[0].type != Dict, "'keys' argument's should be of type dict, not of type " << internal::convert_htbtype(c[0].type))
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
        HTB_RAISE_IF(c[0].type != Dict, "'values' argument's should be of type dict, not of type " << internal::convert_htbtype(c[0].type))
        cell result(List);

        for (auto kv: c[0].dict)
        {
            cell v(kv.second);
            result.list.push_back(v);
        }

        return result;
    }

    cell proc_print(const cells& c)
    {
        for (cellit i = c.begin(); i != c.end(); ++i)
        {
            std::cout << to_string(*i, true) << " ";
        }
        std::cout << std::endl;
        return nil;
    }

    cell proc_prin1(const cells& c)
    {
        HTB_RAISE_IF(c[0].type != String, "'prin1' argument's should be of type string, not of type " << internal::convert_htbtype(c[0].type))
        std::cout << c[0].val;
        return nil;
    }

    cell proc_input(const cells& c)
    {
        if (c.size() == 1)
            std::cout << c[0].val;
        std::string o;
        std::cin >> o;
        return cell(String, o);
    }

    cell proc_getc(const cells&)
    {
        char _c;
        std::cin.get(_c);
        return cell(String, std::string(1, _c));
    }

    cell proc_system(const cells& c)
    {
        HTB_RAISE_IF(c[0].type != String, "'system' argument's should of type string, not of type " << internal::convert_htbtype(c[0].type))
        return cell(Number, system(c[0].val.c_str()));
    }

    cell proc_str_eq(const cells& c)
    {
        HTB_RAISE_IF(c[0].type != String, "'str-eq' arguments' should be of type string, not of type " << internal::convert_htbtype(c[0].type))
        std::string f = c[0].val;
        for (cellit i = c.begin()+1; i != c.end(); ++i)
        {
            HTB_RAISE_IF(i->type != String, "'str-eq' arguments' should be of type string, not of type " << internal::convert_htbtype(i->type))
            if (i->val != f)
                return false_sym;
        }
        return true_sym;
    }

    cell proc_str_cat(const cells& c)
    {
        HTB_RAISE_IF(c[0].type != String, "'str-cat' arguments' should be of type string, not of type " << internal::convert_htbtype(c[0].type))
        std::string f = c[0].val;
        for (cellit i = c.begin()+1; i != c.end(); ++i)
        {
            HTB_RAISE_IF(i->type != String, "'str-cat' arguments' should be of type string, not of type " << internal::convert_htbtype(i->type))
            f += i->val;
        }
        return cell(String, f);
    }

    cell proc_str_reverse(const cells& c)
    {
        HTB_RAISE_IF(c[0].type != String, "'str-reverse' argument's should be of type string, not of type " << internal::convert_htbtype(c[0].type))
        std::string s = c[0].val;
        std::reverse(s.begin(), s.end());
        return cell(String, s);
    }

    cell proc_str_format(const cells& c)
    {
        // see https://github.com/ryjen/format for the syntax of the format engine
        HTB_RAISE_IF(c[0].type != String, "'str-format' first argument should be of type string, not of type " << internal::convert_htbtype(c[0].type))

        try
        {
            rj::format f(c[0].val);
            for (cellit i = c.begin()+1; i != c.end(); ++i)
            {
                HTB_RAISE_IF(i->type != String && i->type != Number,
                         "'str-format' arguments' should be of type string, not of type " << internal::convert_htbtype(i->type))
                if (i->type == Number)
                    f.args(internal::str_to<long>(i->val));
                else
                    f.args(i->val);
            }
            std::string msg = f;
            return cell(String, msg);
        }
        catch (const std::invalid_argument& e)
        {
            HTB_RAISE(e.what())
        }

        // we should never be there, because we either return something in the try or in the catch
        return nil;
    }

    cell proc_typeof(const cells& c)
    {
        return cell(String, internal::convert_htbtype(c[0].type));
    }

    cell proc_random(const cells& c)
    {
        long n;
        std::mt19937 gen;
        if (c.size() == 0)
        {
            std::uniform_real_distribution<> d(0, 1);
            n = d(gen);
        }
        else if (c.size() == 1)
        {
            HTB_RAISE_IF(c[0].type != Number, "'random' arguments' should be of type number, not of type " << internal::convert_htbtype(c[0].type))
            std::uniform_int_distribution<> d(0, internal::str_to<long>(c[0].val));
            n = d(gen);
        }
        else
        {
            HTB_RAISE_IF(c[0].type != Number, "'random' arguments' should be of type number, not of type " << internal::convert_htbtype(c[0].type))
            HTB_RAISE_IF(c[1].type != Number, "'random' arguments' should be of type number, not of type " << internal::convert_htbtype(c[1].type))
            std::uniform_int_distribution<> d(internal::str_to<long>(c[0].val), internal::str_to<long>(c[1].val));
            n = d(gen);
        }
        return cell(Number, n);
    }

    ///////////////////////////////////////////////////// built-in functions
    std::map<std::string, cell> get_builtin()
    {
        std::map<std::string, cell> builtin;

        /* list */
        builtin["append"] = cell(&proc_append, AT_LEAST_2_ARGS);
        builtin["car"] = cell(&proc_car, 1);
        builtin["cdr"]  = cell(&proc_cdr, 1);
        builtin["cons"] = cell(&proc_cons, AT_LEAST_2_ARGS);
        builtin["length"] = cell(&proc_length, 1);
        builtin["list"] = cell(&proc_list, INF_NB_ARGS);
        builtin["null?"] = cell(&proc_nullp, 1);
        builtin["nth"] = cell(&proc_nth, 2);
        /* operators */
        builtin["+"] = cell(&proc_add, AT_LEAST_2_ARGS);
        builtin["-"] = cell(&proc_sub, AT_LEAST_2_ARGS);
        builtin["*"] = cell(&proc_mul, AT_LEAST_2_ARGS);
        builtin["/"] = cell(&proc_div, AT_LEAST_2_ARGS);
        builtin["&"] = cell(&proc_and, AT_LEAST_2_ARGS);
        builtin["|"] = cell(&proc_or, AT_LEAST_2_ARGS);
        builtin["^"] = cell(&proc_pow, 2);
        builtin["%"] = cell(&proc_modulo, 2);
        /* conditionals operators */
        builtin[">"]  = cell(&proc_greater, AT_LEAST_2_ARGS);
        builtin["<"] = cell(&proc_less, AT_LEAST_2_ARGS);
        builtin["<="] = cell(&proc_less_equal, AT_LEAST_2_ARGS);
        builtin[">="] = cell(&proc_great_equal, AT_LEAST_2_ARGS);
        builtin["="] = cell(&proc_eq, AT_LEAST_2_ARGS);
        builtin["cond"] = cell(&proc_cond, AT_LEAST_1_ARGS);
        /* dictionary */
        builtin["dict"] = cell(&proc_dict, AT_LEAST_1_ARGS);
        builtin["keys"] = cell(&proc_keys, 1);
        builtin["values"] = cell(&proc_values, 1);
        /* IO */
        builtin["print"] = cell(&proc_print, AT_LEAST_1_ARGS);
        builtin["prin1"] = cell(&proc_prin1, 1);
        builtin["input"] = cell(&proc_input, BETWEEN_0_1_ARGS);
        builtin["getc"] = cell(&proc_getc, 0);
        /* other */
        builtin["system"] = cell(&proc_system, 1);
        /* strings */
        builtin["str-eq"] = cell(&proc_str_eq, AT_LEAST_2_ARGS);
        builtin["str-cat"] = cell(&proc_str_cat, AT_LEAST_1_ARGS);
        builtin["str-reverse"] = cell(&proc_str_reverse, 1);
        builtin["str-format"] = cell(&proc_str_format, AT_LEAST_2_ARGS);
        /* types */
        builtin["typeof"] = cell(&proc_typeof, 1);
        /* random */
        builtin["random"] = cell(&proc_random, BETWEEN_0_2_ARGS);

        return builtin;
    }

} // namespace htb
