#include "DifferentialQuantizer.hpp"
#include "NonUniformQuantizer.hpp"

using byte_t = uint8_t;

#include <cmath>
#include <numeric>

DifferentialQuantizer::DifferentialQuantizer(size_t resolution)
    : resolution(resolution) {}

void DifferentialQuantizer::quantize(std::vector<uint8_t>& data, bool uniform)
{
    if (data.size() == 0)
        return;

    // Get the differences between values in the sequence
    std::vector<int> diffs(data.size());
    diffs[0] = data[0];
    for (size_t i = 1; i < diffs.size(); i++)
        diffs[i] = data[i] - data[i - 1];

    // Get the differences biased into [0, 255] range
    std::vector<byte_t> diffs_biased(diffs.size());
    for (size_t i = 0; i < diffs.size(); i++)
        diffs_biased[i] = toBias(diffs[i]);

    NonUniformQuantizer non_uniform_quantizer(resolution);
    std::array<uint8_t, 256> quantization_table;
    if (!uniform)
        quantization_table = non_uniform_quantizer.getQuantizationTable(diffs_biased);
    else
    {
        for (size_t i = 0; i < resolution; i++)
        {
            size_t step = 256 / resolution;
            size_t begin = step * i;
            size_t end = step * (i + 1);
            for (size_t k = begin; k < end; k++)
                quantization_table[k] = (byte_t)(begin + step / 2); // middle
        }
    }

    // Quantizing the data.
    data[0] = quantization_table[toBias(data[0])];
    int prev = fromBias(data[0]);
    for (size_t i = 1; i < data.size(); i++)
    {
        data[i] = toBias((int)data[i] - prev);
        data[i] = quantization_table[data[i]];
        prev = prev + fromBias(data[i]);
        // Don't overflow!
        //prev = prev < 0 ? 0 : prev;
        //prev = prev > 255 ? 255 : prev;
    }
}
#include <iostream>
void DifferentialQuantizer::decode(std::vector<uint8_t>& data)
{
    // Decode bias.
    std::vector<int> unbiased(data.size());
    for (size_t i = 0; i < data.size(); i++)
    {
        int unbi = fromBias(data[i]);
        unbiased[i] = unbi;
    }

    data[0] = fromBias(data[0]);
    int previous = data[0];
    for (size_t i = 1; i < data.size(); i++)
    {
        int decoded = previous + unbiased[i];
        decoded = decoded < 0 ? 0 : decoded;
        decoded = decoded > 255 ? 255 : decoded;
        data[i] = decoded;
        previous += unbiased[i];
    }
}


