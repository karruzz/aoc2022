#ifndef TASK09_H
#define TASK09_H

#include "task08.h"

template<>
class Task<9> : public ITask
{
public:
	Task()
	{
		m_testAnswers[Part::One] = "13";
		m_testAnswers[Part::Two] = "36";
	}

	std::string solve(std::istream& data, Part p) override
	{
		const int knotsNumber = (p == Part::One) ? 2 : 10;
		init(knotsNumber);

		auto movements = parse(data);
		for (const auto& m : movements)
			move(m);

		int64_t count = 0;
		for (const auto& vec : m_visitedByTail)
			count = std::accumulate(vec.cbegin(), vec.cend(), count);

		return std::to_string(count);
	}

protected:
	std::string testData(Part part) override
	{
		if (part == Part::One)
			return
				"R 4\n"
				"U 4\n"
				"L 3\n"
				"D 1\n"
				"R 4\n"
				"D 1\n"
				"L 5\n"
				"R 2\n";
		else
			return
				"R 5\n"
				"U 8\n"
				"L 8\n"
				"D 3\n"
				"R 17\n"
				"D 10\n"
				"L 25\n"
				"U 20\n";
	}

private:
	struct Movement
	{
		char m_dir;
		int  m_steps;
	};

	const int MAP_SIZE_X = 1000;
	const int MAP_SIZE_Y = 1000;

	const PointXY m_start = { .x = MAP_SIZE_X / 2, .y = MAP_SIZE_Y / 2 };

	VVI m_visitedByTail;
	std::vector<PointXY> m_knots;

	void printKnots()
	{
		VVC knots = VVC(MAP_SIZE_Y, VC(MAP_SIZE_X, '.'));

		knots[m_start.y][m_start.x] = 's';
		for (int i = m_knots.size() - 1; i >= 0 ; --i)
		{
			knots[m_knots[i].y][m_knots[i].x] = (i == 0) ? 'H' : (i + '0');
		}

		print(knots);
		log("");
	}

	void init(int knotsNumber)
	{
		m_knots = std::vector<PointXY>(knotsNumber, m_start);
		m_visitedByTail = VVI(MAP_SIZE_Y, VI(MAP_SIZE_X, 0));
		updateVisited();
	}

	void move(const Movement& m)
	{
		for (int i = 0; i < m.m_steps; ++i)
		{
			auto& head = m_knots[0];
			step(head, m.m_dir);

			for (int j = 0; j < m_knots.size() - 1; ++j)
			{
				if (!touched(m_knots[j], m_knots[j + 1]))
				{
					follow(m_knots[j], m_knots[j + 1]);
					updateVisited();
				}
			}
		}

		// log ("\nMove ", m.m_dir, ":", m.m_steps, "\n");
		// printKnots();
	}

	void step(PointXY& p, char direction)
	{
		switch(direction)
		{
			case('U'): --p.y; return;
			case('D'): ++p.y; return;
			case('L'): --p.x; return;
			case('R'): ++p.x; return;
		}
	}

	void follow(const PointXY& followed, PointXY& follower)
	{
		if (follower.x == followed.x) // same row
		{
			follower.y += (followed.y > follower.y) ? 1 : -1;
		}
		else if (follower.y == followed.y) // same column
		{
			follower.x += (followed.x > follower.x) ? 1 : -1;
		}
		else //diagonal
		{
			follower.y += (followed.y > follower.y) ? 1 : -1;
			follower.x += (followed.x > follower.x) ? 1 : -1;
		}
	}

	bool touched(const PointXY& p1, const PointXY& p2)
	{
		return std::abs(p1.x - p2.x) <= 1
		    && std::abs(p1.y - p2.y) <= 1;
	}

	void updateVisited()
	{
		const auto& tail = m_knots.back();
		m_visitedByTail[tail.y][tail.x] = 1;
	}

	std::list<Movement> parse(std::istream& data)
	{
		std::list<Movement> movements;

		static const std::regex moveRegex("([A-Z]) ([0-9]+)");
		std::smatch sm;

		std::string line;
		while(std::getline(data, line))
		{
			cropp(line);

			std::regex_match(line, sm, moveRegex);
			movements.push_back({ .m_dir = sm[1].str()[0]
			                    , .m_steps  = convert<int>(sm[2].str()) });
		}

		return movements;
	}
};


#endif