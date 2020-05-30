#pragma once

#include <string>
#include <vector>
#include <array>
#include <cstdint>

class Tga
{
public:
    std::array<std::vector<uint8_t>, 3> data;
    uint16_t width = 0;
    uint16_t height = 0;

    Tga() = default;
    Tga(const std::string & path);
    void load(const std::string & path);
    void save(const std::string & path);

};