#include "CodedTga.hpp"
#include "BandSplitter.hpp"
#include "NonUniformQuantizer.hpp"
#include "DifferentialQuantizer.hpp"

#include <stdexcept>

using byte_t = uint8_t;

CodedTga::CodedTga(int resolution)
    : resolution(resolution) {}

CodedTga::CodedTga(int resolution, const Tga& original, bool uniform)
    : resolution(resolution)
{
    encode(original, uniform);
}

void CodedTga::encode(const Tga& original, bool uniform_diff)
{
    if (original.data.size() != channels.size())
        throw std::runtime_error("Incompatible image.");

    width = original.width;
    height = original.height;

    // Split every channel into 2 bands separately.
    for (size_t i = 0; i < channels.size(); i++)
    {
        BandSplitter band_splitter(original.data[i]);
        channels[i].upper_band = floatToByteVectorBiased(band_splitter.upper_band);
        channels[i].lower_band = floatToByteVector(band_splitter.lower_band);
        channels[i].even = original.data[i].size() % 2 == 0;
    }

    // Quantize bands
    for (size_t i = 0; i < channels.size(); i++)
    {
        // Upper band is quantized with non-uniform quantizer
        NonUniformQuantizer non_uniform_quantizer(resolution);
        non_uniform_quantizer.quantize(channels[i].upper_band);

        // Lower band is quantized differentialy with non-uniform quantizer
        DifferentialQuantizer diff_quantizer(resolution);
        diff_quantizer.quantize(channels[i].lower_band, uniform_diff);
    }
}

Tga CodedTga::decode()
{
    // Decode lower band
    for (size_t i = 0; i < channels.size(); i++)
    {
        DifferentialQuantizer diff_quantizer(resolution);
        diff_quantizer.decode(channels[i].lower_band);
    }

    Tga result;
    result.width = width;
    result.height = height;

    // Merge quantized bands together
    for (size_t i = 0; i < channels.size(); i++)
    {
        auto upper_band = byteToFloatVectorBiased(channels[i].upper_band);
        auto lower_band = byteToFloatVector(channels[i].lower_band);

        BandSplitter band_splitter(upper_band, lower_band);
        if (channels[i].even)
            result.data[i] = band_splitter.mergeEven();
        else
            result.data[i] = band_splitter.mergeUneven();
    }

    return result;
}

std::vector<byte_t> CodedTga::floatToByteVector(const std::vector<float>& vec)
{
    std::vector<uint8_t> result(vec.size());
    for (size_t i = 0; i < vec.size(); i++)
    {
        result[i] = (byte_t)(vec[i] + 0.5f);
    }
    return result;
}

std::vector<byte_t> CodedTga::floatToByteVectorBiased(const std::vector<float>& vec)
{
    std::vector<uint8_t> result(vec.size());
    for (size_t i = 0; i < vec.size(); i++)
    {
        result[i] = (byte_t)(vec[i] + 127 + 0.5f);
    }
    return result;
}

std::vector<float> CodedTga::byteToFloatVector(const std::vector<uint8_t>& vec)
{
    std::vector<float> result(vec.size());
    for (size_t i = 0; i < vec.size(); i++)
        result[i] = (float)vec[i];
    return result;
}

std::vector<float> CodedTga::byteToFloatVectorBiased(const std::vector<uint8_t>& vec)
{
    std::vector<float> result(vec.size());
    for (size_t i = 0; i < vec.size(); i++)
        result[i] = (float)vec[i] - 127;
    return result;
}
