#include <catch2/catch.hpp>
#include "Tga.hpp"

#include <string>
#include <cstdio>

#pragma warning( disable : 6237 6319 )

const std::string path_red = "data/red.tga";
const std::string path_green = "data/green.tga";
const std::string path_blue = "data/blue.tga";

bool validSizes(Tga& img)
{
    return img.data[0].size() == img.data[1].size() 
        && img.data[1].size() == img.data[2].size();
}

TEST_CASE("Create empty TGA object", "[TGA]")
{
    Tga img;
    REQUIRE(img.data.size() == 3);
    CHECK(img.data[0].size() == 0);
    CHECK(img.data[1].size() == 0);
    CHECK(img.data[2].size() == 0);
}

TEST_CASE("Try to load nonexistent image", "[TGA]")
{
    Tga img;
    CHECK_THROWS(img.load("abc"));
}

TEST_CASE("Load red image", "[TGA]")
{
    Tga img(path_red);
    CHECK(validSizes(img));

    for (unsigned int x : img.data[0]) // Blue
        REQUIRE(x == 0);
    for (unsigned int x : img.data[1]) // Green
        REQUIRE(x == 0);
    for (unsigned int x : img.data[2]) // Red
        REQUIRE(x == 255);
}

TEST_CASE("Load green image", "[TGA]")
{
    Tga img(path_green);
    CHECK(validSizes(img));

    for (unsigned int x : img.data[0]) // Blue
        REQUIRE(x == 0);
    for (unsigned int x : img.data[1]) // Green
        REQUIRE(x == 255);
    for (unsigned int x : img.data[2]) // Red
        REQUIRE(x == 0);
}

TEST_CASE("Load blue image", "[TGA]")
{
    Tga img(path_blue);
    CHECK(validSizes(img));

    for (unsigned int x : img.data[0]) // Blue
        REQUIRE(x == 255);
    for (unsigned int x : img.data[1]) // Green
        REQUIRE(x == 0);
    for (unsigned int x : img.data[2]) // Red
        REQUIRE(x == 0);
}

TEST_CASE("Save red image", "[TGA]")
{
    Tga img(path_red);
    img.save("red.tga.test"); // Save new file

    SECTION("Verify that image is saved correctly")
    {
        Tga img_2("red.tga.test");
        CHECK(validSizes(img_2));
        for (unsigned int x : img.data[0]) // Blue
            REQUIRE(x == 0);
        for (unsigned int x : img.data[1]) // Green
            REQUIRE(x == 0);
        for (unsigned int x : img.data[2]) // Red
            REQUIRE(x == 255);
    }
    remove("red.tga.test");
}