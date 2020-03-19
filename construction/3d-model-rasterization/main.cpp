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
int generateModel(const Arguments & args, const ModelBuilder &);

int main(int argc, char **argv)
{
    if (!local_args.parse(argc, argv))
    {
        return -1;
    }

    ModelBuilder builder;
    if (!parseModel(args, builder))
    {
        cout << "Unable to parse model file: " << args.model_file << endl;
        return -1;
    }

    return generateModel(args, builder);
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

int generateModel(const Arguments & args, const ModelBuilder & builder)
{
    ModelBuilder::Statistics stats = builder.getStatistics();
    if (stats.count_vertices == 0 || stats.count_polygons == 0)
    {
        cout << "Error.  Invalid model loaded:" << stats << endl;
        return -1;
    }
    if (args.show_stats)
    {
        cout << stats;
    }
    ModelGenerator generator(builder, args.model_height_meters);
    generator.generate(builder);
    return 0;
}