/**
* Code by Folaefolc
* A Lisp-like done just to concurrence Lisp itself (kind of crazy game for me)
* License MIT
*/
#include "hitoban.hpp"

#define READ_FILE(name, env) HANDLE_EXCEPTION(name) \
        RAISE_IF(name.type != String, "'require' needs strings, not " << convert_htbtype(name.type)) \
        LOAD_FILE(name.val) \
        RAISE_IF(content == FILE_NOT_FOUND, "Can not find the required file '" << name.val << "'") \
        run_string(content, env);

namespace htb
{

static bool strict = false;
static bool tracking = false;
const std::vector<std::regex> regexs = {
    std::regex("^['\"][^'\"]+['\"]"),                         // strings
    std::regex("^:"),                                               // dict key symbol
    std::regex("^[\\(\\)]"),                                      // parenthesis
    std::regex("^((\\+|-)?[[:digit:]]+)(\\.(([[:digit:]]+)?))?((e|E)((\\+|-)?)[[:digit:]]+)?"),                  // numbers
    std::regex("^[#@$':_!?\\-\\w]+"),                   // words
    std::regex("^(\\+|-|\\*|/|%|<=|>=|!=|<|>|=)"),    // operators
    std::regex("^\\s+"),                                         // whitespaces
    std::regex("^;.*")                                            // comments
};

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

// define the bare minimum set of primitives necessary to pass the unit tests
void add_globals(environment& env)
{
    env["nil"] = nil;
    env["false"] = false_sym;
    env["true"] = true_sym;
    env["print"] = cell(&proc_print);

    for(const auto& v : get_builtin())
        env[v.first] = v.second;
}

void set_strict(bool s)
{
    strict = s;
}

void set_tracking(bool t)
{
    tracking = t;
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
    if (tracking)
    {
        std::cout << "x: " << to_string(x) << " "
                        << "[" << ((!env->has_outer()) ? "global" : "ref on global") << "]"
                        << std::endl;
    }

    // quitting if we got an exception
    if (x.type == Exception && strict)
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
            else
                return c.get_in(to_long(key));
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
                eval(x.list[i], env);
            return eval(x.list[x.list.size() - 1], env);
        }
        if (x.list[0].val == "ns")  // (ns "name" ...)
        {
            RAISE_IF(x.list.size() < 2, "'ns' needs at least one argument 'name' (string)")
            HANDLE_EXCEPTION(x.list[1])
            environment* sub = env->get_namespace(x.list[1].val);

            if (x.list.size() > 2)
                for (unsigned int i = 2; i < x.list.size(); ++i)
                    eval(x.list[i], sub);

            return nil;
        }
        if (x.list[0].val == "i-ns")  // (i-ns "name" ...)
        {
            RAISE_IF(x.list.size() < 2, "'i-ns' needs at least one argument 'name' (string)")
            HANDLE_EXCEPTION(x.list[1])
            // we set is_ins to true
            environment* sub = env->get_namespace(x.list[1].val, true);
            add_globals(*sub);

            if (x.list.size() > 2)
                for (unsigned int i = 2; i < x.list.size(); ++i)
                    eval(x.list[i], sub);

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
                    READ_FILE((*i), env)
                }
            }
            else if (c.type == String)
            {
                READ_FILE(c, env)
            }
            else if (c.type == Dict)
            {
                for (auto kv: c.dict)
                {
                    environment* sub = env->get_namespace(kv.first);
                    READ_FILE(kv.second, sub)
                }
            }
            else
            {
                std::stringstream ss; ss << "require' takes a dict, a list or a single string as an argument, not a " << convert_htbtype(c.type) << std::endl;
                return cell(Exception, ss.str());
            }
            return nil;
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
    return cell(Exception, "Not a function");
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
                if (!contains_only(m[0], ' ') && m0.substr(0, 1) != ";")
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

// convert given cell to a Hitoban-readable string
std::string to_string(const cell& exp, bool from_htb)
{
    if (exp.type == List)
    {
        std::string s("(");
        for (cell::iter e = exp.list.begin(); e != exp.list.end(); ++e)
            s += to_string(*e) + ' ';
        if (s[s.size() - 1] == ' ')
            s.erase(s.size() - 1);
        return s + ')';
    }
    else if (exp.type == Lambda)
        return "<Lambda>";
    else if (exp.type == Proc)
        return "<Proc>";
    else if (exp.type == Exception)
        return "<Exception> " + exp.val;
    else if (exp.type == Dict && !from_htb)
        return "<Dict>";
    else if (exp.type == Dict && from_htb)
        return "{}";
    else if (exp.type == String && !from_htb)
        return "\"" + exp.val + "\"";
    else if (exp.type == String && from_htb)
        return exp.val;
    return exp.val;
}

cell run_string(const std::string& code, environment* env)
{
    cell result = eval(read(code), env);
    return result;
}

// the default read-eval-print-loop
void repl(const std::string& prompt, environment* env)
{
    for (;;)
    {
        std::cout << prompt;
        std::string line;
        std::getline(std::cin, line);
        if (line == "help")
        {
            std::cout << "Type \"quit\" to quit the repl"
                            << std::endl;
        }
        else if (line == "quit")
        {
            std::cout << "Bye !" << std::endl;
            break;
        }
        else
            std::cout << to_string(run_string(line, env)) << std::endl;
    }
}

void print_shell_headers()
{
    std::cout << "Hitoban " << VER_FULLVERSION_STRING
                    << " (last build on " << VER_DATE << "/" << VER_MONTH << "/" << VER_YEAR << ")"
                    << " [status " << VER_STATUS << "]"
                    << std::endl;
}

int tests()
{
    environment global_env;
    add_globals(global_env);
    print_shell_headers();
    // basics tests
    TEST("(+ 1 2 3 4 5 6 7 8 9)", "45");
    TEST("(print \"hello\" \"world\")", "nil");
    TEST("(quote (testing 1 (2.0) -3.14e159))", "(testing 1 (2.0) -3.14e159)");
    TEST("(quote \"hello  world\")", "\"hello  world\"");
    TEST("(const constante 5)", "5");
    TEST("(quote ()) ; quote ()", "()");
    TEST("(set! constante 4)", "<Exception> constante is a const expr, can not set its value to something else");
    TEST("(+ 2 2)", "4");
    TEST("(+ (* 2 100) (* 1 10))", "210");
    // arrays tests
    TEST("(def array (list 4 5 6 7 8))", "(4 5 6 7 8)");
    TEST("(def floating 5.5)", "5.5");
    TEST("(#2 array)", "6");
    TEST("(nth 2 array)", "6");
    // conditions tests
    TEST("(if (> 6 5) (+ 1 1) (+ 2 2))", "2");
    TEST("(if (< 6 5) (+ 1 1) (+ 2 2))", "4");
    TEST("(def x 3)", "3");
    TEST("x", "3");
    TEST("(+ x x)", "6");
    TEST("(begin (def x 1) (set! x (+ x 1)) (+ x 1))", "3");
    TEST("(cond ((< 1 0) false) (nil true))", "true");
    TEST("(cond ((= 1 0) false))", "nil");
    TEST("(cond ((= 0 1) 0) ((= 1 1) 1) ((= 2 2) 2))", "1");
    // lambdas tests
    TEST("((lambda (x) (+ x x)) 5)", "10");
    TEST("(def twice (lambda (x) (* 2 x)))", "<Lambda>");
    TEST("(twice 5)", "10");
    TEST("(def compose (lambda (f g) (lambda (x) (f (g x)))))", "<Lambda>");
    TEST("((compose list twice) 5)", "(10)");
    TEST("(def repeat (lambda (f) (compose f f)))", "<Lambda>");
    TEST("((repeat twice) 5)", "20");
    TEST("((repeat (repeat twice)) 5)", "80");
    TEST("(def fact (lambda (n) (if (<= n 1) 1 (* n (fact (- n 1))))))", "<Lambda>");
    TEST("(fact 3)", "6");
    //TEST("(fact 50)", "30414093201713378043612608166064768844377641568960512000000000000");
    TEST("(fact 12)", "479001600"); // no bignums; this is as far as we go with 32 bits
    TEST("(def abs (lambda (n) ((if (> n 0) + -) 0 n)))", "<Lambda>");
    TEST("(list (abs -3) (abs 0) (abs 3))", "(3 0 3)");
    TEST("(def combine (lambda (f)"
             "(lambda (x y)"
                "(if (null? x) (quote ())"
                "(f (list (car x) (car y))"
                "((combine f) (cdr x) (cdr y)))))))", "<Lambda>");
    TEST("(def zip (combine cons))", "<Lambda>");
    TEST("(zip (list 1 2 3 4) (list 5 6 7 8))", "((1 5) (2 6) (3 7) (4 8))");
    TEST("(def riff-shuffle (lambda (deck) (begin"
            "(def take (lambda (n seq) (if (<= n 0) (quote ()) (cons (car seq) (take (- n 1) (cdr seq))))))"
            "(def drop (lambda (n seq) (if (<= n 0) seq (drop (- n 1) (cdr seq)))))"
            "(def mid (lambda (seq) (/ (length seq) 2)))"
             "((combine append) (take (mid deck) deck) (drop (mid deck) deck)))))", "<Lambda>");
    TEST("(riff-shuffle (list 1 2 3 4 5 6 7 8))", "(1 5 2 6 3 7 4 8)");
    TEST("((repeat riff-shuffle) (list 1 2 3 4 5 6 7 8))",  "(1 3 5 7 2 4 6 8)");
    TEST("(riff-shuffle (riff-shuffle (riff-shuffle (list 1 2 3 4 5 6 7 8))))", "(1 2 3 4 5 6 7 8)");
    TEST("(def range (lambda (a b) (if (= a b) (quote ()) (cons a (range (+ a 1) b)))))", "<Lambda>");
    TEST("(range 0 10)", "(0 1 2 3 4 5 6 7 8 9)");
    TEST("(def count-down-from (lambda (n) (lambda () (set! n (- n 1)))))", "<Lambda>");
    TEST("(def cdf4 (count-down-from 4))", "<Lambda>");
    TEST("(cdf4)", "3");
    TEST("(cdf4)", "2");
    TEST("(cdf4)", "1");
    TEST("(cdf4)", "0");
    TEST("(def count-to "
         "(lambda (x y) "
            "(lambda () "
                "(if (= x y) nil (set! x (+ x 1))))))", "<Lambda>");
    TEST("(def truuc (count-to 5 7))", "<Lambda>");
    TEST("(truuc)", "6");
    TEST("(truuc)", "7");
    TEST("(truuc)", "nil");
    // closures tests
    TEST("(def set-hidden 0)", "0");
    TEST("(def get-hidden 0)", "0");
    TEST("((lambda ()"
        "(begin"
            "(def hidden 0)"
            "(set! set-hidden (lambda (n) (set! hidden n)))"
            "(set! get-hidden (lambda () hidden)))))", "<Lambda>");
    TEST("(get-hidden)", "0");
    TEST("(set-hidden 1234)", "1234");
    TEST("(get-hidden)", "1234");
    // dicts tests
    TEST("(def dico (dict (list \"hello\" 1) (list \"other\" (list 4 5 6))))", "<Dict>");
    TEST("(nth \"hello\" dico)", "1");
    TEST("(#other dico)", "(4 5 6)");
    TEST("(#hello dico)", "1");
    TEST("(keys dico)", "(\"hello\" \"other\")");
    TEST("(values dico)", "(1 (4 5 6))");
    TEST("(def testdct (dict (:name \"truc\") (:machin (list 1 2 3 4))))", "<Dict>");
    TEST("(nth \"name\" testdct)", "\"truc\"");
    TEST("(nth \"machin\" testdct)", "(1 2 3 4)");
    // require tests (+ ns tests)
    TEST("(require (list \"tests/simple.htb\"))", "nil");
    TEST("(ns \"\truc\" (print hello bid c))", "nil");
    TEST("(ns \"test\" (def ns_test_a 5))", "nil");
    TEST("(print ns_test_a)", "<Exception> Unbound symbol 'ns_test_a'");
    TEST("(ns \"test\" (print ns_test_a))", "nil");
    TEST("(require (dict (:sub \"tests/simple.htb\")))", "nil");
    TEST("(ns \"sub\" (print bid c))", "nil");

    std::cout
        << "total tests " << g_test_count
        << ", total failures " << g_fault_count
        << std::endl;
    return g_fault_count ? EXITFAILURE : EXITSUCCESS;
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
            std::cout << "Hitoban " << htb::VER_FULLVERSION_STRING << std::endl;
            return EXITSUCCESS;
        }
        if (input == "tests")  // running the tests
            return htb::tests();

        // if we are here, we have a filename passed as an argument
        if (htb::check_if_file_exists(input))
        {
            std::string content = htb::read_file(input);

            htb::environment global_env;
            htb::add_globals(global_env);

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
            htb::run_string(content, &global_env);
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
