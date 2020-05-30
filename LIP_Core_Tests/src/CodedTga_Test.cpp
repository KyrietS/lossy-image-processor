#include <catch2/catch.hpp>
#include "CodedTga.hpp"

#include <string>
#include <fstream>
#include <cstdio> // remove
#include <cmath>

#pragma warning( disable : 6237 6319 )

const std::string path_red = "data/red.tga";
const std::string path_green = "data/green.tga";
const std::string path_blue = "data/blue.tga";
const std::string path_color = "data/color.tga";

TEST_CASE("Create empty Coded Tga object")
{
    CodedTga ctga(1);
    REQUIRE(ctga.channels.size() == 3);
    CHECK(ctga.channels[0].upper_band.size() == 0);
    CHECK(ctga.channels[1].upper_band.size() == 0);
    CHECK(ctga.channels[2].upper_band.size() == 0);
    CHECK(ctga.channels[0].lower_band.size() == 0);
    CHECK(ctga.channels[1].lower_band.size() == 0);
    CHECK(ctga.channels[2].lower_band.size() == 0);
}

TEST_CASE("Encode & Decode red image with resolution of 2")
{
    CodedTga ctga(2);
    Tga tga(path_red);
    ctga.encode(tga);      
    Tga result = ctga.decode();

    for (size_t i = 0; i < result.data[0].size(); i++)
    {
        REQUIRE(result.data[0][i] == 0);  // Blue
        REQUIRE(result.data[1][i] == 0);  // Green
        REQUIRE(result.data[2][i] >= 254);  // Red
    }
}

TEST_CASE("Encode & Decode green image with resolution of 2")
{
    CodedTga ctga(2);
    Tga tga(path_green);
    ctga.encode(tga);
    Tga result = ctga.decode();

    for (size_t i = 0; i < result.data[0].size(); i++)
    {
        REQUIRE(result.data[0][i] == 0);  // Blue
        REQUIRE(result.data[1][i] >= 254);  // Green
        REQUIRE(result.data[2][i] >= 0);  // Red
    }
}

TEST_CASE("Encode & Decode blue image with resolution of 2")
{
    CodedTga ctga(2);
    Tga tga(path_blue);
    ctga.encode(tga);
    Tga result = ctga.decode();

    for (size_t i = 0; i < result.data[0].size(); i++)
    {
        REQUIRE(result.data[0][i] >= 254);  // Blue
        REQUIRE(result.data[1][i] == 0);  // Green
        REQUIRE(result.data[2][i] == 0);  // Red
    }
}

TEST_CASE("Encode & Decode colorful image with resolution of 8")
{
    Tga tga(path_color);
    CodedTga ctga(8);
    ctga.encode(tga);
    Tga result = ctga.decode();
    
    // The image should be decoded with almost no loss
    for (size_t i = 0; i < result.data[0].size(); i++)
    {
        REQUIRE(std::abs((int)result.data[0][i] - (int)tga.data[0][i]) <= 1);
        REQUIRE(std::abs((int)result.data[1][i] - (int)tga.data[1][i]) <= 1);
        REQUIRE(std::abs((int)result.data[2][i] - (int)tga.data[2][i]) <= 1);
    }
}
