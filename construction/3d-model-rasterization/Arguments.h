#include "ArgParser.h"
#include <string>

class Arguments : public ArgumentParser
{
public:
    std::string model_file;
    int model_height_meters;
    bool show_stats{false};
    std::string output_directory{"."};
    bool output_cross_model{false};
    bool verbose_output{false};

    Arguments()
    {
        addArgument<Type::STRING>("--model-file", "-m", "Path to .obj 3d model file", &model_file, Required::TRUE);
        addArgument<Type::INTEGER>("--model-height", "-h", "Model height in meters.  Used with y extent to determine model scale.  Note that Minecraft blocks are 1 meter cubes.", &model_height_meters, Required::TRUE);
        addArgument<Type::BOOL_FLAG>("--show-stats", "-s", "Print statistics about the model and exit", &show_stats);
        addArgument<Type::STRING>("--output-dir", "-o", "Directory where output data is to be written", &output_directory);
        addArgument<Type::BOOL_FLAG>("--output-cross-model", "-oc", "Output a 3D cross section model", &output_cross_model);
        addArgument<Type::BOOL_FLAG>("--verbose", "-v", "Output verbose logging", &verbose_output);
    }
};