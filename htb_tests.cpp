/*
* Code by Folaefolc
* A Lisp-like done just to concurrence Lisp itself (kind of crazy game for me)
* Interpreted programming language, C++14 ; main purpose is for video games
* License MIT
*/

#include "hitoban.hpp"

namespace htb
{

    namespace tests
    {

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
            TEST("(nth 4 \"hello\")", "\"o\"");
            TEST("(#1 \"hello\")", "\"e\"");

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
            TEST("(fact 12)", "479001600");  // no bignums; this is as far as we go with 32 bits
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
            TEST("(def count-to (lambda (x y) (lambda () (begin (const _x x) (if (= x y) nil (if (= x _x) (begin (set! x (+ x 1)) (- x 1)) (set! x (+ x 1))))))))", "<Lambda>");
            TEST("(def truuc (count-to 5 7))", "<Lambda>");
            TEST("(truuc)", "5");
            TEST("(truuc)", "6");
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
            TEST("(isdef hidden)", "false");

            // dicts tests
            TEST("(def dico (dict (list \"hello\" 1) (list \"other\" (list 4 5 6))))", "(:hello 1 :other (4 5 6))");
            TEST("(nth \"hello\" dico)", "1");
            TEST("(#other dico)", "(4 5 6)");
            TEST("(#hello dico)", "1");
            TEST("(keys dico)", "(\"hello\" \"other\")");
            TEST("(values dico)", "(1 (4 5 6))");
            TEST("(def testdct (dict (:name \"truc\") (:machin (list 1 2 3 4))))", "(:machin (1 2 3 4) :name \"truc\")");
            TEST("(nth \"name\" testdct)", "\"truc\"");
            TEST("(nth \"machin\" testdct)", "(1 2 3 4)");

            // require tests (+ ns tests)
            TEST("(require (list \"tests/simple.htb\"))", "nil");
            TEST("(ns \"simple.htb\" (ns \"truc\" (print hello bid c)))", "nil");
            TEST("(ns \"test\" (def ns_test_a 5))", "nil");
            TEST("(print ns_test_a)", "<Exception> Unbound symbol 'ns_test_a'");
            TEST("(ns \"test\" (print ns_test_a))", "nil");
            TEST("(require (dict (:sub \"tests/smth.htb\")))", "nil");
            TEST("(ns \"sub\" (print bid2 c2))", "nil");
            TEST("(list-current-ns)", "(\"simple.htb\" \"sub\" \"test\")");

            // other functions included by default
            //TEST("(md5 \"hello\")", "\"5d41402abc4b2a76b9719d911017c592\"");  // need to implement it in the standard lib
            TEST("(system \"echo\")", "0");
            TEST("(typeof (random))", "\"Number\"");

            // types
            TEST("(typeof 1)", "\"Number\"");

            // strings
            TEST("(str-reverse \"hello\")", "\"olleh\"");
            TEST("(str-cat \"hello \" \"world\")", "\"hello world\"");
            TEST("(str-eq \"hello\" \"hello\")", "true");
            TEST("(str-eq \"hello\" \"Æ\")", "false");
            TEST("(str-format \"{0} walked up {1} miles and saw {2} and {0}\" \"a bear\" 20 \"an eagle\")", "<Exception> specifier index not ordered");
            TEST("(str-format \"{0} walked up {1} miles and saw {2}\" \"a bear\" 20 \"an eagle\")", "\"a bear walked up 20 miles and saw an eagle\"");

            // number of arguments verification
            TEST("(abs)", "<Exception> Too few arguments, got 0 expected 1");
            TEST("(abs 1 2)", "<Exception> Too much arguments, got 2 expected 1");
            TEST("(keys)", "<Exception> 'keys' needs 1 argument(s) not 0");
            TEST("(cond)", "<Exception> 'cond' needs at least 1 argument not 0");
            TEST("(<= 1)", "<Exception> '<=' needs at least 2 arguments not 1");
            TEST("(input 1 2 3)", "<Exception> 'input' needs 0 to 1 argument, not 3");
            TEST("(random 4 5 6 7)", "<Exception> 'random' needs 0 to 2 arguments, not 4");

            std::cout << std::endl
                            << "========================================" << std::endl
                            << "Total tests " << g_test_count
                            << ", total failure(s) " << g_fault_count
                            << std::endl;

            return g_fault_count ? EXITFAILURE : EXITSUCCESS;
        }

    }  // namespace tests

}  // namespace htb
