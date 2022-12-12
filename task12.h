#ifndef TASK12_H
#define TASK12_H

#include "task11.h"

template<>
class Task<12> : public ITask
{
public:
	Task()
	{
		m_testAnswers[Part::One] = "31";
		m_testAnswers[Part::Two] = "29";
	}

	std::string solve(std::istream& data, Part part) override
	{
		Heightmap heightMap = parse(data);
		return std::to_string(bfs(heightMap, part));
	}

protected:
	std::string testData(Part part) override
	{
		return
			"Sabqponm\n"
			"abcryxxl\n"
			"accszExk\n"
			"acctuvwj\n"
			"abdefghi\n";
	}

private:
	struct Heightmap
	{
		VVC m_levels;
		PointXY m_start, m_end;
		char level(const PointXY& p) const { return m_levels[p.y][p.x]; }
	};

	int bfs(const Heightmap& hm, Part part)
	{
		struct Step
		{
			PointXY m_point;
			char m_level;
			int m_index;
		};

		int height = hm.m_levels.size();
		int width  = hm.m_levels[0].size();
		VVI visited(height, VI(width, 0));

		const PointXY& begin = (part == Part::One) ? hm.m_start : hm.m_end;
		visited[begin.y][begin.x] = 1;

		std::list<Step> steps;
		steps.push_back({.m_point = begin, .m_level = hm.level(begin), .m_index = 0 });
		while (!steps.empty())
		{
			Step step = steps.front();
			steps.pop_front();

			PointXY left  = { .x = step.m_point.x - 1, .y = step.m_point.y };
			PointXY right = { .x = step.m_point.x + 1, .y = step.m_point.y };
			PointXY up    = { .x = step.m_point.x, .y = step.m_point.y - 1 };
			PointXY down  = { .x = step.m_point.x, .y = step.m_point.y + 1 };

			std::vector<PointXY> points = { left, right, up, down };
			for (const auto& p : points)
			{
				if (p.x < 0 || p.x >= width || p.y < 0 || p.y >= height) continue;
				if (visited[p.y][p.x]) continue;

				if (part == Part::One)
				{
					if (hm.level(p) > step.m_level + 1) continue;
					if (p == hm.m_end) return step.m_index + 1;
				}
				else
				{
					if (hm.level(p) < step.m_level - 1) continue;
					if (hm.level(p) == 'a') return step.m_index + 1;
				}

				visited[p.y][p.x] = 1;
				steps.push_back({.m_point = p,
				                 .m_level = hm.level(p),
				                 .m_index = step.m_index + 1 });
			}
		}

		return 0;
	}

	Heightmap parse(std::istream& data)
	{
		Heightmap heightMap;

		int y = 0;
		std::string line;
		while(std::getline(data, line))
		{
			cropp(line);

			int start = line.find('S');
			if (start != std::string::npos)
			{
				heightMap.m_start = { .x = start, .y = y };
				line[start] = 'a';
			}

			int end = line.find('E');
			if (end != std::string::npos)
			{
				heightMap.m_end = { .x = end, .y = y };
				line[end] = 'z';
			}

			heightMap.m_levels.push_back({ line.begin(), line.end() });
			++y;
		}

		return heightMap;
	}
};


#endif