#include "gmock/gmock.h"
#include "fix_keyword.h"
#include "levenshtein_algo.h"

class FixKeywordTestFixture : public ::testing::Test
{
protected:
	virtual void SetUp()
	{
		tool.initialize(ALGO_LEVENSHTEIN);
	}

	virtual void TearDown()
	{
		tool.clearAlgorithm();
	}

	KeywordFixTool tool;
};

TEST_F(FixKeywordTestFixture, ResetPointTest)
{
	tool.inputFile("keyword_weekday_500.txt");
	EXPECT_TRUE(tool.UZ > 9);

	tool.resetPoint(MAX_POINT);
	EXPECT_EQ(tool.UZ, 9);
}

TEST_F(FixKeywordTestFixture, WrongInputFileTest)
{
	tool.inputFile("keyword_weekday_5000000.txt");
	EXPECT_EQ(tool.UZ, 9);
}

TEST_F(FixKeywordTestFixture, WrongAlgoTest)
{
	tool.clearAlgorithm();
	tool.initialize(ALGO_END);
	EXPECT_EQ(nullptr, tool.algo);
}

TEST_F(FixKeywordTestFixture, WrongInputTest)
{
	string ret = tool.fixKeyword("text", "someday");
	EXPECT_EQ(ret, "");

	ret = tool.fixKeyword("text", "");
	EXPECT_EQ(ret, "");
}

TEST_F(FixKeywordTestFixture, CompleteHitTest)
{
	tool.fixKeyword("aaaaaa", "monday");
	tool.fixKeyword("bbbbbb", "tuesday");
	tool.fixKeyword("cccccc", "wednesday");
	tool.fixKeyword("dddddd", "thursday");
	tool.fixKeyword("eeeeee", "friday");
	tool.fixKeyword("ffffff", "saturday");
	tool.fixKeyword("gggggg", "sunday");

	int point = 0;
	tool.fixKeyword("aaaaaa", "monday");
	for (Node& node : tool.weekBest[0])
	{
		if (node.name == "aaaaaa")
		{
			point = node.point;
		}
	}
	EXPECT_EQ(point, 11);

	tool.fixKeyword("bbbbbb", "monday");
	for (Node& node : tool.twoBest[0])
	{
		if (node.name == "bbbbbb")
		{
			point = node.point;
		}
	}
	EXPECT_EQ(point, 12);

	tool.fixKeyword("gggggg", "sunday");
	for (Node& node : tool.weekBest[6])
	{
		if (node.name == "gggggg")
		{
			point = node.point;
		}
	}
	EXPECT_EQ(point, 17);
}

TEST_F(FixKeywordTestFixture, SimilarHitTest)
{
	tool.fixKeyword("aaaaaa", "monday");
	tool.fixKeyword("bbbbbb", "tuesday");
	tool.fixKeyword("cccccc", "wednesday");
	tool.fixKeyword("dddddd", "thursday");
	tool.fixKeyword("eeeeee", "friday");
	tool.fixKeyword("ffffff", "saturday");
	tool.fixKeyword("gggggg", "sunday");

	int point = 0;
	tool.fixKeyword("aaaaa", "monday");
	for (Node& node : tool.weekBest[0])
	{
		if (node.name == "aaaaaa")
		{
			point = node.point;
		}
	}
	EXPECT_EQ(point, 10);

	tool.fixKeyword("bbbbb", "monday");
	for (Node& node : tool.twoBest[0])
	{
		if (node.name == "bbbbbb")
		{
			point = node.point;
		}
	}
	EXPECT_EQ(point, 11);

	tool.fixKeyword("ggggg", "sunday");
	for (Node& node : tool.weekBest[6])
	{
		if (node.name == "gggggg")
		{
			point = node.point;
		}
	}
	EXPECT_EQ(point, 16);
}

TEST_F(FixKeywordTestFixture, NewInputTest)
{
	tool.fixKeyword("aaaaaa", "monday");
	tool.fixKeyword("bbbbbb", "tuesday");
	tool.fixKeyword("cccccc", "wednesday");
	tool.fixKeyword("dddddd", "thursday");
	tool.fixKeyword("eeeeee", "friday");
	tool.fixKeyword("ffffff", "saturday");
	tool.fixKeyword("gggggg", "sunday");

	int point = 0;
	tool.fixKeyword("abcdef", "monday");
	for (Node& node : tool.weekBest[0])
	{
		if (node.name == "aaaaaa")
		{
			point = node.point;
		}
	}
	EXPECT_EQ(point, 10);

	for (Node& node : tool.weekBest[0])
	{
		if (node.name == "abcdef")
		{
			point = node.point;
		}
	}
	EXPECT_EQ(point, 17);
}

TEST_F(FixKeywordTestFixture, CompleteHitAfterRearrangeTest)
{
	tool.fixKeyword("aaaaaa", "monday");
	tool.fixKeyword("bbbbbb", "tuesday");
	tool.fixKeyword("cccccc", "wednesday");
	tool.fixKeyword("dddddd", "thursday");
	tool.fixKeyword("eeeeee", "friday");
	tool.fixKeyword("ffffff", "saturday");
	tool.fixKeyword("gggggg", "sunday");

	tool.resetPoint(MAX_POINT);

	int point = 0;
	tool.fixKeyword("aaaaaa", "monday");
	for (Node& node : tool.weekBest[0])
	{
		if (node.name == "aaaaaa")
		{
			point = node.point;
		}
	}
	EXPECT_EQ(point, 2);

	tool.fixKeyword("bbbbbb", "monday");
	for (Node& node : tool.twoBest[0])
	{
		if (node.name == "bbbbbb")
		{
			point = node.point;
		}
	}
	EXPECT_EQ(point, 3);

	tool.fixKeyword("gggggg", "sunday");
	for (Node& node : tool.weekBest[6])
	{
		if (node.name == "gggggg")
		{
			point = node.point;
		}
	}
	EXPECT_EQ(point, 2);
}

TEST_F(FixKeywordTestFixture, AddToFullListTest)
{
	tool.fixKeyword("aaaaaa", "monday");
	tool.fixKeyword("bbbbbb", "monday");
	tool.fixKeyword("cccccc", "monday");
	tool.fixKeyword("dddddd", "monday");
	tool.fixKeyword("eeeeee", "monday");
	tool.fixKeyword("ffffff", "monday");
	tool.fixKeyword("gggggg", "monday");
	tool.fixKeyword("hhhhhh", "monday");
	tool.fixKeyword("iiiiii", "monday");
	tool.fixKeyword("jjjjjj", "monday");

	EXPECT_EQ(tool.weekBest[0].size(), 10);

	tool.fixKeyword("zzzzzz", "monday");

	EXPECT_EQ(tool.weekBest[0].size(), 10);

	int point = 0;
	for (Node& node : tool.weekBest[0])
	{
		if (node.name == "zzzzzz")
		{
			point = node.point;
		}
	}
	EXPECT_EQ(point, 20);
}

TEST_F(FixKeywordTestFixture, IntegratedTest)
{
	tool.inputFile("keyword_weekday_500.txt");

	EXPECT_TRUE(tool.UZ > 9);

	for (int i = 0; i < WEEK_BEST_SLOT_NUM; i++)
	{
		EXPECT_TRUE(tool.weekBest[i].size() <= 10);
	}
	for (int i = 0; i < TWO_BEST_SLOT_NUM; i++)
	{
		EXPECT_TRUE(tool.twoBest[i].size() <= 10);
	}

	int expected = tool.UZ + 1;
	string newKeyword = "new new text sample";
	tool.fixKeyword(newKeyword, "monday");

	EXPECT_TRUE(tool.weekBest[0].size() <= 10);

	int actual = 0;
	for (Node& node : tool.weekBest[0])
	{
		if (node.name == newKeyword)
		{
			actual = node.point;
		}
	}
	EXPECT_EQ(actual, expected);

	tool.fixKeyword(newKeyword, "monday");
	for (Node& node : tool.weekBest[0])
	{
		if (node.name == newKeyword)
		{
			actual = node.point;
		}
	}
	EXPECT_EQ(actual, expected * 1.1);
}