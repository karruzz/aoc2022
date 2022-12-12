#ifndef TASK10_H
#define TASK10_H

#include "task09.h"

template<>
class Task<10> : public ITask
{
public:
	Task()
	{
		m_testAnswers[Part::One] = "13140";
		m_testAnswers[Part::Two] =
			"##..##..##..##..##..##..##..##..##..##..\n"
			"###...###...###...###...###...###...###.\n"
			"####....####....####....####....####....\n"
			"#####.....#####.....#####.....#####.....\n"
			"######......######......######......####\n"
			"#######.......#######.......#######.....\n";
	}

	std::string solve(std::istream& data, Part part) override
	{
		std::unique_ptr<Handler> handler;
		switch (part)
		{
			case(Part::One) : handler = std::make_unique<Strength>(); break;
			case(Part::Two) : handler = std::make_unique<Screen>(); break;
		};

		auto instructions = parse(data);
		execute(instructions, *handler);
		return (*handler)();
	}

protected:
	std::string testData(Part part) override
	{
		return
			"addx 15\n"
			"addx -11\n"
			"addx 6\n"
			"addx -3\n"
			"addx 5\n"
			"addx -1\n"
			"addx -8\n"
			"addx 13\n"
			"addx 4\n"
			"noop\n"
			"addx -1\n"
			"addx 5\n"
			"addx -1\n"
			"addx 5\n"
			"addx -1\n"
			"addx 5\n"
			"addx -1\n"
			"addx 5\n"
			"addx -1\n"
			"addx -35\n"
			"addx 1\n"
			"addx 24\n"
			"addx -19\n"
			"addx 1\n"
			"addx 16\n"
			"addx -11\n"
			"noop\n"
			"noop\n"
			"addx 21\n"
			"addx -15\n"
			"noop\n"
			"noop\n"
			"addx -3\n"
			"addx 9\n"
			"addx 1\n"
			"addx -3\n"
			"addx 8\n"
			"addx 1\n"
			"addx 5\n"
			"noop\n"
			"noop\n"
			"noop\n"
			"noop\n"
			"noop\n"
			"addx -36\n"
			"noop\n"
			"addx 1\n"
			"addx 7\n"
			"noop\n"
			"noop\n"
			"noop\n"
			"addx 2\n"
			"addx 6\n"
			"noop\n"
			"noop\n"
			"noop\n"
			"noop\n"
			"noop\n"
			"addx 1\n"
			"noop\n"
			"noop\n"
			"addx 7\n"
			"addx 1\n"
			"noop\n"
			"addx -13\n"
			"addx 13\n"
			"addx 7\n"
			"noop\n"
			"addx 1\n"
			"addx -33\n"
			"noop\n"
			"noop\n"
			"noop\n"
			"addx 2\n"
			"noop\n"
			"noop\n"
			"noop\n"
			"addx 8\n"
			"noop\n"
			"addx -1\n"
			"addx 2\n"
			"addx 1\n"
			"noop\n"
			"addx 17\n"
			"addx -9\n"
			"addx 1\n"
			"addx 1\n"
			"addx -3\n"
			"addx 11\n"
			"noop\n"
			"noop\n"
			"addx 1\n"
			"noop\n"
			"addx 1\n"
			"noop\n"
			"noop\n"
			"addx -13\n"
			"addx -19\n"
			"addx 1\n"
			"addx 3\n"
			"addx 26\n"
			"addx -30\n"
			"addx 12\n"
			"addx -1\n"
			"addx 3\n"
			"addx 1\n"
			"noop\n"
			"noop\n"
			"noop\n"
			"addx -9\n"
			"addx 18\n"
			"addx 1\n"
			"addx 2\n"
			"noop\n"
			"noop\n"
			"addx 9\n"
			"noop\n"
			"noop\n"
			"noop\n"
			"addx -1\n"
			"addx 2\n"
			"addx -37\n"
			"addx 1\n"
			"addx 3\n"
			"noop\n"
			"addx 15\n"
			"addx -21\n"
			"addx 22\n"
			"addx -6\n"
			"addx 1\n"
			"noop\n"
			"addx 2\n"
			"addx 1\n"
			"noop\n"
			"addx -10\n"
			"noop\n"
			"noop\n"
			"addx 20\n"
			"addx 1\n"
			"addx 2\n"
			"addx 2\n"
			"addx -6\n"
			"addx -11\n"
			"noop\n"
			"noop\n"
			"noop\n";
	}

private:
	struct Instruction
	{
		std::string m_command;
		int m_argument;
	};

	class Handler
	{
	public:
		virtual std::string operator()() const = 0;
		virtual void update(int cycle, int x) = 0;
	};

	class Strength : public Handler
	{
	public:
		std::string operator()() const override
		{
			int strength = 0;
			for (const auto& ic : m_interestedCycles)
				strength += ic.first * ic.second;

			return std::to_string(strength);
		}

		void update(int cycle, int x) override
		{
			auto interestedCycleIterator = m_interestedCycles.find(cycle);
			if (interestedCycleIterator != m_interestedCycles.end())
				interestedCycleIterator->second = x;
		}

	private:
		std::map<int, int> m_interestedCycles = { {20, 0}, {60, 0}, {100, 0}, {140, 0}, {180, 0}, {220, 0} };
	};

	class Screen : public Handler
	{
	public:
		std::string operator()() const override
		{
			std::string screen;
			for (const auto& row : m_screen)
				screen += std::string(row.begin(), row.end()) + '\n';

			return screen;
		}

		void update(int cycle, int x) override
		{
			--x, --cycle;
			if (cycle >= m_width * m_height) return;

			m_sprite = x;
			int screenX = cycle % m_width;
			int screenY = cycle / m_width;

			bool litPixel = (screenX >= m_sprite && screenX < (m_sprite + m_spriteWidth));
			m_screen[screenY][screenX] = litPixel ? '#' : '.';
		}

	private:
		const int m_width = 40, m_height = 6, m_spriteWidth = 3;

		int m_sprite = 0;
		VVC m_screen = VVC(m_height, VC(m_width, '.'));
	};


	void execute(const std::list<Instruction>& instructions, Handler& handler)
	{
		int cycle = 1, x = 1;
		handler.update(cycle, x);
		for (const auto& i : instructions)
		{
			if (i.m_command == "noop")
			{
				++cycle;
				handler.update(cycle, x);
			}
			else if (i.m_command == "addx")
			{
				++cycle;
				handler.update(cycle, x);

				x += i.m_argument;
				++cycle;
				handler.update(cycle, x);
			}
		}
	}


	std::list<Instruction> parse(std::istream& data)
	{
		std::list<Instruction> instructions;

		std::string line;
		while(std::getline(data, line))
		{
			cropp(line);

			int argument = 0;
			std::string command = line.substr(0, 4);

			if (command == "addx")
				argument = convert<int>(line.substr(5));

			instructions.push_back({ .m_command = command
			                       , .m_argument = argument });
		}

		return instructions;
	}
};


#endif