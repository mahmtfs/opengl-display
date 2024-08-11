#ifndef IMPORTER_H
#define IMPORTER_H

#include "glm.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

class Importer{
private:
    Importer() = delete;
    Importer(Importer const& importer) = delete;
public:
    static std::vector<float> importOBJ(const char* file, const bool textured = false);
};

#endif