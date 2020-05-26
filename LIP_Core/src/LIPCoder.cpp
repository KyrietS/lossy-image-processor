//
// Copyright (c) 2020 Sebastian Fojcik
//

#include "LIPCoder.hpp"
#include "Statistics.hpp"

#include <iostream>

using byte_t = unsigned char;

Statistics LIPCoder::encode(const std::string & path_in, const std::string &path_out)
{
	return Statistics();
}

Statistics LIPCoder::decode(const std::string & path_in, const std::string & path_out)
{
	return Statistics();
}

void LIPCoder::updateProgress(double progress)
{
	if (!printProgress || int(progress * 100.0) == currentProgress)
		return;

	currentProgress = int(progress * 100.0);

	std::cout << "[";
	int pos = (int)((double)progressbarWidth * progress);
	for (int i = 0; i < progressbarWidth; ++i) {
		if (i < pos) std::cout << "=";
		else if (i == pos) std::cout << ">";
		else std::cout << " ";
	}
	std::cout << "] " << currentProgress << " %\r";
	std::cout.flush();
}

void LIPCoder::clearProgress()
{
	if (printProgress)
	{
		std::cout << std::string((size_t)progressbarWidth + 8, ' ');
		std::cout << "\r";
		std::cout.flush();
	}
}
