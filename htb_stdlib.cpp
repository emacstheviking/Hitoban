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
        RAISE_IF(c.size() < 2, "'add' needs at least two arguments")
        HANDLE_EXCEPTION(c[0])
        long n = internal::str_to<long>(c[0].val);
        for (cellit i = c.begin()+1; i != c.end(); ++i)
        {
            HANDLE_EXCEPTION((*i))
            RAISE_IF(i->type != Number, "Not a number : " << i->val)
            n += internal::str_to<long>(i->val);
        }

        return cell(Number, internal::str(n));
    }

    cell proc_sub(const cells& c)
    {
        RAISE_IF(c.size() < 1, "'sub' needs at least one argument")
        HANDLE_EXCEPTION(c[0])
        long n = internal::str_to<long>(c[0].val);
        if (c.size() > 1)
        {
            for (cellit i = c.begin()+1; i != c.end(); ++i)
            {
                HANDLE_EXCEPTION((*i))
                RAISE_IF(i->type != Number, "Not a number : " << i->val)
                n -= internal::str_to<long>(i->val);
            }
        }
        else
            n = -n;

        return cell(Number, internal::str(n));
    }

    cell proc_mul(const cells& c)
    {
        RAISE_IF(c.size() < 2, "'mul' needs at least two arguments")
        long n(1);
        for (cellit i = c.begin(); i != c.end(); ++i)
        {
            HANDLE_EXCEPTION((*i))
            RAISE_IF(i->type != Number, "Not a number : " << i->val)
            n *= internal::str_to<long>(i->val);
        }

        return cell(Number, internal::str(n));
    }

    cell proc_div(const cells& c)
    {
        RAISE_IF(c.size() < 2, "'div' needs at least two arguments")
        HANDLE_EXCEPTION(c[0])
        long n = internal::str_to<long>(c[0].val);
        for (cellit i = c.begin()+1; i != c.end(); ++i)
        {
            HANDLE_EXCEPTION((*i))
            RAISE_IF(i->type != Number, "Not a number : " << i->val)
            n /= internal::str_to<long>(i->val);
        }

        return cell(Number, internal::str(n));
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
        return cell(Number, internal::str(std::pow(internal::str_to<long>(c[0].val), internal::str_to<long>(c[1].val))));
    }

    cell proc_modulo(const cells& c)
    {
        RAISE_IF(c.size() != 2, "'modulo' needs only two arguments")
        HANDLE_EXCEPTION(c[0])
        HANDLE_EXCEPTION(c[1])
        return cell(Number, internal::str(internal::str_to<long>(c[0].val) % internal::str_to<long>(c[1].val)));
    }

    cell proc_greater(const cells& c)
    {
        RAISE_IF(c.size() < 2, "'gt' needs at least two arguments")
        HANDLE_EXCEPTION(c[0])
        long n = internal::str_to<long>(c[0].val);
        for (cellit i = c.begin()+1; i != c.end(); ++i)
        {
            HANDLE_EXCEPTION((*i))
            RAISE_IF(i->type != Number, "Not a number : " << i->val)
            if (n <= internal::str_to<long>(i->val))
                return false_sym;
        }
        return true_sym;
    }

    cell proc_great_equal(const cells& c)
    {
        RAISE_IF(c.size() < 2, "'ge' needs at least two arguments")
        HANDLE_EXCEPTION(c[0])
        long n = internal::str_to<long>(c[0].val);
        for (cellit i = c.begin()+1; i != c.end(); ++i)
        {
            HANDLE_EXCEPTION((*i))
            RAISE_IF(i->type != Number, "Not a number : " << i->val)
            if (n < internal::str_to<long>(i->val))
                return false_sym;
        }
        return true_sym;
    }

    cell proc_less(const cells& c)
    {
        RAISE_IF(c.size() < 2, "'lt' needs at least two arguments")
        HANDLE_EXCEPTION(c[0])
        long n = internal::str_to<long>(c[0].val);
        for (cellit i = c.begin()+1; i != c.end(); ++i)
        {
            HANDLE_EXCEPTION((*i))
            RAISE_IF(i->type != Number, "Not a number : " << i->val)
            if (n >= internal::str_to<long>(i->val))
                return false_sym;
        }
        return true_sym;
    }

    cell proc_less_equal(const cells& c)
    {
        RAISE_IF(c.size() < 2, "'le' needs at least two arguments")
        HANDLE_EXCEPTION(c[0])
        long n = internal::str_to<long>(c[0].val);
        for (cellit i = c.begin()+1; i != c.end(); ++i)
        {
            HANDLE_EXCEPTION((*i))
            RAISE_IF(i->type != Number, "Not a number : " << i->val)
            if (n > internal::str_to<long>(i->val))
                return false_sym;
        }
        return true_sym;
    }

    cell proc_eq(const cells& c)
    {
        RAISE_IF(c.size() < 2, "'eq' needs at least two arguments")
        HANDLE_EXCEPTION(c[0])
        long n = internal::str_to<long>(c[0].val);
        for (cellit i = c.begin()+1; i != c.end(); ++i)
        {
            HANDLE_EXCEPTION((*i))
            RAISE_IF(i->type != Number, "Not a number : " << i->val)
            long b = internal::str_to<long>(i->val);
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
            return cell(Number, internal::str(c[0].val.size()));
        return cell(Number, internal::str(c[0].list.size()));
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
            long n = internal::str_to<long>(c[0].val);
            cell tmp = c[1];
            const cell temp = tmp.get_in(n);

            COPY(temp, result)

            return result;
        }
        else if (c[1].type == Dict)
        {
            cell tmp = c[1];
            RAISE_IF(c[0].type != String, "Keys in dict are of type string, not of type " << internal::convert_htbtype(c[0].type))
            const cell temp = tmp.get_in(c[0].val);

            COPY(temp, result)

            return result;
        }
        else if (c[1].type == String)
        {
            long n = internal::str_to<long>(c[0].val);
            RAISE_IF(n >= long(c[1].val.size()), "'nth' can not get a character at pos " << n << " because it is outside the string")
            return cell(String, std::string(1, c[1].val[n]));
        }
        // we should not be there
        RAISE("The object should be of type list, dict or string, not of type " << internal::convert_htbtype(c[1].type))
    }

    cell proc_dict(const cells& c)
    {
        cell result(Dict);

        for (unsigned int i = 0; i < c.size(); ++i)
        {
            HANDLE_EXCEPTION(c[i])
            COPY(c[i], temp)

            RAISE_IF(temp.type != List, "Arguments of 'dict' should be of type list, not of type " << internal::convert_htbtype(temp.type))
            HANDLE_EXCEPTION(temp.list[0])
            RAISE_IF(temp.list[0].type != String, "Keys for 'dict' should only be of type string, not of type " << internal::convert_htbtype(temp.list[0].type))
            std::string key(temp.list[0].val);
            RAISE_IF(temp.list.size() > 2,"Lists to define (key value) in dict should not be of size " << temp.list.size())  // we have more than 2 elements, not normal
            COPY(temp.list[1], v)
            result.dict[key] = v;
        }

        return result;
    }

    cell proc_keys(const cells& c)
    {
        RAISE_IF(c.size() != 1, "'keys' needs only one argument : an object of type dict")
        HANDLE_EXCEPTION(c[0])
        RAISE_IF(c[0].type != Dict, "'keys' argument's should be of type dict, not of type " << internal::convert_htbtype(c[0].type))
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
        RAISE_IF(c[0].type != Dict, "'values' argument's should be of type dict, not of type " << internal::convert_htbtype(c[0].type))
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
        RAISE_IF(c[0].type != String, "'prin1' argument's should be of type string, not of type " << internal::convert_htbtype(c[0].type))
        std::cout << c[0].val;
        return nil;
    }

    cell proc_input(const cells& c)
    {
        RAISE_IF(c.size() > 1, "'input' needs one or zero argument")
        if (c.size() == 1)
        {
            HANDLE_EXCEPTION(c[0])
            std::cout << c[0].val;
        }
        std::string o;
        std::cin >> o;
        return cell(String, o);
    }

    cell proc_getc(const cells& c)
    {
        RAISE_IF(c.size() > 0, "'getc' needs no arguments")
        char _c;
        std::cin.get(_c);
        return cell(String, std::string(1, _c));
    }

    cell proc_system(const cells& c)
    {
        RAISE_IF(c.size() != 1, "'system' needs only one argument")
        HANDLE_EXCEPTION(c[0])
        RAISE_IF(c[0].type != String, "'system' argument's should of type string, not of type " << internal::convert_htbtype(c[0].type))

        return cell(Number, internal::str(system(c[0].val.c_str())));
    }

    cell proc_str_eq(const cells& c)
    {
        RAISE_IF(c.size() < 2, "'str-eq' needs at least two arguments")
        RAISE_IF(c[0].type != String, "'str-eq' arguments' should be of type string, not of type " << internal::convert_htbtype(c[0].type))
        std::string f = c[0].val;
        for (unsigned int i = 1; i < c.size(); ++i)
        {
            HANDLE_EXCEPTION(c[i])
            RAISE_IF(c[i].type != String, "'str-eq' arguments' should be of type string, not of type " << internal::convert_htbtype(c[i].type))
            if (c[i].val != f)
                return false_sym;
        }
        return true_sym;
    }

    cell proc_str_cat(const cells& c)
    {
        RAISE_IF(c.size() < 2, "'str-cat' needs at least two arguments")
        HANDLE_EXCEPTION(c[0])
        RAISE_IF(c[0].type != String, "'str-cat' arguments' should be of type string, not of type " << internal::convert_htbtype(c[0].type))
        std::string f = c[0].val;
        for (unsigned int i = 1; i < c.size(); ++i)
        {
            HANDLE_EXCEPTION(c[i])
            RAISE_IF(c[i].type != String, "'str-cat' arguments' should be of type string, not of type " << internal::convert_htbtype(c[i].type))
            f += c[i].val;
        }
        return cell(String, f);
    }

    cell proc_str_reverse(const cells& c)
    {
        RAISE_IF(c.size() != 1, "'str-reverse' needs only one argument")
        HANDLE_EXCEPTION(c[0])
        RAISE_IF(c[0].type != String, "'str-reverse' argument's should be of type string, not of type " << internal::convert_htbtype(c[0].type))
        std::string s = c[0].val;
        std::reverse(s.begin(), s.end());
        return cell(String, s);
    }

    cell proc_str_format(const cells& c)
    {
        // see https://github.com/ryjen/format for the syntax of the format engine
        RAISE_IF(c.size() < 2, "'str-format' needs at least 2 arguments")
        HANDLE_EXCEPTION(c[0])
        RAISE_IF(c[0].type != String, "'str-format' first argument should be of type string, not of type " << internal::convert_htbtype(c[0].type))

        try
        {
            rj::format f(c[0].val);
            for (unsigned int i = 1; i < c.size(); ++i)
            {
                HANDLE_EXCEPTION(c[i])
                RAISE_IF(c[i].type != String && c[i].type != Number,
                         "'str-format' arguments' should be of type string, not of type " << internal::convert_htbtype(c[i].type))
                if (c[i].type == Number)
                    f.args(internal::str_to<long>(c[i].val));
                else
                    f.args(c[i].val);
            }
            std::string msg = f;
            return cell(String, msg);
        }
        catch (const std::invalid_argument& e)
        {
            RAISE(e.what())
        }

        // we should never be there, because we either return something in the try or in the catch
        return nil;
    }

    cell proc_typeof(const cells& c)
    {
        RAISE_IF(c.size() != 1, "'typeof' needs only one argument")
        return cell(String, internal::convert_htbtype(c[0].type));
    }

    cell proc_random(const cells& c)
    {
        RAISE_IF(c.size() > 2, "'random' needs between zero and two arguments")
        long n;
        std::mt19937 gen;
        if (c.size() == 0)
        {
            std::uniform_real_distribution<> d(0, 1);
            n = d(gen);
        }
        else if (c.size() == 1)
        {
            HANDLE_EXCEPTION(c[0])
            RAISE_IF(c[0].type != Number, "'random' arguments' should be of type number, not of type " << internal::convert_htbtype(c[0].type))
            std::uniform_int_distribution<> d(0, internal::str_to<long>(c[0].val));
            n = d(gen);
        }
        else
        {
            HANDLE_EXCEPTION(c[0])
            RAISE_IF(c[0].type != Number, "'random' arguments' should be of type number, not of type " << internal::convert_htbtype(c[0].type))
            HANDLE_EXCEPTION(c[1])
            RAISE_IF(c[1].type != Number, "'random' arguments' should be of type number, not of type " << internal::convert_htbtype(c[1].type))
            std::uniform_int_distribution<> d(internal::str_to<long>(c[0].val), internal::str_to<long>(c[1].val));
            n = d(gen);
        }
        return cell(Number, internal::str(n));
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
        builtin["%"] = cell(&proc_modulo);
        /* conditionals operators */
        builtin[">"]  = cell(&proc_greater);
        builtin["<"] = cell(&proc_less);
        builtin["<="] = cell(&proc_less_equal);
        builtin[">="] = cell(&proc_great_equal);
        builtin["="] = cell(&proc_eq);
        builtin["cond"] = cell(&proc_cond);
        /* dictionary */
        builtin["dict"] = cell(&proc_dict);
        builtin["keys"] = cell(&proc_keys);
        builtin["values"] = cell(&proc_values);
        /* IO */
        builtin["print"] = cell(&proc_print);
        builtin["prin1"] = cell(&proc_prin1);
        builtin["input"] = cell(&proc_input);
        builtin["getc"] = cell(&proc_getc);
        /* other */
        builtin["system"] = cell(&proc_system);
        /* strings */
        builtin["str-eq"] = cell(&proc_str_eq);
        builtin["str-cat"] = cell(&proc_str_cat);
        builtin["str-reverse"] = cell(&proc_str_reverse);
        builtin["str-format"] = cell(&proc_str_format);
        /* types */
        builtin["typeof"] = cell(&proc_typeof);
        /* random */
        builtin["random"] = cell(&proc_random);

        return builtin;
    }

} // namespace htb
