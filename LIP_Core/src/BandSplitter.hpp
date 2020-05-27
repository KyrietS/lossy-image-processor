#pragma once

#include <vector>
#include <cstdint>

class BandSplitter
{
public:
    std::vector<float> upper_band;
    std::vector<float> lower_band;

    BandSplitter() = default;
    BandSplitter(const std::vector<uint8_t>& data);

    void split(const std::vector<uint8_t>& data);
};