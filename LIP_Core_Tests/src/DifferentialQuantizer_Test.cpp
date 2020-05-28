#include <catch2/catch.hpp>
#include "DifferentialQuantizer.hpp"

#include <vector>

#pragma warning( disable : 6237 6319 )

using byte_t = uint8_t;

TEST_CASE("Quantization of empty data set")
{
    std::vector<byte_t> data;
    SECTION("Resolution of 0")
    {
        DifferentialQuantizer quant(0);
        quant.quantize(data);
        CHECK(data.size() == 0);
    }
    SECTION("Resolution of 2")
    {
        DifferentialQuantizer quant(2);
        quant.quantize(data);
        CHECK(data.size() == 0);
    }
}

TEST_CASE("Quantize constant sequence")
{
    std::vector<byte_t> data = { 6, 6, 6, 6 };
    SECTION("Resolution of 1")
    {
        DifferentialQuantizer quant(1);
        quant.quantize(data);
        CHECK(data[0] == data[1]);
        CHECK(data[1] == data[2]);
        CHECK(data[2] == data[3]);
    }
    SECTION("Resolution of 2")
    {
        DifferentialQuantizer quant(2);
        quant.quantize(data);
        quant.decode(data);
        for (size_t i = 0; i < data.size(); i++)
            REQUIRE(data[i] == 6);
    }
    SECTION("Resolution of 16")
    {
        DifferentialQuantizer quant(16);
        quant.quantize(data);
        quant.decode(data);
        for (size_t i = 1; i < data.size(); i++)
            REQUIRE(data[i] == 6);
    }
}

TEST_CASE("Quantize increasing sequence")
{
    std::vector<byte_t> data = { 2, 6, 8 };
    // diffs are: 2, 4, 2.
    // above diffs sequence will be used to generate quantization table.
    
    SECTION("Resolution of 1")
    {
        // All values will be quantized into mean.
        DifferentialQuantizer quant(1);
        quant.quantize(data);
        CHECK(data[0] == data[1]);
        CHECK(data[1] == data[2]);
    }
    SECTION("Resolution of 2")
    {
        DifferentialQuantizer quant(2);
        quant.quantize(data);
        quant.decode(data);
        CHECK(data[0] == 2);
        CHECK(data[1] == 6);
        CHECK(data[2] == 8);
    }
    SECTION("Resolution of 4")
    {
        DifferentialQuantizer quant(4);
        quant.quantize(data);
        quant.decode(data);
        CHECK(data[0] == 2);
        CHECK(data[1] == 6);
        CHECK(data[2] == 8);
    }
}

TEST_CASE("Quantize decreasing sequence")
{
    std::vector<byte_t> data = { 8, 6, 2 };
    // diffs are: 8, -2, -4
    // above diffs sequence will be used to generate quantization table.

    SECTION("Resolution of 1")
    {
        // All values will be quantized into mean.
        DifferentialQuantizer quant(1);
        quant.quantize(data);
        quant.decode(data);
        CHECK(data[0] == data[1]);
        CHECK(data[1] == data[2]);
    }
    SECTION("Resolution of 4")
    {
        DifferentialQuantizer quant(4);
        quant.quantize(data);
        quant.decode(data);
        CHECK(data[0] == 8);
        CHECK(data[1] == 6);
        CHECK(data[2] == 2);
    }
    SECTION("Resolution of 16")
    {
        DifferentialQuantizer quant(4);
        quant.quantize(data);
        quant.decode(data);
        CHECK(data[0] == 8);
        CHECK(data[1] == 6);
        CHECK(data[2] == 2);
    }
}