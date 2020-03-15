#pragma once
#include "ModelReader.h"

// See https://en.wikipedia.org/wiki/Wavefront_.obj_file
// This is the file format used in USU CS 5400, Computer Graphics, in 2001.
class ModelReader_Wavefront : public ModelReader
{
public:
    virtual bool parse(const std::string &, ModelConsumer &);
};