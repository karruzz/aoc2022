#ifndef TASK15_H
#define TASK15_H

#include "task14.h"

template<>
class Task<15> : public ITask
{
public:
	Task()
	{
		m_testAnswers[Part::One] = "26";
		m_testAnswers[Part::Two] = "56000011";
	}

	std::string solve(std::istream& data, Part part) override
	{
		auto sensors = parse(data);

		if (part == Part::One)
		{
			const int LINE = isTesting() ? 10 : 2000000;

			VI vi;
			int min = std::numeric_limits<int>::max(), max = std::numeric_limits<int>::min();
			std::list<PI> boundaries;
			for (const auto& s : sensors)
			{
				auto boundary = s.getCoverage(LINE);
				if (!boundary) continue;

				if (boundary->first < min) min = boundary->first;
				if (boundary->second > max) max = boundary->second;
				boundaries.push_back(*boundary);
			}

			VC vc(max - min + 1, '.');
			for (const auto& boundary : boundaries)
				for (int i = boundary.first; i <= boundary.second; ++i)
					vc[i - min] = '#';

			for (const auto& s : sensors)
				if (s.m_beacon.y == LINE && s.m_beacon.x >= min && s.m_beacon.x <= max)
					vc[s.m_beacon.x - min] = '.';

			return std::to_string(std::count(vc.cbegin(), vc.cend(), '#'));
		}
		else
		{
			const int LIMIT = isTesting() ? 20 : 4000000;

			auto checkPoint = [&sensors](int x, int y, int excludeSensor) -> bool
			{
				for (int i = 0; i < sensors.size(); ++i)
				{
					if (i == excludeSensor) continue;
					if (sensors[i].inside(x, y)) return false;
				}
				return true;
			};

			std::vector<PointXY> sides = {
				{ .x = 1,  .y = 1  },
				{ .x = 1,  .y = -1 },
				{ .x = -1, .y = 1  },
				{ .x = -1, .y = -1 }
			};

			for (int i = 0; i < sensors.size(); ++i)
			{
				// loop around boundaries of each sensor range:
				for (const auto& side : sides)
				{
					int m = sensors[i].m_manhattan;
					for (int j = 0; j <= m; ++j)
					{
						int64_t x = sensors[i].m_position.x + side.x * j;
						int64_t y = sensors[i].m_position.y + side.y * (m - j + 1);

						if (x < 0 || x >= LIMIT || y < 0 || y >= LIMIT) continue;

						if (checkPoint(x, y, i))
							return std::to_string(x * 4000000 + y);
					}
				}
			}

			return "";
		}
	}

protected:
	std::string testData(Part part) override
	{
		return
			"Sensor at x=2, y=18: closest beacon is at x=-2, y=15\n"
			"Sensor at x=9, y=16: closest beacon is at x=10, y=16\n"
			"Sensor at x=13, y=2: closest beacon is at x=15, y=3\n"
			"Sensor at x=12, y=14: closest beacon is at x=10, y=16\n"
			"Sensor at x=10, y=20: closest beacon is at x=10, y=16\n"
			"Sensor at x=14, y=17: closest beacon is at x=10, y=16\n"
			"Sensor at x=8, y=7: closest beacon is at x=2, y=10\n"
			"Sensor at x=2, y=0: closest beacon is at x=2, y=10\n"
			"Sensor at x=0, y=11: closest beacon is at x=2, y=10\n"
			"Sensor at x=20, y=14: closest beacon is at x=25, y=17\n"
			"Sensor at x=17, y=20: closest beacon is at x=21, y=22\n"
			"Sensor at x=16, y=7: closest beacon is at x=15, y=3\n"
			"Sensor at x=14, y=3: closest beacon is at x=15, y=3\n"
			"Sensor at x=20, y=1: closest beacon is at x=15, y=3\n";
	}

private:

	struct Sensor
	{
		PointXY m_position;
		PointXY m_beacon;
		int m_manhattan;

		std::optional<PI> getCoverage(int y) const
		{
			int diff = std::abs(m_position.y - y);
			if (diff > m_manhattan)
			{
				return std::optional<PI>();
			}
			else
			{
				int localCoverage = m_manhattan - diff;
				return PI(m_position.x - localCoverage, m_position.x + localCoverage);
			}
		}

		bool inside(int x, int y) const
		{
			int diff = std::abs(m_position.x - x) + std::abs(m_position.y - y);
			return diff <= m_manhattan;
		}
	};

	std::vector<Sensor> parse(std::istream& data)
	{
		std::vector<Sensor> sensors;

		std::string line;
		while(std::getline(data, line))
		{
			cropp(line);

			static const std::regex sensorRegex("Sensor at x=([-0-9]+), y=([-0-9]+): closest beacon is at x=([-0-9]+), y=([-0-9]+)");
			std::smatch sm;
			std::regex_match(line, sm, sensorRegex);

			int x1 = convert<int>(sm[1].str()), y1 = convert<int>(sm[2].str());
			int x2 = convert<int>(sm[3].str()), y2 = convert<int>(sm[4].str());

			sensors.push_back({ .m_position = { .x = x1, .y = y1 }
			                  , .m_beacon   = { .x = x2, .y = y2 }
			                  , .m_manhattan = std::abs(x1 - x2) + std::abs(y1 - y2) });
		}

		return sensors;
	}
};


#endif