#pragma once

#include <vector>
#include <array>

class NonUniformQuantizer
{
public:
    NonUniformQuantizer(size_t resolution);

    void quantize(std::vector<uint8_t>& data);
    std::array<uint8_t, 256> getQuantizationTable(const std::vector<uint8_t>& data);
private:
    const size_t resolution;
    std::array<uint64_t, 256> histogram;
    std::vector<size_t> boundaries;

    void fillHistogram(const std::vector<uint8_t>& data);
    void findBoundaries();
    size_t findMiddleBoundary(size_t left, size_t right);
    uint8_t meanBetweenBoundaries(size_t left, size_t right);
};
