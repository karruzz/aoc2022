#ifndef TASK14_H
#define TASK14_H

#include "task13.h"

template<>
class Task<14> : public ITask
{
public:
	Task()
	{
		m_testAnswers[Part::One] = "24";
		m_testAnswers[Part::Two] = "93";
	}

	std::string solve(std::istream& data, Part part) override
	{
		auto cave = parse(data);
		if (part == Part::Two) addFloor(cave);
		fillWithSand(cave);

		int sandsCount = 0;
		for (const auto& row : cave.m_map)
			sandsCount += std::count(row.cbegin(), row.cend(), 'o');

		return std::to_string(sandsCount);
	}

protected:
	std::string testData(Part part) override
	{
		return
			"498,4 -> 498,6 -> 496,6\n"
			"503,4 -> 502,4 -> 502,9 -> 494,9\n";
	}

private:

	struct Cave
	{
		VVC m_map;
		int m_minx, m_width, m_height;
	};

	void fillWithSand(Cave& cave)
	{
		while (addSandGrain(cave))
		{ }
	}

	void addFloor(Cave& cave)
	{
		cave.m_map[cave.m_height - 1] = VC(cave.m_width, '#');
	}

	bool addSandGrain(Cave& cave)
	{
		bool changed = false;

		PointXY grain = { .x = 500, .y = 0 };

		bool noSpaceInCave = cave.m_map[grain.y][grain.x - cave.m_minx] == 'o';
		if (noSpaceInCave) return false;

		bool grainMoved = false;
		bool outOfMap = false;
		do
		{
			std::vector<PointXY> possiblePositions = {
				{ .x = grain.x,     .y = grain.y + 1 },
				{ .x = grain.x - 1, .y = grain.y + 1 },
				{ .x = grain.x + 1, .y = grain.y + 1 }
			};

			grainMoved = false;
			for (const auto& p : possiblePositions)
			{
				if (p.y >= cave.m_height)
				{
					outOfMap = true;
					break;
				}
				if (cave.m_map[p.y][p.x - cave.m_minx] == '.')
				{
					grain = p;
					grainMoved = true;
					break;
				}
			}
		}
		while (grainMoved && !outOfMap);

		if (!outOfMap)
			cave.m_map[grain.y][grain.x - cave.m_minx] = 'o';

		return !outOfMap;
	}

	Cave parse(std::istream& data)
	{
		std::list<std::list<PointXY>> sandWalls;

		int minx = 10000, maxx = 0, miny = 0, maxy = 0;
		std::string line;
		while(std::getline(data, line))
		{
			cropp(line);

			std::list<PointXY> oneWall;
			std::list<std::string> coordinates = splitToList<std::string>(line, " -> ");
			for (const auto& coordinate : coordinates)
			{
				std::vector<int> point = splitToVec<int>(coordinate, ",");
				PointXY p = { .x = point[0], .y = point[1] };
				oneWall.push_back(p);
				if (p.x > maxx) maxx = p.x;
				if (p.x < minx) minx = p.x;
				if (p.y > maxy) maxy = p.y;
			}
			sandWalls.push_back(oneWall);
		}

		maxy += 3, maxx += 200, minx -= 200;
		int width  = maxx - minx;
		int height = maxy - miny;

		Cave cave = {
			.m_map    = VVC(height, VC(width, '.')),
			.m_minx   = minx,
			.m_width  = width,
			.m_height = height
		};

		for (const auto& wall : sandWalls)
		{
			PointXY prevPoint = wall.front();
			for (auto point : wall)
			{
				PointXY tmp = point;
				if (prevPoint.x == point.x)
				{
					if (prevPoint.y > point.y) swap(prevPoint, point);
					for (int i = prevPoint.y; i <= point.y; ++i)
						cave.m_map[i][point.x - minx] = '#';
				}
				else
				{
					if (prevPoint.x > point.x) swap(prevPoint, point);
					for (int i = prevPoint.x; i <= point.x; ++i)
						cave.m_map[point.y][i - minx] = '#';
				}
				prevPoint = tmp;
			}
		}

		return cave;
	}
};


#endif