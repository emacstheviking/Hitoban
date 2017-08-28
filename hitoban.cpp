/**
* Code by Folaefolc
* A Lisp-like done just to concurrence Lisp itself (kind of crazy game for me)
* License MIT
*/
#include "hitoban.hpp"

namespace htb
{

///////////////////////////////////////////////////// constants

static bool htb_STRICT = false;
static bool htb_TRACKING = false;
const std::vector<std::regex> regexs = {
    std::regex("^\"[^\"]*\""),                         // strings
    std::regex("^:"),                                               // dict key symbol
    std::regex("^[\\(\\)]"),                                      // parenthesis
    std::regex("^((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?((e|E)((\\+|-)?)[[:digit:]]+)?"),                  // numbers
    std::regex("^[#@$':_!?\\-\\w]+"),                   // words
    std::regex("^(\\+|-|\\*|/|%|<=|>=|!=|<|>|=|\\^)"),    // operators
    std::regex("^\\s+"),                                         // whitespaces
    std::regex("^;.*")                                            // comments
};

// define functions to create and modify an Hitoban environment
void add_globals(environment& env)
{
    env["nil"] = nil;
    env["false"] = false_sym;
    env["true"] = true_sym;

    for(const auto& v : get_builtin())
        env[v.first] = v.second;
}

void set_strict(bool s)
{
    htb_STRICT = s;
}

void set_tracking(bool t)
{
    htb_TRACKING = t;
}

environment init_environment()
{
    environment env;
    add_globals(env);

    return env;
}

///////////////////////////////////////////////////// main

cell eval(cell x, environment* env)
{
    if (htb_TRACKING)
    {
        std::string nx = to_string(x);
        std::cout << log(termcolor::cyan, "x: " << nx) << " "
                        << log(termcolor::yellow, "[" << ((!env->has_outer()) ? "global": "ref on global")) << ", "
                            << log(termcolor::green, ((env->isfile) ? (std::string("is a file `") + env->fname +"`") : "not a file"))
                        << log(termcolor::yellow, "]")
                        << std::endl;
    }

    // quitting if we got an exception
    if (x.type == Exception && htb_STRICT)
        throw std::runtime_error(std::string("Encountered an exception will in strict mode\n") + to_string(x));

    if (x.type == Symbol)
        return env->find(x.val)[x.val];
    if (x.type == Number || x.type == String)
        return x;
    if (x.list.empty())
        return nil;
    if (x.list[0].val == ":")  // dict key symbol
    {
        RAISE_IF(x.list.size() < 1, "':' symbolize the beginning of a dict key, the length should be of 2, not of " << x.list.size())

        cell exps;
        exps.type = List;

        x.list[1].type = String;
        exps.list.push_back(x.list[1]);
        for (cell::iter exp = x.list.begin() + 2; exp != x.list.end(); ++exp)
            exps.list.push_back(eval(*exp, env));
        return exps;
    }

    if (x.list[0].type == Symbol)
    {
        if (x.list[0].val.substr(0, 1) == "#")  // (#key hashmap)
        {
            std::string key = x.list[0].val.substr(1);
            RAISE_IF(key.empty(), x.list[0].val << " is not a valid key (length must be > 0)")
            cell c(eval(x.list[1], env));
            if (c.type == Dict)
                return c.get_in(key);
            else if (c.type == List)
                return c.get_in(to_long(key));
            else if (c.type == String)
            {
                long n = to_long(key);
                RAISE_IF(n >= long(c.val.size()), "'#' can not get a character at pos " << n << " because it is outside the string")
                return cell(String, std::string(1, c.val[n]));
            }
            RAISE("The object should be of type dict, list or string to use the # pattern, not of type " << convert_htbtype(c.type))
        }
        if (x.list[0].val == "quote")       // (quote exp)
            return x.list[1];
        if (x.list[0].val == "if")          // (if test conseq [alt])
            return eval(eval(x.list[1], env).val == false_sym.val ? (x.list.size() < 4 ? nil : x.list[3]) : x.list[2], env);
        if (x.list[0].val == "set!")        // (set! var exp)
        {
            cell c = env->find(x.list[1].val)[x.list[1].val];
            RAISE_IF(c.const_expr, x.list[1].val << " is a const expr, can not set its value to something else")
            return env->find(x.list[1].val)[x.list[1].val] = eval(x.list[2], env);
        }
        if (x.list[0].val == "def")      // (def var exp)
            return (*env)[x.list[1].val] = eval(x.list[2], env);
        if (x.list[0].val == "const")   // (const var exp)
        {
            cell c = eval(x.list[2], env);
            c.const_expr = true;
            return (*env)[x.list[1].val] = c;
        }
        if (x.list[0].val == "lambda")   // (lambda (var*) exp)
        {
            x.type = Lambda;
            // keep a reference to the environment that exists now (when the
            // lambda is being defined) because that's the outer environment
            // we'll need to use when the lambda is executed
            x.env = env;
            return x;
        }
        if (x.list[0].val == "begin")   // (begin exp*)
        {
            for (unsigned int i = 1; i < x.list.size() - 1; ++i)
            {
                cell c = eval(x.list[i], env);
                HANDLE_EXCEPTION(c)
            }
            return eval(x.list[x.list.size() - 1], env);
        }
        if (x.list[0].val == "ns")  // (ns "name" ...)
        {
            RAISE_IF(x.list.size() < 2, "'ns' needs at least one argument 'name' (string)")
            HANDLE_EXCEPTION(x.list[1])
            environment* sub = env->get_namespace(x.list[1].val);

            if (x.list.size() > 2)
                for (unsigned int i = 2; i < x.list.size(); ++i)
                {
                    cell c = eval(x.list[i], sub);
                    HANDLE_EXCEPTION(c)
                }

            return nil;
        }
        if (x.list[0].val == "require") // (require exp); exp as a list, a dict or a string
        {
            RAISE_IF(x.list.size() != 2, "require' takes only 1 argument, not " << x.list.size())
            cell c = eval(x.list[1], env);

            if (c.type == List)
            {
                for (cellit i = c.list.begin(); i != c.list.end(); i++)
                {
                    internal::_read_file(i, env);
                }
            }
            else if (c.type == String)
            {
                internal::_read_file(&c, env);
            }
            else if (c.type == Dict)
            {
                for (auto kv: c.dict)
                {
                    environment* sub = env->get_namespace(kv.first);
                    internal::_read_file(&kv.second, sub, false);
                }
            }
            else
                RAISE("require' takes a dict, a list or a single string as an argument, not a " << convert_htbtype(c.type))
            return nil;
        }
        ///////////////////////////////////////////////////// procedures that need to use an environment (not provided in htb_stdlib because it only takes cells)
        if (x.list[0].val == "list-current-ns")  // (list-current-ns file)
        {
            RAISE_IF(x.list.size() != 1, "'list-current-ns' takes only no argument")
            cell output(List);
            for (auto name : env->get_namespaces())
                output.list.push_back(cell(String, name));
            return output;
        }
        if (x.list[0].val == "get-opened-file")  // (get-opened-file)
        {
            RAISE_IF(x.list.size() != 1, "'get-opened-file' takes no argument")
            std::string fname = env->get_parent_file();
            return cell(String, fname);
        }
    }

    // (proc exp*)
    cell proc(eval(x.list[0], env));
    cells exps;
    for (cell::iter exp = x.list.begin() + 1; exp != x.list.end(); ++exp)
        exps.push_back(eval(*exp, env));
    if (proc.type == Lambda)
    {
        // Create an environment for the execution of this lambda function
        // where the outer environment is the one that existed* at the time
        // the lambda was defined and the new inner associations are the
        // parameter names with the given arguments.
        // *Although the environment existed at the time the lambda was defined
        // it wasn't necessarily complete - it may have subsequently had
        // more symbols defined in that environment.
        //                  body                                           params              args
        return eval(proc.list[2], new environment(proc.list[1].list, exps, proc.env));
    }
    else if (proc.type == Proc)
        return proc.proc(exps);
    // we have something like
    // (... (thing other ...))
    // and we are trying to parse "thing" alone
    if (x.type == List)
    {
        cell exp;
        for (cell::iter e = x.list.begin(); e != x.list.end(); ++e)
            exp.list.push_back(eval(*e, env));
        return exp;
    }
    RAISE("Not a function")
}

///////////////////////////////////////////////////// parse, read and user interaction

void raise_tokenizing_error(const std::string& str, const std::string& s)
{
    std::cout << "Could not tokenize correctly: " << std::endl;
    std::vector<std::string> subs = split_string(str, "\n");
    // take off the newline char in s
    std::string s2;
    s2.reserve(s.size());
    for (unsigned int j=0; j < s.size(); j++)
    {
        if (s[j] != '\n' && s[j] != '\r')
            s2 += s[j];
    }

    // print each line while it does not contain the error
    // then print a well placed "^" to indicate what could not be tokenized
    for (auto& element: subs)
    {
        std::cout << element << std::endl;

        std::string::size_type p = element.find(s2);
        if (p != std::string::npos)
        {
            // we found the line where the error is at
            for (std::string::size_type i=0; i < p; i++) std::cout << " ";
            std::cout << "^" << std::endl;
            break;
        }
    }
    throw std::runtime_error("Tokenizing failed");
}

// convert given string to list of tokens
std::list<std::string> tokenize(const std::string& str)
{
    std::string s = str;
    std::list<std::string> tokens;

    while (!s.empty())
    {
        bool ok = false;

        for (const auto& r : regexs)
        {
            std::smatch m;
            if (std::regex_search(s, m, r))
            {
                std::string m0(m[0]);
                if (std::string::npos != m0.find_first_not_of(" \t\v\r\n") && m0.substr(0, 1) != ";")
                    tokens.push_back(m[0]);
                s = std::regex_replace(s, r, std::string(""), std::regex_constants::format_first_only);
                ok = true;
                break;
            }
        }
        if (!ok)
        {
            raise_tokenizing_error(str, s);
            break;
        }
    }

    return tokens;
}

// numbers become Numbers; every other token is a Symbol
cell atom(const std::string& token)
{
    if (isdig(token[0]) || (token[0] == '-' && isdig(token[1])))
        return cell(Number, token);
    if (token.substr(0, 1) == "\"" || token.substr(0, 1) == "'")
        return cell(String, token.substr(1, token.size() - 2));
    return cell(Symbol, token);
}

// return the Hitoban expression in the given tokens
cell read_from(std::list<std::string>& tokens)
{
    const std::string token(tokens.front());
    tokens.pop_front();
    if (token == "(")
    {
        cell c(List);
        while (tokens.front() != ")")
            c.list.push_back(read_from(tokens));
        tokens.pop_front();
        return c;
    }
    else
        return atom(token);
}

// return the Hitoban expression represented by the given string
cell read(const std::string& s)
{
    std::list<std::string> tokens(tokenize(s));
    return read_from(tokens);
}

// execute a string of Hitoban code, in a given environment
cell run_string(const std::string& code, environment* env)
{
    cell result = eval(read(code), env);
    return result;
}

} // namespace htb

int start_repl()
{
    htb::print_shell_headers();
    std::cout << "Type \"help\" for more information."
                    << std::endl;

    htb::environment global_env;
    htb::add_globals(global_env);
    htb::repl("> ", &global_env);

    return EXITSUCCESS;
}

int main(int argc, char *argv[])
{
    if (argc < 2)  // we just started the program as a normal one, launch the read-eval-print-loop
        start_repl();
    else if (argc >= 2)
    {
        int c = 1;
        std::string input = argv[c];

        if (input == "-h")  // help
        {
            std::cout << "Help message" << std::endl
                            << "Usage: " << argv[0] << " [option] ... [file | tests] [args...]" << std::endl
                            << "Options and arguments:" << std::endl
                            << "-h      : print this help message and exit" << std::endl
                            << "-w      : stop execution if an exception is detected" << std::endl
                            << "-t      : print all the details of the execution" << std::endl
                            << "-v      : print the Hitoban version number and exit" << std::endl
                            << "file    : program read from script file" << std::endl
                            << "tests   : launch all the tests and print how many passed, then exit" << std::endl
                            << "args... : arguments passed to program in ARGS" << std::endl
                            ;
            return EXITSUCCESS;
        }
        if (input == "-w")  // strict mode
        {
            htb::set_strict(true);

            if (argc >= c + 2) input = argv[++c];
            else
            {
                start_repl();  // we do not have other arguments, start a repl
                return EXITSUCCESS;
            }
        }
        if (input == "-t")  // tracking mode
        {
            htb::set_tracking(true);

            if (argc >= c + 2) input = argv[++c];
            else
            {
                start_repl();  // we do not have other arguments, start a repl
                return EXITSUCCESS;
            }
        }
        if (input == "-v")  // version
        {
            htb::print_shell_headers();
            return EXITSUCCESS;
        }
        if (input == "tests")  // running the tests
            return htb::tests();

        // if we are here, we have a filename passed as an argument
        if (htb::check_if_file_exists(input))
        {
            std::string content = htb::read_file(input);

            // setting up environment
            htb::environment global_env;
            htb::add_globals(global_env);
            // we must save that we are in a file !
            global_env.isfile = true;
            global_env.fname = input;

            // checking for arguments
            if (argc >= c + 2)
            {
                // we have arguments for the program, let's put the in a const variable named ARGS
                std::string args = "(const ARGS (list ";
                for (int i=c + 1; i < argc; i++)
                {
                    std::string cn(argv[i]);
                    args += std::string(cn) + " ";
                }
                args += "))";

                // push back the arguments in the environment by interpreting them
                htb::run_string(args, &global_env);
            }

            // running the code
            std::cout << to_string(htb::run_string(content, &global_env)) << std::endl;
        }
        else
        {
            // we can not find the file, just exit
            std::cout << argv[0] << ": can not open file '" << input << "'" << std::endl;
            return EXITFAILURE;
        }
    }

    return EXITSUCCESS;
}
