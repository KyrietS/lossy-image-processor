#include <catch2/catch.hpp>
#include "BandSplitter.hpp"

#include <vector>
#include <cstdio>

#pragma warning( disable : 6237 6319 )

TEST_CASE("Create empty Band Splitter", "[BandSplitter]")
{
    BandSplitter bs;
    CHECK(bs.lower_band.size() == 0);
    CHECK(bs.upper_band.size() == 0);
}

TEST_CASE("Split empty set of data", "[BandSplitter]")
{
    BandSplitter bs;
    std::vector<uint8_t> data = {};
    bs.split(data);
    CHECK(bs.lower_band.size() == 0);
    CHECK(bs.upper_band.size() == 0);
}

TEST_CASE("Split bands", "[BandSplitter]")
{
    BandSplitter bs;

    SECTION("Single element")
    {
        std::vector<uint8_t> data = { 100 };
        bs.split(data);
        REQUIRE(bs.upper_band.size() == 1);
        REQUIRE(bs.lower_band.size() == 1);
        CHECK(bs.lower_band[0] == (100 + 0) / 2);
        CHECK(bs.upper_band[0] == (100 - 0) / 2);
    }

    SECTION("Two elements")
    {
        std::vector<uint8_t> data = { 100, 200 };
        bs.split(data);
        REQUIRE(bs.upper_band.size() == 2);
        REQUIRE(bs.lower_band.size() == 2);
        CHECK(bs.lower_band[0] == (100 + 0) / 2);
        CHECK(bs.upper_band[0] == (100 - 0) / 2);
        CHECK(bs.lower_band[1] == (200 + 100) / 2);
        CHECK(bs.upper_band[1] == (200 - 100) / 2);
    }

    SECTION("Many elements")
    {
        // Generate data
        const int DATA_SIZE = 16;
        std::vector<uint8_t> data(DATA_SIZE);
        for (size_t i = 0; i < data.size(); i++)
            data[i] = (uint8_t)i * 7;

        bs.split(data);
        REQUIRE(bs.upper_band.size() == DATA_SIZE);
        REQUIRE(bs.lower_band.size() == DATA_SIZE);
        CHECK(bs.upper_band[0] == (float)data[0] / 2.0f);
        CHECK(bs.lower_band[0] == (float)data[0] / 2.0f);
        for (size_t i = 1; i < DATA_SIZE; i++)
        {
            CHECK(bs.upper_band[i] == ((float)data[i] - (float)data[i - 1]) / 2);
            CHECK(bs.lower_band[i] == ((float)data[i] + (float)data[i - 1]) / 2);
        }
    }
}