#include "DifferentialQuantizer.hpp"
#include "NonUniformQuantizer.hpp"

using byte_t = uint8_t;

DifferentialQuantizer::DifferentialQuantizer(size_t resolution)
    : resolution(resolution) {}

void DifferentialQuantizer::quantize(std::vector<uint8_t>& data)
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
    auto quantization_table = non_uniform_quantizer.getQuantizationTable(diffs_biased);

    // Quantizing the data.
    data[0] = quantization_table[toBias(data[0])];
    byte_t prev = fromBias(data[0]);
    for (size_t i = 1; i < data.size(); i++)
    {
        data[i] = toBias((int)data[i] - prev);
        data[i] = quantization_table[data[i]];
        prev = prev + fromBias(data[i]);
    }
}

void DifferentialQuantizer::decode(std::vector<uint8_t>& data)
{
    // Decode bias.
    for (auto& x : data)
        x = fromBias(x);

    for (size_t i = 1; i < data.size(); i++)
    {
        data[i] = data[i - 1] + data[i];
    }
}


