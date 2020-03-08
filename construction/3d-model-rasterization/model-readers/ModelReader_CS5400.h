#pragma once
#include "ModelReader.h"

class ModelReader_CS5400 : public ModelReader
{
public:
    virtual bool canParse(const std::string &);
    virtual bool parse(const std::string &, ModelConsumer &);
};