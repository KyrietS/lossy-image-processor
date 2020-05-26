#pragma once
#include <string>
#include "Statistics.hpp"

class LIPCoder
{
public:
	LIPCoder(bool printProgress = false)
		: printProgress(printProgress) {}

	Statistics encode(const std::string & path_in, const std::string & path_out);
	Statistics decode(const std::string & path_in, const std::string & path_out);
private:
	bool printProgress;
	int currentProgress = 0;
	const int progressbarWidth = 70;

	void updateProgress(double progress);
	void clearProgress();
};
