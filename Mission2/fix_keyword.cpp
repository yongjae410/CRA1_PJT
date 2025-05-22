#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

#include "fix_keyword.h"
#include "levenshtein_algo.h"

using namespace std;

bool KeywordFixTool::similer(const std::string& a, const std::string& b) {
	if (a.empty() || b.empty()) return false;

	int dist = algo->getSimilarity(a, b);
	int max_len = std::max(a.length(), b.length());
	double similarity = 1.0 - (double)dist / max_len;

	int score = 1 + static_cast<int>(similarity * 99);

	if (score >= 80) return true;
	return false;
}

int KeywordFixTool::getWeekBestIndex(string day)
{
	if (day == "monday") return 0;
	if (day == "tuesday") return 1;
	if (day == "wednesday") return 2;
	if (day == "thursday") return 3;
	if (day == "friday") return 4;
	if (day == "saturday") return 5;
	if (day == "sunday") return 6;
	return -1;
}

int KeywordFixTool::getTwoBestIndex(int weekBestIndex)
{
	if (weekBestIndex >= 0 && weekBestIndex <= 4) return 0;
	return 1;
}

void KeywordFixTool::resetPoint(int updatedPoint)
{
	if (UZ >= MAX_POINT || updatedPoint >= MAX_POINT)
	{
		UZ = 9.0;
		for (int i = 0; i < WEEK_BEST_SLOT_NUM; i++)
		{
			int score = weekBest[i].size();
			for (Node& node : weekBest[i])
			{
				node.point = score;
				score--;
			}
		}

		for (int i = 0; i < TWO_BEST_SLOT_NUM; i++)
		{
			int score = twoBest[i].size();
			for (Node& node : twoBest[i])
			{
				node.point = score;
				score--;
			}
		}
	}
}

void KeywordFixTool::rearrangeSlot(int weekBestIndex, int twoBestIndex)
{
	std::sort(weekBest[weekBestIndex].rbegin(), weekBest[weekBestIndex].rend());
	std::sort(twoBest[twoBestIndex].rbegin(), twoBest[twoBestIndex].rend());

	while (weekBest[weekBestIndex].size() > 10)
	{
		weekBest[weekBestIndex].pop_back();
	}

	while (twoBest[twoBestIndex].size() > 10)
	{
		twoBest[twoBestIndex].pop_back();
	}
}

int KeywordFixTool::checkCompleteHit(string keyword, int weekBestIndex, int twoBestIndex)
{
	for (Node& node : weekBest[weekBestIndex])
	{
		if (node.name == keyword)
		{
			if (node.point < 10)
				node.point++;	// At least 1 point shall be increased for complete HIT
			else
				node.point += (node.point * 0.1);
			return node.point;
		}
	}

	for (Node& node : twoBest[twoBestIndex])
	{
		if (node.name == keyword)
		{
			if (node.point < 10)
				node.point++;	// At least 1 point shall be increased for complete HIT
			else
				node.point += (node.point * 0.1);
			return node.point;
		}
	}

	return 0;
}

bool KeywordFixTool::checkSimilerHit(string keyword, int weekBestIndex, int twoBestIndex)
{
	for (Node& node : weekBest[weekBestIndex])
	{
		if (similer(node.name, keyword))
		{
			return true;
		}
	}

	for (Node& node : twoBest[twoBestIndex])
	{
		if (similer(node.name, keyword))
		{
			return true;
		}
	}

	return false;
}

void KeywordFixTool::insertNewKeyword(string keyword, int weekBestIndex, int twoBestIndex)
{
	weekBest[weekBestIndex].push_back({ keyword, UZ });
	twoBest[twoBestIndex].push_back({ keyword, UZ });
}

string KeywordFixTool::fixKeyword(string keyword, string day)
{
	int weekBestIndex = getWeekBestIndex(day);
	if (weekBestIndex == -1)
	{
		return "";
	}
	int twoBestIndex = getTwoBestIndex(weekBestIndex);

	int updatedPoint = 0;
	do
	{
		UZ++;
		updatedPoint = checkCompleteHit(keyword, weekBestIndex, twoBestIndex);
		if (updatedPoint != 0)
		{
			break;
		}

		bool similerHit = checkSimilerHit(keyword, weekBestIndex, twoBestIndex);
		if (similerHit)
		{
			break;
		}

		insertNewKeyword(keyword, weekBestIndex, twoBestIndex);
	} while (0);

	resetPoint(updatedPoint);
	rearrangeSlot(weekBestIndex, twoBestIndex);
	return keyword;
}

void KeywordFixTool::inputFile(string filename)
{
	ifstream fin;
	fin.open(filename.c_str());
	if (!fin.is_open())
	{
		std::cout << "error while open file" << std::endl;
		return;
	}

	string line;
	while (getline(fin, line))
	{
		int pos = line.rfind(' ');
		string keyword = line.substr(0, pos);
		string day = line.substr(pos + 1);
		string ret = fixKeyword(keyword, day);
		std::cout << ret << "\n";
	}

	fin.close();
}

void KeywordFixTool::initialize(SIMILARITY_ALGO algoNum)
{
	switch (algoNum)
	{
	case ALGO_LEVENSHTEIN:
		algo = new LevenshteinAlgo();
	default:
		break;
	}

	UZ = 9;

	for (int i = 0; i < WEEK_BEST_SLOT_NUM; i++)
	{
		weekBest[i].clear();
	}
	for (int i = 0; i < TWO_BEST_SLOT_NUM; i++)
	{
		twoBest[i].clear();
	}
}

void KeywordFixTool::clearAlgorithm()
{
	if (algo != nullptr)
	{
		delete algo;
		algo = nullptr;
	}
}
