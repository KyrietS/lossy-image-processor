#include "Tga.hpp"

#include <cstdint>
#include <fstream>
#include <stdexcept>

using byte_t = uint8_t;

Tga::Tga(const std::string& path)
{
    load(path);
}

void Tga::load(const std::string& path)
{
	std::ifstream file(path, std::ifstream::binary);
	if (!file.is_open())
		throw std::runtime_error("File cannot be opened.");

	file.seekg(12, file.beg);

	file.read(reinterpret_cast<char*>(&width), 2);
	file.read(reinterpret_cast<char*>(&height), 2);
	size_t num_of_pixels = (size_t)width * (size_t)height;

	file.seekg(2, file.cur);

	data[0].resize(num_of_pixels);
	data[1].resize(num_of_pixels);
	data[2].resize(num_of_pixels);
	for (size_t i = 0; i < num_of_pixels; i++)
	{
		data[0][i] = file.get();
		data[1][i] = file.get();
		data[2][i] = file.get();
	}
}

void Tga::save(const std::string& path)
{
	size_t total_pixels = data[0].size(); // size of one of the channels
	if ((size_t)width * (size_t)height != total_pixels)
		throw std::runtime_error("Incorrect image dimensions");

	std::ofstream file(path, std::ifstream::binary);

	// HEADER
	file.put(0); // image id length
	file.put(0); // color map type
	file.put(2); // image type (uncompressed true-color image)
	file.put(0); // first entry index
	file.put(0);
	file.put(0); // color map length
	file.put(0);
	file.put(0); // color mape entry size
	file.put(0); // x origin
	file.put(0);
	file.put(0); // y origin
	file.put(0);
	file.write(reinterpret_cast<char*>(&width), 2);
	file.write(reinterpret_cast<char*>(&height), 2);
	file.put(24); // pixel depth
	file.put(0);  // image descriptor

	// DATA
	for (size_t i = 0; i < total_pixels; i++)
	{
		file.put(data[0][i]);
		file.put(data[1][i]);
		file.put(data[2][i]);
	}

	// FOOTER
	file.write("\0\0\0\0", 4); // extension offset
	file.write("\0\0\0\0", 4); // developer offset
	file.write("TRUEVISION-XFILE", 16); // signature
	file.put('.'); // dot
	file.put(0);   // NULL
}

