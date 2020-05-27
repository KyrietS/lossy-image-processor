#include "BandSplitter.hpp"

using byte_t = uint8_t;

BandSplitter::BandSplitter(const std::vector<byte_t>& data)
{
    split(data);
}

void BandSplitter::split(const std::vector<byte_t>& data)
{
    if (data.size() == 0)  
        return;

    upper_band.resize(data.size());
    lower_band.resize(data.size());

    upper_band[0] = (float)data[0] / 2;
    lower_band[0] = (float)data[0] / 2;

    for (size_t i = 1; i < data.size(); i++)
    {
        lower_band[i] = ((float)data[i] + (float)data[i - 1]) / 2.0f;
        upper_band[i] = ((float)data[i] - (float)data[i - 1]) / 2.0f;
    }
}
