#include "ArgParser.h"
#include <string>

class Arguments : public ArgumentParser
{
public:
    std::string model_file;
    int model_height_meters;
    bool show_stats{false};

    Arguments()
    {
        addArgument("--model-file", "-m", "Path to .obj 3d model file", &model_file, Required::TRUE);
        addArgument("--model-height", "-h", "Model height in meters.  Used with y extent to determine model scale.  Note that Minecraft blocks are 1 meter cubes.", &model_height_meters, Required::TRUE);
        addArgument("--show-stats", "-s", "Print statistics about the model and exit", &show_stats);
    }
};