#pragma once
#include <string>
#include "similarity_checker.h"

using namespace std;

struct Node {
	string name;
	double point;

	bool operator<(const Node& other) const {
		return point < other.point;
	}
};

#define WEEK_BEST_SLOT_NUM 7
#define TWO_BEST_SLOT_NUM 2
#define MAX_KEYWORD_NUM_IN_SLOT 10
#define MAX_POINT 2100000000

enum SIMILARITY_ALGO
{
	ALGO_LEVENSHTEIN = 1,
	ALGO_END = 9,
};

class KeywordFixTool
{
public:
	void initialize(SIMILARITY_ALGO algoNum);
	void clearAlgorithm();
	void inputFile(string filename);
	string fixKeyword(string keyword, string day);
	bool similer(const std::string& a, const std::string& b);
	int getWeekBestIndex(string day);
	int getTwoBestIndex(int weekBestIndex);
	void resetPoint(int updatedPoint);
	void rearrangeSlot(int weekBestIndex, int twoBestIndex);
	int checkCompleteHit(string keyword, int weekBestIndex, int twoBestIndex);
	bool checkSimilerHit(string keyword, int weekBestIndex, int twoBestIndex);
	void insertNewKeyword(string keyword, int weekBestIndex, int twoBestIndex);
	
	vector<Node> weekBest[WEEK_BEST_SLOT_NUM];	// monday ~ sunday
	vector<Node> twoBest[TWO_BEST_SLOT_NUM];	// weekday, weekend
	double UZ;
	SimilarityCheckAlgo* algo;
};
