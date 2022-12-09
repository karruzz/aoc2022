#ifndef TASK04_H
#define TASK04_H

#include "task03.h"

template<>
class Task<4> : public ITask
{
public:
	Task()
	{
		m_testAnswers[Part::One] = "2";
		m_testAnswers[Part::Two] = "4";
	}

	std::string solve(std::istream& data, Part p) override
	{
		int crosses = 0;
		std::string schedule;
		while(std::getline(data, schedule))
		{
			cropp(schedule);
			bool criteria = (p == Part::One)
			              ? contain(parseLine(schedule))
			              : overlap(parseLine(schedule));
			if (criteria)
				++crosses;
		}

		return std::to_string(crosses);
	}

protected:
	std::string testData(Part part) override
	{
		return
			"2-4,6-8\n"
			"2-3,4-5\n"
			"5-7,7-9\n"
			"2-8,3-7\n"
			"6-6,4-6\n"
			"2-6,4-8\n";
	}

private:
	bool contain(const std::pair<VI, VI>& p)
	{
		VI set1 = p.first;
		VI set2 = p.second;

		if (set1.size() > set2.size())
			std::swap(set1, set2);

		std::sort(set1.begin(), set1.end());
		std::sort(set2.begin(), set2.end());

		VI diff;
		std::set_difference(set1.cbegin(), set1.cend(),
		                    set2.cbegin(), set2.cend(),
		                    std::back_inserter(diff));
		return diff.empty();
	}

	bool overlap(const std::pair<VI, VI>& p)
	{
		VI set1 = p.first;
		VI set2 = p.second;

		std::sort(set1.begin(), set1.end());
		std::sort(set2.begin(), set2.end());

		VI diff;
		std::set_intersection(set1.cbegin(), set1.cend(),
		                      set2.cbegin(), set2.cend(),
		                      std::back_inserter(diff));
		return !diff.empty();
	}

	std::pair<VI, VI> parseLine(const std::string& line)
	{
		static const std::regex base_regex("([0-9]+)-([0-9]+),([0-9]+)-([0-9]+)");

		auto section = [](std::string b, std::string e) -> VI
		{
			int sectionBegin = convert<int>(b);
			int sectionEnd   = convert<int>(e);

			VI ids;
			for (int i = sectionBegin; i <= sectionEnd; ++i)
				ids.push_back(i);

			return ids;
		};

		std::smatch sm;
		std::regex_match(line, sm, base_regex);
		return { section(sm[1].str(), sm[2].str()), section(sm[3].str(), sm[4].str()) };
	}
};


#endif