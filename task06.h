#ifndef TASK06_H
#define TASK06_H

#include "task05.h"

template<>
class Task<6> : public ITask
{
public:
	Task()
	{
		m_testAnswers[Part::One] = "7";
		m_testAnswers[Part::Two] = "19";
	}

	std::string solve(std::istream& data, Part p) override
	{
		std::list<char> queue;
		std::map<char, int> map;

		std::string input;
		std::getline(data, input);

		int sequenceSize = (p == Part::One) ? 4 : 14;

		int i = 0;
		for (; i < input.size(); ++i)
		{
			if (queue.size() == sequenceSize)
			{
				if (map.size() == queue.size()) break;

				char back = queue.back();
				queue.pop_back();

				if (--map.at(back) == 0) map.erase(back);
			}

			queue.push_front(input[i]);
			map[input[i]]++;
		}

		return std::to_string(i);
	}

protected:
	std::string testData(Part part) override
	{
		return "mjqjpqmgbljsphdztnvjfqwrcgsmlb";
	}
};


#endif