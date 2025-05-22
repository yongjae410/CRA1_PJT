#pragma once
#include "similarity_checker.h"
#include <string>

class LevenshteinAlgo : public SimilarityCheckAlgo
{
public:
	virtual int getSimilarity(std::string a, std::string b) override;
};