#ifndef TASK17_H
#define TASK17_H

#include "task16.h"

template<>
class Task<17> : public ITask
{
public:
	Task() : m_shapes(initShapes())
	{
		m_testAnswers[Part::One] = "3068";
		m_testAnswers[Part::Two] = "1514285714288";
	}

	std::string solve(std::istream& data, Part part) override
	{
		std::deque<VC> chamber = std::deque<VC>(10, { '|','.','.','.','.','.','.','.','|'});
		chamber[0] = { '#','-','-','-','-','-','-','-','#'};

		int jetLinePosition = 0;
		std::string jetLine = parse(data);

		bool patternInited = false;
		std::deque<VC> pattern(300);

		const int64_t ROCKS_MAX = (part == Part::One) ? 2023 : 1000000000001;
		int64_t rock = 0, linesPoped = 0;

		int64_t patternBeginLine = 0, patternBeginRock = 0;
		int64_t patternPeriodLines = 0, patternPeriodRocks = 0;
		bool patternApplied = false;
		while (rock < ROCKS_MAX)
		{
			for (Shape shape : m_shapes)
			{
				++rock;
				if (rock >= ROCKS_MAX)
					break;

				static const int64_t PERIOD_STABILIZED = 1000;
				if (!patternInited && linesPoped > PERIOD_STABILIZED)
				{
					std::copy(chamber.begin(), chamber.begin() + pattern.size(), pattern.begin());
					patternInited = true;
				}

				while (chamber.size() > (3 * pattern.size()))
				{
					chamber.pop_front();
					++linesPoped;

					if (patternInited && !patternApplied)
					{
						if (std::equal(pattern.begin(), pattern.end(), chamber.begin()))
						{
							if (patternBeginLine == 0)
							{
								patternBeginLine = linesPoped;
								patternBeginRock = rock;
							}
							else
							{
								if (patternPeriodRocks == 0)
								{
									patternPeriodLines = linesPoped - patternBeginLine;
									patternPeriodRocks = rock - patternBeginRock;
								}
							}
						}
					}
				}

				if (patternPeriodRocks != 0 && !patternApplied)
				{
					patternApplied = true;
					int64_t addPeriods = (ROCKS_MAX - rock - 10LL * patternPeriodRocks) / patternPeriodRocks;
					linesPoped += patternPeriodLines * addPeriods;
					rock += patternPeriodRocks * addPeriods;
				}


				shape.m_position.x = 3, shape.m_position.y = findTop(chamber) + 4;

				int newHeight = shape.m_position.y + shape.height();
				while (newHeight > chamber.size())
					chamber.push_back({ '|','.','.','.','.','.','.','.','|'});

				while (true)
				{
					int shift = (jetLine[jetLinePosition] == '>') ? 1 : -1;
					++jetLinePosition;
					if (jetLinePosition >= jetLine.size()) jetLinePosition = 0;

					shape.m_position.x += shift;
					if (collides(chamber, shape))
						shape.m_position.x -= shift;

					--shape.m_position.y;
					if (collides(chamber, shape))
					{
						++shape.m_position.y;
						break;
					}
				}

				addShape(chamber, shape);
			}
		}

		return std::to_string(findTop(chamber) + linesPoped);
	}

protected:
	std::string testData(Part part) override
	{
		return
			">>><<><>><<<>><>>><<<>>><<<><<<>><>><<>>\n";
	}

private:
	struct Shape
	{
		VVC m_points;
		PointXY m_position;

		int height() const { return m_points.size(); }
		int width()  const { return m_points[0].size(); }
	};

	const std::vector<Shape> m_shapes;


	void printChamber(const std::deque<VC>& chamber)
	{
		for (int i = chamber.size() - 1; i >= 0; --i)
			printOne(chamber[i]);
	}


	void addShape(std::deque<VC>& chamber, const Shape& shape)
	{
		for (int y = 0; y < shape.height(); ++y)
			for (int x = 0; x < shape.width(); ++x)
				if (shape.m_points[y][x] == '#')
					chamber[shape.m_position.y + y][shape.m_position.x + x] = shape.m_points[y][x];
	}


	bool collides(const std::deque<VC>& chamber, const Shape& shape)
	{
		for (int y = 0; y < shape.height(); ++y)
			for (int x = 0; x < shape.width(); ++x)
			{
				if (shape.m_points[y][x] != '#')
					continue;

				if (chamber[shape.m_position.y + y][shape.m_position.x + x] != '.')
					return true;
			}

		return false;
	}


	int64_t findTop(const std::deque<VC>& chamber)
	{
		for (int i = chamber.size() - 1; i >= 0; --i)
			if (std::any_of(chamber[i].cbegin(), chamber[i].cend(), [](int c) { return c == '#'; }))
				return i;

		assert(false);
	}


	std::vector<Shape> initShapes()
	{
		std::string figures =
			"####\n"
			"\n"
			".#.\n"
			"###\n"
			".#.\n"
			"\n"
			"###\n" // rotated vertically for a correct indexing
			"..#\n"
			"..#\n"
			"\n"
			"#\n"
			"#\n"
			"#\n"
			"#\n"
			"\n"
			"##\n"
			"##\n";

		std::stringstream ss(figures);

		std::vector<Shape> shapes;
		shapes.push_back(Shape());
		std::string line;
		while(std::getline(ss, line))
		{
			if (!line.empty())
				shapes.back().m_points.push_back(VC(line.begin(), line.end()));
			else
				shapes.push_back(Shape());
		}

		return shapes;
	}

	std::string parse(std::istream& data)
	{
		std::string jetPattern;
		std::getline(data, jetPattern);
		cropp(jetPattern);
		return jetPattern;
	}
};


#endif