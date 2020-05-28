#include <catch2/catch.hpp>
#include "NonUniformQuantizer.hpp"

#include <vector>
#include <cstdio>
#include <numeric>
#include <cmath>

#pragma warning( disable : 6237 6319 )

using byte_t = uint8_t;

TEST_CASE("Quantization of empty set of data does nothing")
{
    NonUniformQuantizer quant(8);
    std::vector<byte_t> empty_data;
    quant.quantize(empty_data);

    CHECK(empty_data.size() == 0);
}

TEST_CASE("Resolution of 0 does nothing")
{
    NonUniformQuantizer quant(0);
    std::vector<byte_t> data = { 1, 2, 3 };
    quant.quantize(data);
    REQUIRE(data.size() == 3);
    CHECK(data[0] == 1);
    CHECK(data[1] == 2);
    CHECK(data[2] == 3);
}

TEST_CASE("Quantization of data with 1 element")
{
    std::vector<byte_t> data = { 100 };

    SECTION("Resolution of 1")
    {
        NonUniformQuantizer quant(1);
        quant.quantize(data);
    }
    SECTION("Resolution of 2")
    {
        NonUniformQuantizer quant(2);
        quant.quantize(data);
    }
    SECTION("Resolution of 3")
    {
        NonUniformQuantizer quant(3);
        quant.quantize(data);
    }
    SECTION("Resolution of 10")
    {
        NonUniformQuantizer quant(10);
        quant.quantize(data);
    }

    CHECK(data.size() == 1);
    CHECK(data[0] == 100);
}

TEST_CASE("Quantization of data with 2 different elements.")
{
    std::vector<byte_t> data = { 100, 200 };

    SECTION("Resolution of 1")
    {
        NonUniformQuantizer quant(1);
        quant.quantize(data);
        CHECK(data[0] == 150);
        CHECK(data[1] == 150);
    }
    SECTION("Resolution of 2")
    {
        NonUniformQuantizer quant(2);
        quant.quantize(data);
        CHECK(data[0] == 100);
        CHECK(data[1] == 200);
    }
    SECTION("Resolution of 3")
    {
        NonUniformQuantizer quant(3);
        quant.quantize(data);
        CHECK(data[0] == 100);
        CHECK(data[1] == 200);
    }
    SECTION("Resolution of 10")
    {
        NonUniformQuantizer quant(10);
        quant.quantize(data);
        CHECK(data[0] == 100);
        CHECK(data[1] == 200);
    }
}

TEST_CASE("Quantization of data with 2 the same element")
{
    std::vector<byte_t> data = { 100, 100 };

    SECTION("Resolution of 1")
    {
        NonUniformQuantizer quant(1);
        quant.quantize(data);

    }
    SECTION("Resolution of 2")
    {
        NonUniformQuantizer quant(2);
        quant.quantize(data);
    }
    SECTION("Resolution of 3")
    {
        NonUniformQuantizer quant(3);
        quant.quantize(data);
    }
    SECTION("Resolution of 10")
    {
        NonUniformQuantizer quant(10);
        quant.quantize(data);
    }
    CHECK(data[0] == 100);
    CHECK(data[1] == 100);
}

TEST_CASE("Quantization of data with resolution of 1")
{
    std::vector<byte_t> data = {
        50,
        100, 100, 100,
        200, 200, 200,
        250
    };

    NonUniformQuantizer quant(1);
    quant.quantize(data);
    byte_t mean = (byte_t)(std::accumulate(data.begin(), data.end(), 0LL) / data.size());

    for (byte_t byte : data)
        REQUIRE(byte == mean);
}

TEST_CASE("Quantization of data with resolution of 2")
{
    std::vector<byte_t> data = {
        50,
        100, 100, 100,
        200, 200, 200,
        250
    };

    NonUniformQuantizer quant(2);
    quant.quantize(data);

    byte_t mean_left = (byte_t)std::round((50 + 100 + 100 + 100) / 4.0);
    byte_t mean_right = (byte_t)std::round((200 + 200 + 200 + 250) / 4.0);

    for (size_t i = 0; i < data.size() / 2; i++) // First half.
        REQUIRE(data[i] == mean_left);
    for (size_t i = data.size() / 2; i < data.size(); i++) // Second half.
        REQUIRE(data[i] == mean_right);
}

TEST_CASE("Quantization of data with resolution of 4")
{
    std::vector<byte_t> data = {
        50,
        100, 100, 100,
        200, 200, 200,
        250
    };
    std::vector<byte_t> data_copy = data;

    NonUniformQuantizer quant(4);
    quant.quantize(data); // No data is lost

    CHECK(data == data_copy);
}

TEST_CASE("Generate quantization table for empty data set")
{
    std::vector<byte_t> data;

    SECTION("Resolution of 0")
    {
        NonUniformQuantizer quant(0);
        auto quant_table = quant.getQuantizationTable(data);
        REQUIRE(quant_table.size() == 256);
        for (size_t i = 0; i < quant_table.size(); i++)
            REQUIRE(quant_table[i] == i);
    }
}

TEST_CASE("Generate quantization table from data")
{
    std::vector<byte_t> data = {
        50,
        100, 100, 100,
        200, 200, 200,
        250
    };
    NonUniformQuantizer quant(2);
    auto quant_table = quant.getQuantizationTable(data);

    byte_t mean_left = (byte_t)std::round((50 + 100 + 100 + 100) / 4.0);
    byte_t mean_right = (byte_t)std::round((200 + 200 + 200 + 250) / 4.0);

    REQUIRE(quant_table.size() == 256);

    for (size_t i = 0; i <= 101; i++) // First half.
        REQUIRE((int)quant_table[i] == (int)mean_left);
    for (size_t i = 102; i < 255; i++) // Second half.
        REQUIRE((int)quant_table[i] == (int)mean_right);
}