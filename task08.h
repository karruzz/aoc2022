#ifndef TASK08_H
#define TASK08_H

#include "task07.h"

template<>
class Task<8> : public ITask
{
public:
	Task()
	{
		m_testAnswers[Part::One] = "21";
		m_testAnswers[Part::Two] = "8";
	}

	std::string solve(std::istream& data, Part p) override
	{
		auto forest = parse(data);
		int visibleCount = 0, maxScore = 0;
		solve(forest, visibleCount, maxScore);
		return std::to_string(p == Part::One ? visibleCount : maxScore);
	}

protected:
	std::string testData(Part part) override
	{
		return
			"30373\n"
			"25512\n"
			"65332\n"
			"33549\n"
			"35390\n";
	}

private:
	VVI parse(std::istream& data)
	{
		VVI forest;

		std::string line;
		while(std::getline(data, line))
		{
			cropp(line);
			VI row;
			std::transform(line.cbegin(), line.cend(), std::back_inserter(row), [](char c) { return c - '0'; });
			forest.push_back(row);
		}

		return forest;
	}

	void solve(const VVI& forest, int& visibleCount, int& maxScore)
	{
		int rows = forest.size(), cols = forest[0].size();
		VVI visib(rows, VI(cols)), score(rows, VI(cols));

		enum Direction
		{
			Left, Right, Top, Bottom
		};

		auto visibility = [&forest, cols, rows](int x, int y, Direction d) -> std::tuple<bool, int>
		{
			int score = 0, current = forest[y][x];
			while (x > 0 && x < (cols - 1) && y > 0 && y < (rows - 1))
			{
				switch (d)
				{
					case (Left):    --x; break;
					case (Right):   ++x; break;
					case (Top):     --y; break;
					case (Bottom):  ++y; break;
				}

				++score;
				if (forest[y][x] >= current) return { false, score };
			}

			return { true, score };
		};

		for (int y = 0; y < rows; ++y)
		{
			for (int x = 0; x < cols; ++x)
			{
				auto left  = visibility(x, y, Left);
				auto right = visibility(x, y, Right);
				auto top   = visibility(x, y, Top);
				auto bot   = visibility(x, y, Bottom);

				visib[y][x] = std::get<0>(left) || std::get<0>(right) || std::get<0>(top) || std::get<0>(bot);
				score[y][x] = std::get<1>(left) *  std::get<1>(right) *  std::get<1>(top) *  std::get<1>(bot);
			}
		}

		for (const auto& vec : visib)
			visibleCount = std::accumulate(vec.cbegin(), vec.cend(), visibleCount);

		maxScore = score[0][0];
		for (const auto& v : score)
		{
			int maxLocal = *std::max_element(v.begin(), v.end());
			maxScore = (maxLocal > maxScore) ? maxLocal : maxScore;
		}
	}
};


#endif