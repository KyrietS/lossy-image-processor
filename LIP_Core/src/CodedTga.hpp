#pragma once

#include "Tga.hpp"
#include "BandSplitter.hpp"
#include <array>
#include <vector>
#include <cstdint>

class CodedTga
{
    struct Band
    {
        std::vector<uint8_t> upper_band;
        std::vector<uint8_t> lower_band;
        bool even;
    };
public:
    std::array<Band, 3> channels;

    CodedTga(int resolution);
    CodedTga(int resolution, const Tga& original, bool uniform);
    void encode(const Tga& original, bool uniform = false);
    Tga decode();

private:
    int resolution;
    uint16_t width = 0;
    uint16_t height = 0;

    std::vector<uint8_t> floatToByteVectorBiased(const std::vector<float>& vec);
    std::vector<uint8_t> floatToByteVector(const std::vector<float>& vec);
    
    std::vector<float> byteToFloatVectorBiased(const std::vector<uint8_t>& vec);
    std::vector<float> byteToFloatVector(const std::vector<uint8_t>& vec);
};