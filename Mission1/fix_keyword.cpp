#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

struct Node {
	string name;
	int point;

	bool operator<(const Node& other) const {
		return point < other.point;
	}
};

#define WEEK_BEST_SLOT_NUM 7
#define TWO_BEST_SLOT_NUM 2
#define MAX_KEYWORD_NUM_IN_SLOT 10
#define MAX_POINT 2100000000

vector<Node> weekBest[WEEK_BEST_SLOT_NUM];	// monday ~ sunday
vector<Node> twoBest[TWO_BEST_SLOT_NUM];	// weekday, weekend
int UZ = 9;

int levenshtein(const std::string& a, const std::string& b) {
	const size_t len_a = a.size();
	const size_t len_b = b.size();

	std::vector<std::vector<int>> d(len_a + 1, std::vector<int>(len_b + 1));

	for (size_t i = 0; i <= len_a; ++i) d[i][0] = i;
	for (size_t j = 0; j <= len_b; ++j) d[0][j] = j;

	for (size_t i = 1; i <= len_a; ++i) {
		for (size_t j = 1; j <= len_b; ++j) {
			if (a[i - 1] == b[j - 1])
				d[i][j] = d[i - 1][j - 1];
			else
				d[i][j] = 1 + std::min({ d[i - 1][j], d[i][j - 1], d[i - 1][j - 1] });
		}
	}
	return d[len_a][len_b];
}

bool similer(const std::string& a, const std::string& b) {
	if (a.empty() || b.empty()) return false;

	int dist = levenshtein(a, b);
	int max_len = std::max(a.length(), b.length());
	double similarity = 1.0 - (double)dist / max_len;
	
	int score = 1 + static_cast<int>(similarity * 99);
	
	if (score >= 80) return true;
	return false;
}

int getWeekBestIndex(string day)
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

int getTwoBestIndex(int weekBestIndex)
{
	if (weekBestIndex >= 0 && weekBestIndex <= 4) return 0;
	return 1;
}

void resetPoint(int newMaxPoint)
{
	if (UZ >= MAX_POINT || newMaxPoint >= MAX_POINT)
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

void rearrangeSlot(int weekBestIndex, int twoBestIndex)
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

int checkCompleteHit(string keyword, int weekBestIndex, int twoBestIndex)
{
	for (Node& node : weekBest[weekBestIndex])
	{
		if (node.name == keyword)
		{
			if (node.point < 10)
				node.point++;
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
				node.point++;
			else
				node.point += (node.point * 0.1);
			return node.point;
		}
	}

	return 0;
}

bool checkSimilerHit(string keyword, int weekBestIndex, int twoBestIndex)
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

void insertNewKeyword(string keyword, int weekBestIndex, int twoBestIndex)
{
	weekBest[weekBestIndex].push_back({ keyword, UZ });
	twoBest[twoBestIndex].push_back({ keyword, UZ });
}

string fixKeyword(string keyword, string day)
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

void inputFile(string filename)
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
		int pos = line.find(' ');
		string keyword = line.substr(0, pos);
		string day = line.substr(pos + 1);
		string ret = fixKeyword(keyword, day);
		std::cout << ret << "\n";
	}

	fin.close();
}

int main()
{
	inputFile("keyword_weekday_500.txt");
	return 0;
}