#ifndef TASK02_H
#define TASK02_H

#include "task01.h"

template<>
class Task<2> : public ITask
{
public:
	Task()
	{
		m_testAnswers[Part::One] = "15";
		m_testAnswers[Part::Two] = "12";
	}

	std::string solve(std::istream& data, Part p) override
	{
		int score = 0;

		std::string game;
		while(std::getline(data, game))
		{
			cropp(game);
			VC round = split<char>(game, ' ');

			if (p == Part::One)
			{
				score += m_strategy.at(round[0]).at(round[1]) + m_shape.at(round[1]);
			}
			else
			{
				score += m_plan.at(round[1]);
				char choice = m_strategy2.at(round[1]).at(round[0]);
				score += m_shape.at(choice);
			}
		}

		return std::to_string(score);
	}

protected:
	std::string testData(Part part) override
	{
		return
			"A Y\n"
			"B X\n"
			"C Z\n";
	}

private:
	std::map<char, int> m_shape =
	{
		{ 'X', 1 },
		{ 'Y', 2 },
		{ 'Z', 3 }
	};

	std::map<char, int> m_plan =
	{
		{ 'X', 0 },  // lose
		{ 'Y', 3 },  // draw
		{ 'Z', 6 }   // win
	};

	std::map<char, std::map<char, int>> m_strategy =
	{
		{ 'A', {{'X', 3}, {'Y', 6}, {'Z', 0}} },
		{ 'B', {{'X', 0}, {'Y', 3}, {'Z', 6}} },
		{ 'C', {{'X', 6}, {'Y', 0}, {'Z', 3}} }
	};

	std::map<char, std::map<char, char>> m_strategy2 =
	{
		{ 'X', {{'A', 'Z'}, {'B', 'X'}, {'C', 'Y'}} },  // lose
		{ 'Y', {{'A', 'X'}, {'B', 'Y'}, {'C', 'Z'}} },  // draw
		{ 'Z', {{'A', 'Y'}, {'B', 'Z'}, {'C', 'X'}} }   // win
	};
};


#endif