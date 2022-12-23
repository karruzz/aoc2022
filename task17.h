#ifndef TASK17_H
#define TASK17_H

#include "task16.h"

template<>
class Task<17> : public ITask
{
public:
	Task()
	{
		init();

		m_testAnswers[Part::One] = "3068";
		m_testAnswers[Part::Two] = "1514285714288";
	}

	std::string solve(std::istream& data, Part part) override
	{
		VVC chamber = VVC(5, { '|','.','.','.','.','.','.','.','|'});
		chamber[0] = { '#','-','-','-','-','-','-','-','#'};

		int jetLinePosition = 0;
		std::string jetLine = parse(data);

		VI tops(1);

		int64_t rock = 0;
		int64_t ROCKS_MAX = (part == Part::One) ? 2023 : 50000000;
		while (rock < ROCKS_MAX)
		{
			for (Shape shape : m_shapes)
			{
				tops.resize(rock);
				tops[rock] = findTop(chamber);
				shape.m_position.x = 3, shape.m_position.y = tops[rock] + 4;

				++rock;
				if (rock >= ROCKS_MAX)
					break;

				int newHeight = shape.m_position.y + shape.height();
				if (newHeight >= chamber.size())
					chamber.resize(newHeight, { '|','.','.','.','.','.','.','.','|'});

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
				// printChamber(chamber);
			}
		}

		if (part == Part::One)
		{
			return std::to_string(findTop(chamber));
		}
		else
		{
			log("Finished ", chamber.size(), ", top size ", tops.size());
			VC start = chamber[1];

			for (int repeatPosition = 2; repeatPosition < chamber.size(); ++repeatPosition)
			{
				auto it = std::find(chamber.begin() + repeatPosition, chamber.end(), start);
				int repetition = 0;
				if (it != chamber.end())
				{
					repetition = std::distance(chamber.begin(), it);

					bool same = true;
					for (int i = 0; i < 100; ++i)
					{
						if (chamber[1 + i] != chamber[repetition + i])
						{
							if (i > 3)
							{
								log("** begin **");
								for (int j = 0; j <= i; ++j)
								{
									std::string line(chamber[1 + j].begin(), chamber[1 + j].end());
									log(line);
								}

								log("** repeat **: ", repetition);
								for (int j = 0; j <= i; ++j)
								{
									std::string line(chamber[repetition + j].begin(), chamber[repetition + j].end());
									log(line);
								}
								log();
							}
							same = false;
							break;
						}
					}

					if (same)
					{
						log("Repetition since ", repetition);
						return "";
					}

					repeatPosition = repetition + 1;
				}
				else
				{
					break;
				}
			}
			log("End ");
			return "";
		}
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

	const int ChamberWide = 7;
	std::vector<Shape> m_shapes;


	void printChamber(const VVC& chamber)
	{
		for (int i = chamber.size() - 1; i >= 0; --i)
		{
			for (char c : chamber[i])
				std::cout << c;
			std::cout << std::endl;
		}
		log("*********");
	}


	void addShape(VVC& chamber, const Shape& shape)
	{
		for (int y = 0; y < shape.height(); ++y)
			for (int x = 0; x < shape.width(); ++x)
				if (shape.m_points[y][x] == '#')
					chamber[shape.m_position.y + y][shape.m_position.x + x] = shape.m_points[y][x];
	}


	bool collides(const VVC& chamber, const Shape& shape)
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


	int64_t findTop(const VVC& chamber)
	{
		for (int i = chamber.size() - 1; i >= 0; --i)
			if (std::any_of(chamber[i].cbegin(), chamber[i].cend(), [](int c) { return c == '#'; }))
				return i;

		return -1;
	}


	void init()
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

		m_shapes.push_back(Shape());
		std::string line;
		while(std::getline(ss, line))
		{
			if (!line.empty())
				m_shapes.back().m_points.push_back(VC(line.begin(), line.end()));
			else
				m_shapes.push_back(Shape());
		}
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