#include "NonUniformQuantizer.hpp"

#include <algorithm>
#include <numeric>
#include <array>

using byte_t = uint8_t;

NonUniformQuantizer::NonUniformQuantizer(size_t resolution)
    : resolution(resolution), histogram{ 0 } {}


void NonUniformQuantizer::quantize(std::vector<byte_t>& data)
{
    auto quantization_table = getQuantizationTable(data);

    // Quantizing values.
    for (byte_t& byte : data)
        byte = quantization_table[byte];
}

std::array<uint8_t, 256> NonUniformQuantizer::getQuantizationTable(const std::vector<uint8_t>& data)
{
    fillHistogram(data);
    findBoundaries();

    // Generating non-uniform quantization table from data.
    std::array<byte_t, 256> quantization_table;
    std::iota(quantization_table.begin(), quantization_table.end(), 0);
    for (size_t i = 1; i < boundaries.size(); i++)
    {
        size_t left = boundaries[i - 1];
        size_t right = boundaries[i];
        byte_t quantized_value = meanBetweenBoundaries(left, right);
        for (size_t k = left; k < right; k++)
            quantization_table[k] = quantized_value;
    }

    return quantization_table;
}

void NonUniformQuantizer::fillHistogram(const std::vector<byte_t>& data)
{
    for (byte_t byte : data)
        histogram[byte]++;
}

void NonUniformQuantizer::findBoundaries()
{
    if (resolution == 0)
        return;

    boundaries.clear();
    boundaries.push_back(0);
    boundaries.push_back(256);

    while (boundaries.size() < resolution + 1)
    {
        std::vector<size_t> new_boundaries;
        std::sort(boundaries.begin(), boundaries.end());

        // For every pair of boundaries find new middle boundary. (log2)
        for (size_t i = 1; i < boundaries.size(); i++)
        {
            size_t boundary = findMiddleBoundary(boundaries[i-1], boundaries[i]);
            new_boundaries.push_back(boundary);
        }

        boundaries.insert(boundaries.end(), new_boundaries.begin(), new_boundaries.end());
    }

    std::sort(boundaries.begin(), boundaries.end());
}

size_t NonUniformQuantizer::findMiddleBoundary(size_t left, size_t right)
{
    long long left_sum = 0;
    long long right_sum = 0;
    long long middle = 0;

    while (left != right)
    {
        if (left_sum < right_sum)
        {
            left_sum += histogram[left];
            left++;
            middle = left;
        }
        else
        {
            middle = right;
            right--;
            right_sum += histogram[right];
        }
    }

    return middle;
}

byte_t NonUniformQuantizer::meanBetweenBoundaries(size_t left, size_t right)
{
    long long weighted_sum = 0;
    long long sum_of_weights = 0;
    for (size_t i = left; i < right; i++)
    {
        weighted_sum += i * histogram[i];
        sum_of_weights += histogram[i];
    }

    if (sum_of_weights == 0)
        return (byte_t)(right - left) / 2;
    else
        return (byte_t)((double)weighted_sum / sum_of_weights + 0.5);
}