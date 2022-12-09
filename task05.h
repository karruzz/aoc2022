#ifndef TASK05_H
#define TASK05_H

#include "task04.h"

template<>
class Task<5> : public ITask
{
public:
	Task()
	{
		m_testAnswers[Part::One] = "CMZ";
		m_testAnswers[Part::Two] = "MCD";
	}

	std::string solve(std::istream& data, Part p) override
	{
		parse(data);

		for (const auto& m : m_movements)
		{
			LC tmp;
			for (int i = 0; i < m.count; ++i)
			{
				tmp.push_back(m_crates[m.from].back());
				m_crates[m.from].pop_back();
			}

			if (p == Part::Two) tmp.reverse();
			std::copy(tmp.begin(), tmp.end(), std::back_inserter(m_crates[m.to]));
		}

		std::string result;
		for (const auto& crate : m_crates)
		{
			if (crate.empty()) break;
			result += crate.back();
		}

		return result;
	}

protected:
	std::string testData(Part part) override
	{
		return
			"    [D]    \n"
			"[N] [C]    \n"
			"[Z] [M] [P]\n"
			" 1   2   3 \n"
			"\n"
			"move 1 from 2 to 1\n"
			"move 3 from 1 to 3\n"
			"move 2 from 2 to 1\n"
			"move 1 from 1 to 2\n";
	}

private:
	struct Movement
	{
		int count = 0;
		int from  = 0;
		int to    = 0;
	};

	VLC m_crates;
	std::list<Movement> m_movements;

	void parse(std::istream& data)
	{
		m_crates.clear();
		m_crates.resize(9);
		m_movements.clear();

		std::string line;
		while(std::getline(data, line))
		{
			cropp(line);
			if (std::any_of(line.begin(), line.end(), ::isdigit)) break;

			for (int i = 1; i < line.size(); i = i + 4)
			{
				if (line[i] == ' ') continue;
				m_crates[i / 4].push_front(line[i]);
			}
		}

		static const std::regex moveRegex("move ([0-9]+) from ([0-9]+) to ([0-9]+)");
		std::smatch sm;
		while(std::getline(data, line))
		{
			cropp(line);
			if (line.empty()) continue;

			std::regex_match(line, sm, moveRegex);
			m_movements.push_back({ .count = convert<int>(sm[1].str())
			                      , .from  = convert<int>(sm[2].str()) - 1
			                      , .to    = convert<int>(sm[3].str()) - 1 });
		}
	}
};


#endif