#pragma once

#include <vector>
#include <cstdint>

class DifferentialQuantizer
{
public:
    DifferentialQuantizer(size_t resolution);
    void quantize(std::vector<uint8_t>& data, bool uniform = false);
    void decode(std::vector<uint8_t>& data);
private:
    size_t resolution;

    uint8_t toBias(int value)
    {
        return (uint8_t)(((value+1) / 2) + 127);
    }

    int fromBias(uint8_t value)
    {
        return ((int)value - 127) * 2;
    }
};