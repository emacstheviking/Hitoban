#pragma once

#include "htb_types.hpp"
#include "htb_includes.hpp"
#include "htb_functions.hpp"

namespace htb
{

///////////////////////////////////////////////////// cell

struct environment; // forward declaration; cell and environment reference each other

// a variant that can hold any kind of lisp value
struct cell
{
    typedef cell (*proc_type)(const std::vector<cell>&);
    typedef std::vector<cell>::const_iterator iter;
    typedef std::map<std::string, cell> map;

    cell_type type;
    std::string val;
    std::vector<cell> list;
    map dict;
    proc_type proc;
    environment* env;
    bool const_expr;

    cell(cell_type type=Symbol) :
        type(type)
        , env(0)
        , const_expr(false)
    {}
    cell(cell_type type, const std::string& val) :
        type(type)
        , val(val)
        , env(0)
        , const_expr(false)
    {}
    cell(proc_type proc) :
        type(Proc)
        , proc(proc)
        , env(0)
        , const_expr(false)
    {}

    cell get_in(const std::string& key)
    {
        if (type == Dict)
        {
            if (!dict.empty())
            {
                if (dict.find(key) != dict.end())
                    return dict[key];
                std::stringstream ss; ss << "Can not find the key " << key << " in the dict" << std::endl;
                return cell(Exception, ss.str());
            }
            else
            {
                std::stringstream ss; ss << "Can not access an element with the key " << key << " because the dict is empty" << std::endl;
                return cell(Exception, ss.str());
            }
        }
        else
        {
            std::stringstream ss; ss << "Can not access a sub element because the object is not a dict" << std::endl;
            return cell(Exception, ss.str());
        }
    }
    cell get_in(long n)
    {
        if (type == List)
        {
            if (n < long(list.size()))
                return list[n];
            else
            {
                std::stringstream ss; ss << "Can not find the  " << n << "th element in the list" << std::endl;
                return cell(Exception, ss.str());
            }
        }
        else
        {
            std::stringstream ss; ss << "Can not access a sub element because the object is not a list" << std::endl;
            return cell(Exception, ss.str());
        }
    }
};

typedef std::vector<cell> cells;
typedef cells::const_iterator cellit;

const cell false_sym(Symbol, "false");
const cell true_sym(Symbol, "true"); // anything that isn't false_sym is true
const cell nil(Symbol, "nil");

/* Experimental
inline bool compare_cells(const cell& a, const cell& b)
{
    if (a.list.empty() && b.list.empty() && a.dict.empty() && b.dict.empty())
        return (a.type == b.type && a.val == b.val && a.proc == b.proc && a.const_expr == b.const_expr);
    else if (!a.list.empty() && !b.list.empty() && a.dict.empty() && b.dict.empty())
    {
        bool r = (a.type == b.type && a.val == b.val && a.proc == b.proc && a.const_expr == b.const_expr && a.list.size() == b.list.size());
        if (!r)
            return false;
        for (unsigned int i=0; i < a.list.size(); ++i)
        {
            if (!compare_cells(a.list[i], b.list[i]))
                return false;
        }
        return true;
    }
    else if (a.list.empty() && b.list.empty() && !a.dict.empty() && !b.dict.empty())
    {
        bool r = (a.type == b.type && a.val == b.val && a.proc == b.proc && a.const_expr == b.const_expr && a.dict.size() == b.dict.size());
        if (!r)
            return false;
        for (auto kv: a.dict)
        {
            if (b.dict.find(kv.first) == a.dict.end())
                return false;
            if (!compare_cells(kv.second, b.dict.find(kv.first)->second))
                return false;
        }
        return true;
    }

    return false;
}
*/

///////////////////////////////////////////////////// environment

// a dictionary that (a) associates symbols with cells, and
// (b) can chain to an "outer" dictionary
struct environment {
    environment(environment* outer=0, bool isolated=false) :
        outer_(outer)
        , isolated_(isolated)
    {}

    environment(const cells& parms, const cells& args, environment* outer) :
        outer_(outer)
        , isolated_(false)
    {
        cellit a = args.begin();
        for (cellit p = parms.begin(); p != parms.end(); ++p)
        {
            env_[p->val] = *a++;
        }
    }

    // map a variable name onto a cell
    typedef std::map<std::string, cell> map;

    // return a reference to the innermost environment where 'var' appears
    map& find(const std::string& var)
    {
        if (env_.find(var) != env_.end())
            return env_; // the symbol exists in this environment
        if (outer_)
            return outer_->find(var); // attempt to find the symbol in some "outer" env

        std::stringstream ss; ss << "Unbound symbol '" << var << "'";
        errors[var] = cell(Exception, ss.str());
        return errors;
    }

    // return a reference to the cell associated with the given symbol 'var'
    cell& operator[] (const std::string & var)
    {
        return env_[var];
    }

    // return true is the environment is isolated
    bool is_isolated()
    {
        return isolated_;
    }

    // get a namespace or create one
    environment* get_namespace(const std::string& name, bool is_ins=false)
    {
        if (namespaces.find(name) == namespaces.end())
        {
            if (!is_ins)
                namespaces[name] = new environment(this);
            else
                namespaces[name] = new environment(0, true);
        }
        /*std::string msg("You are trying to create a");
        if (is_ins) msg += "n isolated";
        else msg += " non-isolated";
        msg += " namespace that already exists in the opposite state";
        RAISE_IF(namespaces[name]->is_isolated() != is_ins, msg)*/

        return namespaces[name];
    }

private:
    map env_; // inner symbol->cell mapping
    map errors;
    std::map<std::string, environment*> namespaces;
    environment* outer_; // next adjacent outer env, or 0 if there are no further environments
    bool isolated_;
};

} // namespace htb
