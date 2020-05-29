#pragma once

#include "Tga.hpp"
#include "CodedTga.hpp"

#include <stdexcept>

class Statistics
{
public:

	// Mean Squared Error for whole image
	double mse(const Tga& original, const Tga& coded)
	{
		if (original.data.size() != coded.data.size())
			throw std::runtime_error("Images have different sizes.");
		if (original.data.size() == 0)
			return 0.0;

		const size_t num_of_channels = original.data.size();
		const size_t num_of_pixels = original.data[0].size();
		double error = 0;
		for (size_t i = 0; i < num_of_channels; i++)
		{
			for (size_t k = 0; k < num_of_pixels; k++)
			{
				double original_value = original.data[i][k];
				double coded_value = coded.data[i][k];
				error += (original_value - coded_value) * (original_value - coded_value);
			}
		}
		return error / (num_of_channels * num_of_pixels);
	}

	double mse(const std::vector<uint8_t>& original, const std::vector<uint8_t>& coded)
	{
		if (original.size() != coded.size())
			throw std::runtime_error("Channels have different sizes.");
		if (original.size() == 0)
			return 0.0;

		const size_t num_of_pixels = original.size();
		double error = 0;
		for (size_t i = 0; i < num_of_pixels; i++)
		{
			double original_value = original[i];
			double coded_value = coded[i];
			error += (original_value - coded_value) * (original_value - coded_value);
		}

		return error / num_of_pixels;
	}

	// Signal to Noise Ratio for whole image
	double snr(double mse, const Tga& original)
	{
		if (mse == 0 || original.data.size() == 0)
			return 0.0;

		double error = 0;
		const size_t num_of_channels = original.data.size();
		const size_t num_of_pixels = original.data[0].size();
		for (size_t i = 0; i < num_of_channels; i++)
		{
			for (size_t k = 0; k < num_of_pixels; k++)
			{
				double original_value = original.data[i][k];
				error += original_value * original_value;
			}
		}

		return error / (mse * num_of_channels * num_of_pixels);
	}

	double snr(double mse, const std::vector<uint8_t> original)
	{
		if (mse == 0 || original.size() == 0)
			return 0.0;

		double error = 0;
		const size_t num_of_pixels = original.size();
		for (size_t i = 0; i < num_of_pixels; i++)
		{
			double original_value = original[i];
			error += original_value * original_value;
		}

		return error / (mse * num_of_pixels);
	}
};