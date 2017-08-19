#include "hitoban.hpp"

///////////////////////////////////////////////////// shell

namespace htb
{

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

}  // namespace htb

