#include <memory>
#include <string>
#include <vector>
#include <iostream>

enum class Required
{
    TRUE,
    FALSE
};

class ArgumentParser
{
public:
    enum Type
    {
        UNDEFINED = 0,
        STRING,
        INTEGER,
        BOOL_FLAG
    };

    void addArgument(const std::string long_arg, const std::string short_arg, const std::string &help, std::string *dest, Required required = Required::FALSE)
    {
        auto arg = std::make_unique<Argument>(long_arg, short_arg, help, required);
        arg->type = Type::STRING;
        arg->value.string_value = dest;
        arguments.push_back(std::move(arg));
    }

    void addArgument(const std::string long_arg, const std::string short_arg, const std::string &help, int *dest, Required required = Required::FALSE)
    {
        auto arg = std::make_unique<Argument>(long_arg, short_arg, help, required);
        arg->type = Type::INTEGER;
        arg->value.int_value = dest;
        arguments.push_back(std::move(arg));
    }

    void addArgument(const std::string long_arg, const std::string short_arg, const std::string &help, bool *dest)
    {
        auto arg = std::make_unique<Argument>(long_arg, short_arg, help, Required::FALSE);
        arg->type = Type::BOOL_FLAG;
        arg->value.bool_value = dest;
        arguments.push_back(std::move(arg));
    }

    void printArgs(Required type)
    {
        for (auto &arg : arguments)
        {
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
        for (auto &arg : arguments)
            if (arg->required == Required::TRUE)
                required++;
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
            for (auto &arg : arguments)
            {
                if (current_arg == arg->long_arg || current_arg == arg->short_arg)
                {
                    switch (arg->type)
                    {
                    case Type::STRING:
                        i++;
                        if (i == argc)
                        {
                            std::cout << arg->long_arg << " requires another argument." << std::endl;
                            return false;
                        }
                        *arg->value.string_value = argv[i];
                        arg->assigned = true;
                        break;
                    case Type::INTEGER:
                        i++;
                        if (i == argc)
                        {
                            std::cout << arg->long_arg << " requires another argument." << std::endl;
                            return false;
                        }
                        *arg->value.int_value = strtol(argv[i], nullptr, 10);
                        arg->assigned = true;
                        break;
                    case Type::BOOL_FLAG:
                        *arg->value.bool_value = true;
                        arg->assigned = true;
                        break;
                    }
                }
            }
        }
        for (auto &arg : arguments)
        {
            if (!arg->assigned && arg->required == Required::TRUE)
            {
                std::cout << arg->long_arg << " is a required argument." << std::endl;
                return false;
            }
        }
        return true;
    }

    class Argument
    {
    public:
        union Value {
            std::string *string_value;
            int *int_value;
            bool *bool_value;
        };
        Value value;
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
    std::vector<std::unique_ptr<Argument>> arguments;
};