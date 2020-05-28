#include <catch2/catch.hpp>
#include "BandSplitter.hpp"

#include <vector>
#include <cstdio>

#pragma warning( disable : 6237 6319 )

using byte_t = uint8_t;

TEST_CASE("Create empty Band Splitter")
{
    BandSplitter bs;
    CHECK(bs.upper_band.size() == 0);
    CHECK(bs.lower_band.size() == 0);
}

TEST_CASE("Split empty set of data")
{
    BandSplitter bs;
    std::vector<byte_t> data = {};
    bs.split(data);
    CHECK(bs.upper_band.size() == 0);
    CHECK(bs.lower_band.size() == 0);
}

TEST_CASE("Split data with single element")
{
    BandSplitter bs;
    std::vector<byte_t> data = { 100 };
    bs.split(data);
    REQUIRE(bs.upper_band.size() == 1);
    REQUIRE(bs.lower_band.size() == 1);
    CHECK(bs.upper_band[0] == -1 * (100 - 0) / 2);
    CHECK(bs.lower_band[0] == (100 + 0) / 2);
}

TEST_CASE("Split data with two elements")
{
    BandSplitter bs;
    std::vector<byte_t> data = { 100, 200 };
    bs.split(data);
    REQUIRE(bs.upper_band.size() == 1);
    REQUIRE(bs.lower_band.size() == 1);
    CHECK(bs.upper_band[0] == (200 - 100) / 2);
    CHECK(bs.lower_band[0] == (200 + 100) / 2);
}

TEST_CASE("Split data with many elements (even)")
{
    BandSplitter bs;
    // Generate data
    const int DATA_SIZE = 16;
    std::vector<byte_t> data(DATA_SIZE);
    for (size_t i = 0; i < data.size(); i++)
        data[i] = (byte_t)i * 7;

    bs.split(data);
    const int BAND_SIZE = (DATA_SIZE + 1) / 2;
    REQUIRE(bs.upper_band.size() == BAND_SIZE);
    REQUIRE(bs.lower_band.size() == BAND_SIZE);
    CHECK(bs.upper_band[0] == ((float)data[1] - (float)data[0]) / 2.0f);
    CHECK(bs.lower_band[0] == ((float)data[1] + (float)data[0]) / 2.0f);
    for (size_t i = 0; i < BAND_SIZE; i++)
    {
        REQUIRE(bs.upper_band[i] == ((float)data[2*i + 1] - (float)data[2*i]) / 2);
        REQUIRE(bs.lower_band[i] == ((float)data[2*i + 1] + (float)data[2*i]) / 2);
    }
}

TEST_CASE("Split data with many elements (uneven)")
{
    BandSplitter bs;
    // Generate data
    const int DATA_SIZE = 17;
    std::vector<byte_t> data(DATA_SIZE);
    for (size_t i = 0; i < data.size(); i++)
        data[i] = (byte_t)i * 7;

    bs.split(data);
    const int BAND_SIZE = (DATA_SIZE + 1) / 2;
    REQUIRE(bs.upper_band.size() == BAND_SIZE);
    REQUIRE(bs.lower_band.size() == BAND_SIZE);
    CHECK(bs.upper_band[0] == ((float)data[1] - (float)data[0]) / 2.0f);
    CHECK(bs.lower_band[0] == ((float)data[1] + (float)data[0]) / 2.0f);
    for (size_t i = 0; i < BAND_SIZE-1; i++)
    {
        REQUIRE(bs.upper_band[i] == ((float)data[2 * i + 1] - (float)data[2 * i]) / 2);
        REQUIRE(bs.lower_band[i] == ((float)data[2 * i + 1] + (float)data[2 * i]) / 2);
    }
    CHECK(bs.upper_band[BAND_SIZE - 1] == -1 * (float)data[data.size() - 1] / 2);
    CHECK(bs.lower_band[BAND_SIZE - 1] == (float)data[data.size() - 1] / 2);
}

TEST_CASE("Merge empty set of data")
{
    BandSplitter bs;
    auto result_1 = bs.mergeEven();
    CHECK(result_1.size() == 0);
    auto result_2 = bs.mergeUneven();
    CHECK(result_2.size() == 0);
}

TEST_CASE("Merge bands with single element")
{
    BandSplitter bs;
    std::vector<byte_t> data = { 50 };
    bs.split(data);
    REQUIRE(bs.upper_band.size() == 1);
    REQUIRE(bs.lower_band.size() == 1);

    SECTION("Uneven merge")
    {
        auto result = bs.mergeUneven();
        REQUIRE(result.size() == 1);
        CHECK(result[0] == 50);
    }
    SECTION("Even merge")
    {
        auto result = bs.mergeEven();
        REQUIRE(result.size() == 2);
        CHECK(result[0] == 50);
        CHECK(result[1] == 0);
    }

}

TEST_CASE("Merge bands with two elements")
{
    BandSplitter bs;
    std::vector<byte_t> data = { 50, 80 };
    bs.split(data);

    REQUIRE(bs.upper_band.size() == 1);
    REQUIRE(bs.lower_band.size() == 1);

    auto result = bs.mergeEven();
    REQUIRE(result.size() == 2);
    CHECK(result[0] == 50);
    CHECK(result[1] == 80);
}

TEST_CASE("Merge bands with many elements (even)")
{
    BandSplitter bs;
    const int DATA_SIZE = 10;
    const int BAND_SIZE = DATA_SIZE / 2;
    std::vector<byte_t> data(DATA_SIZE);
    for (size_t i = 0; i < data.size(); i++)
        data[i] = (byte_t)i * 7;

    bs.split(data);
    REQUIRE(bs.upper_band.size() == BAND_SIZE);
    REQUIRE(bs.lower_band.size() == BAND_SIZE);

    auto result = bs.mergeEven();
    REQUIRE(result.size() == data.size());

    for (size_t i = 0; i < data.size(); i++)
        REQUIRE(result[i] == data[i]);
}

TEST_CASE("Merge bands with many elements (uneven)")
{
    BandSplitter bs;
    const int DATA_SIZE = 11;
    const int BAND_SIZE = (DATA_SIZE + 1) / 2;
    std::vector<byte_t> data(DATA_SIZE);
    for (size_t i = 0; i < data.size(); i++)
        data[i] = (byte_t)i * 7;

    bs.split(data);
    REQUIRE(bs.upper_band.size() == BAND_SIZE);
    REQUIRE(bs.lower_band.size() == BAND_SIZE);

    auto result = bs.mergeUneven();
    REQUIRE(result.size() == data.size());

    for (size_t i = 0; i < data.size(); i++)
        REQUIRE(result[i] == data[i]);
}