//#include <CLI/CLI11.hpp>
#include <iostream>
#include <string>
#include <cstdlib>
#include "CodedTga.hpp"
#include "Statistics.hpp"

void printStats(Tga& original, Tga& decoded);

constexpr int powerOf(int a, int n)
{
    return n == 0 ? 1 : a * powerOf(a, n - 1);
}

int main(int argc, char** argv)
{
    if (argc != 4)
    {
        std::cerr << "Bad usage: lip k (in).tga (out).tga" << std::endl;
    }

    int k = atoi(argv[1]);
    std::string in_path(argv[2]);
    std::string out_path(argv[3]);

    int resolution = powerOf(2, k);
    
    std::cout << "Loading original image..." << std::endl;
    Tga original(in_path);

    std::cout << "Encoding the image..." << std::endl;
    CodedTga coded_1(resolution, original, true);
    CodedTga coded_2(resolution, original, false);

    std::cout << "Decoding the image..." << std::endl;
    Tga decoded_1 = coded_1.decode();
    Tga decoded_2 = coded_2.decode();

    std::cout << "Saving decoded image..." << std::endl;
    decoded_1.save("u-" + out_path);
    decoded_2.save("nu-" + out_path);

    std::cout << "--------------" << std::endl;
    std::cout << "Stats (uniform):" << std::endl;
    printStats(original, decoded_1);
    std::cout << "--------------" << std::endl;
    std::cout << "Stats (non-uniform):" << std::endl;
    printStats(original, decoded_2);

    return 0;
}

void printStats(Tga& original, Tga& decoded)
{
    std::cout << "MSE (image): " << Statistics::mse(original, decoded) << std::endl;
    std::cout << "MSE (red)  : " << Statistics::mse(original.data[2], decoded.data[2]) << std::endl;
    std::cout << "MSE (green): " << Statistics::mse(original.data[1], decoded.data[1]) << std::endl;
    std::cout << "MSE (blue) : " << Statistics::mse(original.data[0], decoded.data[0]) << std::endl;
    std::cout << std::endl;
    std::cout << "SNR (image): " << Statistics::snr(original, decoded) << std::endl;
    std::cout << "SNR (red)  : " << Statistics::snr(original.data[2], decoded.data[2]) << std::endl;
    std::cout << "SNR (green): " << Statistics::snr(original.data[1], decoded.data[1]) << std::endl;
    std::cout << "SNR (blue) : " << Statistics::snr(original.data[0], decoded.data[0]) << std::endl;
}