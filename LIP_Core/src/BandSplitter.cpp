#include "BandSplitter.hpp"

#include <cassert>

using byte_t = uint8_t;

BandSplitter::BandSplitter(const std::vector<byte_t>& data)
{
    split(data);
}

void BandSplitter::split(const std::vector<byte_t>& data)
{
    if (data.size() == 0)
        return;

    lower_band.clear();
    upper_band.clear();

    for (size_t i = 1; i < data.size(); i += 2)
    {
        float lower = ((float)data[i] + (float)data[i - 1]) / 2.0f;
        float upper = ((float)data[i] - (float)data[i - 1]) / 2.0f;
        lower_band.push_back(lower);
        upper_band.push_back(upper);
    }

    // If data is not even sized
    if (data.size() % 2 != 0)
    {
        float lower = (float)data[data.size() - 1] / 2.0f;
        float upper = -1 * (float)data[data.size() - 1] / 2.0f;
        lower_band.push_back(lower);
        upper_band.push_back(upper);
    }
}

std::vector<byte_t> BandSplitter::mergeEven()
{
    return merge(true);
}

std::vector<byte_t> BandSplitter::mergeUneven()
{
    return merge(false);
}

std::vector<byte_t> BandSplitter::merge(bool even_result)
{
    assert(lower_band.size() == upper_band.size());
    const size_t BAND_SIZE = lower_band.size();
    std::vector<byte_t> result;

    if (even_result)
    {
        for (int i = 0; i < BAND_SIZE; i++)
        {
            float even = lower_band[i] - upper_band[i];
            float uneven = lower_band[i] + upper_band[i];

            result.push_back((byte_t)even);
            result.push_back((byte_t)uneven);
        }
    }
    else if (BAND_SIZE > 0)
    {
        for (int i = 0; i < BAND_SIZE - 1; i++)
        {
            float even = lower_band[i] - upper_band[i];
            float uneven = lower_band[i] + upper_band[i];

            result.push_back((byte_t)even);
            result.push_back((byte_t)uneven);
        }
        float last_element = lower_band[BAND_SIZE - 1] - upper_band[BAND_SIZE - 1];
        result.push_back((byte_t)last_element);
    }

    return result;
}
