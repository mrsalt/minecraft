#include <iostream>
#include <memory>
#include <vector>
#include "ArgParser.h"
#include "model-readers/ModelReader_CS5400.h"
#include "ModelBuilder.h"

using namespace std;

int main(int argc, char **argv)
{
    string model_file;
    ArgumentParser parser;
    parser.addStringArgument("--model", "-m", &model_file, Required::TRUE);
    if (!parser.go(argc, argv))
        return -1;
    vector<unique_ptr<ModelReader>> readers;
    readers.push_back(make_unique<ModelReader_CS5400>());

    for (auto &reader : readers)
    {
        if (reader->canParse(model_file))
        {
            ModelBuilder builder;
            if (!reader->parse(model_file, builder))
            {
                return -1;
            }
            break;
        }
    }
    return 0;
}
