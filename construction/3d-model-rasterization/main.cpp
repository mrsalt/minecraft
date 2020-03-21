#include <iostream>
#include <memory>
#include <vector>
#include "ModelReader_Wavefront.h"
#include "Arguments.h"
#include "ModelBuilder.h"
#include "ModelGenerator.h"

using namespace std;

Arguments local_args;
const Arguments & args = local_args;

bool parseModel(const Arguments & args, ModelBuilder &);

int main(int argc, char **argv)
{
    if (!local_args.parse(argc, argv))
    {
        return -1;
    }

    if (args.model_height_meters < 1.0)
    {
        cout << "Model height must be >= 1.0" << endl;
        return -1;
    }

    ModelBuilder builder;
    if (!parseModel(args, builder))
    {
        cout << "Unable to parse model file: " << args.model_file << endl;
        return -1;
    }
    try
    {
        ModelGenerator generator(builder, args.model_height_meters);
        generator.generate();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return 0;
}

bool parseModel(const Arguments & args, ModelBuilder & builder)
{
    vector<unique_ptr<ModelReader>> readers;
    readers.push_back(make_unique<ModelReader_Wavefront>());
    for (auto &reader : readers)
    {
        try
        {
            if (!reader->parse(args.model_file, builder))
            {
                continue;
            }
            if (builder.points.size() == 0 || builder.polygons.size() == 0)
            {
                continue;
            }
            return true;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
    return false;
}
