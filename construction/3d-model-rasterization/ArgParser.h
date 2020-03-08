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
        STRING
    };

    void addStringArgument(const std::string long_arg, const std::string short_arg, std::string *dest, Required required = Required::FALSE)
    {
        auto arg = std::make_unique<Argument>(long_arg, short_arg, required);
        arg->type = Type::STRING;
        arg->value.string_value = dest;
        arguments.push_back(std::move(arg));
    }

    bool go(int argc, char **argv)
    {
        for (int i = 0; i < argc; i++)
        {
            std::string current_arg(argv[i]);
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
        };
        Value value;
        Type type;
        const std::string long_arg;
        const std::string short_arg;
        const Required required;
        bool assigned = {false};

        Argument(const std::string &long_arg, const std::string &short_arg, Required required)
            : long_arg(long_arg), short_arg(short_arg), required(required)
        {
        }
    };
    std::vector<std::unique_ptr<Argument>> arguments;
};