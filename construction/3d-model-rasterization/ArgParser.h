#include <memory>
#include <string>
#include <vector>
#include <iostream>

enum class Required
{
    TRUE,
    FALSE
};

enum class Type
{
    UNDEFINED = 0,
    STRING,
    INTEGER,
    BOOL_FLAG
};

class ArgumentParser
{
public:
    template <Type ARG_TYPE, typename STORAGE>
    void addArgument(const std::string &long_arg, const std::string &short_arg, const std::string &help, STORAGE * dest, Required required = Required::FALSE)
    {
        Argument<STORAGE> * arg = new Argument<STORAGE>(long_arg, short_arg, help, required);
        arg->type = ARG_TYPE;
        arg->value = dest;
        arguments.push_back(arg);
    }

    void printArgs(Required type)
    {
        for (auto &a : arguments)
        {
            Argument<void> *arg = reinterpret_cast<Argument<void> *>(a);
            if (arg->required == type)
            {
                std::cout << arg->short_arg << " " << arg->long_arg << std::endl;
                std::cout << "    " << arg->help << std::endl;
            }
        }
    }

    virtual void printHelp()
    {
        int required = 0;
        for (auto &a : arguments)
        {
            Argument<void> *arg = reinterpret_cast<Argument<void> *>(a);
            if (arg->required == Required::TRUE)
                required++;
        }
        int optional = (int)arguments.size() - required;
        if (required)
        {
            std::cout << "Required arguments:" << std::endl;
            printArgs(Required::TRUE);
        }
        if (optional)
        {
            std::cout << "Optional arguments:" << std::endl;
            printArgs(Required::FALSE);
        }
    }

    bool parse(int argc, char **argv)
    {
        for (int i = 0; i < argc; i++)
        {
            std::string current_arg(argv[i]);
            if (current_arg == "--help" || current_arg == "-h" || current_arg == "?")
            {
                printHelp();
                return false;
            }
            for (auto &a : arguments)
            {
                Argument<void> *arg = reinterpret_cast<Argument<void> *>(a);
                if (current_arg == arg->long_arg || current_arg == arg->short_arg)
                {
                    switch (arg->type)
                    {
                    case Type::STRING:
                    {
                        i++;
                        auto string_arg = reinterpret_cast<Argument<std::string> *>(arg);
                        if (i == argc)
                        {
                            std::cout << string_arg->long_arg << " requires another argument." << std::endl;
                            return false;
                        }
                        *string_arg->value = argv[i];
                        string_arg->assigned = true;
                        break;
                    }
                    case Type::INTEGER:
                    {
                        i++;
                        auto int_arg = reinterpret_cast<Argument<int> *>(arg);
                        if (i == argc)
                        {
                            std::cout << arg->long_arg << " requires another argument." << std::endl;
                            return false;
                        }
                        *int_arg->value = strtol(argv[i], nullptr, 10);
                        int_arg->assigned = true;
                        break;
                    }
                    case Type::BOOL_FLAG:
                    {
                        auto bool_arg = reinterpret_cast<Argument<bool> *>(arg);
                        *bool_arg->value = true;
                        bool_arg->assigned = true;
                        break;
                    }
                    }
                }
            }
        }
        for (auto &a : arguments)
        {
            Argument<void> *arg = reinterpret_cast<Argument<void> *>(a);
            if (!arg->assigned && arg->required == Required::TRUE)
            {
                std::cout << arg->long_arg << " is a required argument." << std::endl;
                return false;
            }
        }
        return true;
    }

    template <typename STORAGE>
    class Argument
    {
    public:
        STORAGE *value;
        Type type;
        const std::string long_arg;
        const std::string short_arg;
        const std::string help;
        const Required required;
        bool assigned{false};

        Argument(const std::string &long_arg, const std::string &short_arg, const std::string &help, Required required)
            : long_arg(long_arg), short_arg(short_arg), help(help), required(required)
        {
        }
    };
    std::vector<void *> arguments;
};