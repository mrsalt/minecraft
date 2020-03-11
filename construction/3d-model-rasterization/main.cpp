#include <iostream>
#include <memory>
#include <vector>
#include "model-readers/ModelReader_CS5400.h"
#include "Arguments.h"
#include "ModelBuilder.h"

using namespace std;

int main(int argc, char **argv)
{
    Arguments args;
    if (!args.parse(argc, argv))
    {
        return -1;
    }
    vector<unique_ptr<ModelReader>> readers;
    readers.push_back(make_unique<ModelReader_CS5400>());

    for (auto &reader : readers)
    {
        if (reader->canParse(args.model_file))
        {
            ModelBuilder builder;
            if (!reader->parse(args.model_file, builder))
            {
                return -1;
            }
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
            break;
        }
    }
    return 0;
}
