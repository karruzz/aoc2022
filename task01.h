#ifndef TASK01_H
#define TASK01_H

#include "task.h"

template<>
class Task<1> : public ITask
{
public:
	Task()
	{
		m_testAnswers[Part::One] = "24000";
		m_testAnswers[Part::Two] = "45000";
	}

	std::string solve(std::istream& data, Part p) override
	{
		parse(data);
		std::sort(m_calories.begin(), m_calories.end(), std::greater<int>());
		return std::to_string((p == Part::One)
		                     ? m_calories[0]
		                     : m_calories[0] + m_calories[1] + m_calories[2]);
	}

protected:
	std::string testData(Part part) override
	{
		return
			"1000\n"
			"2000\n"
			"3000\n"
			"\n"
			"4000\n"
			"\n"
			"5000\n"
			"6000\n"
			"\n"
			"7000\n"
			"8000\n"
			"9000\n"
			"\n"
			"10000\n";
	}

private:
	VI m_calories;

	void parse(std::istream& data)
	{
		m_calories.clear();

		int calories = 0;
		std::string line;
		while(std::getline(data, line))
		{
			cropp(line);

			if (!line.empty())
			{
				calories += convert<int>(line);
			}
			else
			{
				m_calories.push_back(calories);
				calories = 0;
			}
		}
		m_calories.push_back(calories);
	}
};


#endif