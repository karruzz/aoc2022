#ifndef TASK18_H
#define TASK18_H

#include "task17.h"

template<>
class Task<18> : public ITask
{
public:
	Task()
	{
		m_testAnswers[Part::One] = "64";
		m_testAnswers[Part::Two] = "58";
	}

	std::string solve(std::istream& data, Part part) override
	{
		auto droplet = parse(data);

		if (part == Part::One)
		{
			return std::to_string(getSurface(droplet));
		}
		else
		{
			std::list<PointXYZ> surroundedCubes;

			int X = m_maxx + 3, Y = m_maxy + 3, Z = m_maxz + 3;
			VVVC volumeAroundDrop = VVVC(Y, VVC(Z, VC(X, '.')));
			for (const auto& cube : droplet)
				volumeAroundDrop[cube.y + 1][cube.z + 1][cube.x + 1] = '#';

			std::list<PointXYZ> leeWalk;
			PointXYZ start = { .x = 0, .y = 0, .z = 0 };
			leeWalk.push_back(start);

			surroundedCubes.push_back(start);
			volumeAroundDrop[start.x][start.y][start.z] = 'o';
			while (!leeWalk.empty())
			{
				PointXYZ p = leeWalk.front();
				leeWalk.pop_front();

				std::vector<PointXYZ> whereCouldGo = {
					{ .x = p.x + 1, .y = p.y,     .z = p.z     },
					{ .x = p.x - 1, .y = p.y,     .z = p.z     },
					{ .x = p.x,     .y = p.y + 1, .z = p.z     },
					{ .x = p.x,     .y = p.y - 1, .z = p.z     },
					{ .x = p.x,     .y = p.y,     .z = p.z + 1 },
					{ .x = p.x,     .y = p.y,     .z = p.z - 1 }
				};

				for (const auto& p : whereCouldGo)
				{
					if (p.x < 0  || p.y < 0  || p.z < 0 || p.x >= X || p.y >= Y || p.z >= Z)
						continue;

					if ( volumeAroundDrop[p.y][p.z][p.x] != '.')
						continue;

					volumeAroundDrop[p.y][p.z][p.x] = 'o';
					leeWalk.push_back(p);
					surroundedCubes.push_back(p);
				}
			}

			// for (const auto& vvc : volumeAroundDrop)
			// {
			// 	print(vvc);
			// 	log("---------------------------");
			// }

			return std::to_string(getSurface(surroundedCubes) - 2 * (X * Y  + X * Z + Y * Z));
		}
	}

protected:
	std::string testData(Part part) override
	{
		return
			"2,2,2\n"
			"1,2,2\n"
			"3,2,2\n"
			"2,1,2\n"
			"2,3,2\n"
			"2,2,1\n"
			"2,2,3\n"
			"2,2,4\n"
			"2,2,6\n"
			"1,2,5\n"
			"3,2,5\n"
			"2,1,5\n"
			"2,3,5\n";
	}

private:
	int m_maxx = 0, m_maxy = 0, m_maxz = 0;

	int getSurface(std::list<PointXYZ> figure)
	{
		auto vertexToSides = [](int x, int y, int z) -> std::vector<PointXYZ>
		{
			// multiplied by 2, to avoid using float comparisons
			x *= 2, y *= 2, z *= 2;
			return {
				{ .x  = x - 1, .y = y,     .z = z - 1 },
				{ .x  = x - 1, .y = y - 2, .z = z - 1 },
				{ .x  = x,     .y = y - 1, .z = z - 1 },
				{ .x  = x - 2, .y = y - 1, .z = z - 1 },
				{ .x  = x - 1, .y = y - 1, .z = z     },
				{ .x  = x - 1, .y = y - 1, .z = z - 2 }
			};
		};

		std::set<PointXYZ> disconnectedSides;
		for (const auto& cube : figure)
		{
			for (const auto& s : vertexToSides(cube.x, cube.y, cube.z))
			{
				auto sameSide = disconnectedSides.find(s);
				if (sameSide == disconnectedSides.end())
					disconnectedSides.insert(s);
				else
					disconnectedSides.erase(s);
			}
		}
		return disconnectedSides.size();
	}

	std::list<PointXYZ> parse(std::istream& data)
	{
		std::list<PointXYZ> droplet;

		std::string line;
		while(std::getline(data, line))
		{
			cropp(line);

			static const std::regex sensorRegex("([-0-9]+),([-0-9]+),([-0-9]+)");
			std::smatch sm;
			std::regex_match(line, sm, sensorRegex);

			PointXYZ cube = {
				.x = convert<int>(sm[1].str()),
				.y = convert<int>(sm[2].str()),
				.z = convert<int>(sm[3].str())
			};

			if (cube.x > m_maxx) m_maxx = cube.x;
			if (cube.y > m_maxy) m_maxy = cube.y;
			if (cube.z > m_maxz) m_maxz = cube.z;

			droplet.push_back(cube);
		}

		return droplet;
	}
};


#endif