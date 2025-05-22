#pragma once
#include <string>

class SimilarityCheckAlgo
{
public:
	virtual int getSimilarity(std::string a, std::string b) = 0;
};