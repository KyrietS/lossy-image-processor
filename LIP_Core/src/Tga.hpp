#pragma once

#include <string>
#include <vector>
#include <array>
#include <cstdint>

class Tga
{
public:
    std::array<std::vector<uint8_t>, 3> data;

    Tga() = default;
    Tga(const std::string & path);
    void load(const std::string & path);
    void save(const std::string & path, uint16_t width, uint16_t height);

};