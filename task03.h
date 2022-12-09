#ifndef TASK03_H
#define TASK03_H

#include "task02.h"

template<>
class Task<3> : public ITask
{
public:
	Task()
	{
		m_testAnswers[Part::One] = "157";
		m_testAnswers[Part::Two] = "70";
	}

	std::string solve(std::istream& data, Part p) override
	{
		parse(data);

		int sumOfPriorities = 0;
		if (p == Part::One)
		{
			for (const VC& bp: m_backpacks)
			{
				auto b = bp.begin();
				auto m = bp.begin() + bp.size() / 2;
				auto e = bp.end();
				sumOfPriorities += priority(intersection({ {b, m}, {m, e} }));
			}
		}
		else
		{
			for (int i = 0; i < m_backpacks.size(); i = i + 3)
				sumOfPriorities += priority(intersection({m_backpacks[i], m_backpacks[i + 1], m_backpacks[i + 2]}));
		}

		return std::to_string(sumOfPriorities);
	}

protected:
	VVC m_backpacks;

	std::string testData(Part part) override
	{
		return
			"vJrwpWtwJgWrhcsFMMfFFhFp\n"
			"jqHRNqRjqzjGDLGLrsFMfFZSrLrFZsSL\n"
			"PmmdzqPrVvPwwTWBwg\n"
			"wMqvLMZHhHMvwLHjbvcjnnSBnvTQFn\n"
			"ttgJtRGJQctTZtZT\n"
			"CrZsJsPPZsGzwwsLwLmpwMDw\n";
	}

	int priority(char item)
	{
		return item <= 'Z'
		     ? item - 'A' + 27
		     : item - 'a' + 1;
	}

	char intersection(VVC sets)
	{
		assert(sets.size() >= 2);
		for (auto& s: sets)
			std::sort(s.begin(), s.end());

		VC cross = sets[0];
		for (int i = 1; i < sets.size(); ++i)
		{
			VC tmp;
			std::set_intersection(cross.cbegin(), cross.cend(),
			                      sets[i].cbegin(), sets[i].cend(),
			                      std::back_inserter(tmp));
			cross = tmp;
		}
		return cross[0];
	}

	void parse(std::istream& data)
	{
		m_backpacks.clear();

		std::string backpack;
		while(std::getline(data, backpack))
		{
			cropp(backpack);
			m_backpacks.push_back({backpack.begin(), backpack.end()});
		}
	}
};


#endif