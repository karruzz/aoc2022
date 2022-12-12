#ifndef TASK11_H
#define TASK11_H

#include "task10.h"

template<>
class Task<11> : public ITask
{
public:
	Task()
	{
		m_testAnswers[Part::One] = "10605";
		m_testAnswers[Part::Two] = "2713310158";
	}

	std::string solve(std::istream& data, Part part) override
	{
		const int worryDiviser = (part == Part::One) ? 3 : 1;
		const int rounds = (part == Part::One) ? 20 : 10000;

		auto monkeys = parse(data, worryDiviser);

		int64_t leastCommonMultiple = std::accumulate(
			monkeys.cbegin(), monkeys.cend(), 1,
			[](int64_t lcm, const Monkey& monkey) { return lcm * monkey.m_testDiviser; }
		);

		for (int i = 0; i < rounds; ++i)
		{
			for (auto& monkey : monkeys)
			{
				LPI64 inspectResults = monkey.inspect();
				for (const auto& inspectResult : inspectResults)
				{
					int toMonkey = inspectResult.first;
					int64_t item = inspectResult.second;
					if (part == Part::Two)
						item %= leastCommonMultiple;
					monkeys[toMonkey].addItem(item);
				}
			}
		}

		std::vector<int64_t> inspects;
		for (const auto& monkey : monkeys)
			inspects.push_back(monkey.inspected());

		std::sort(inspects.begin(), inspects.end(), std::greater<int64_t>());
		return std::to_string(inspects[0] * inspects[1]);
	}

protected:
	std::string testData(Part part) override
	{
		return
			"Monkey 0:\n"
			"  Starting items: 79, 98\n"
			"  Operation: new = old * 19\n"
			"  Test: divisible by 23\n"
			"    If true: throw to monkey 2\n"
			"    If false: throw to monkey 3\n"
			"\n"
			"Monkey 1:\n"
			"  Starting items: 54, 65, 75, 74\n"
			"  Operation: new = old + 6\n"
			"  Test: divisible by 19\n"
			"    If true: throw to monkey 2\n"
			"    If false: throw to monkey 0\n"
			"\n"
			"Monkey 2:\n"
			"  Starting items: 79, 60, 97\n"
			"  Operation: new = old * old\n"
			"  Test: divisible by 13\n"
			"    If true: throw to monkey 1\n"
			"    If false: throw to monkey 3\n"
			"\n"
			"Monkey 3:\n"
			"  Starting items: 74\n"
			"  Operation: new = old + 3\n"
			"  Test: divisible by 17\n"
			"    If true: throw to monkey 0\n"
			"    If false: throw to monkey 1\n";
	}

private:
	using Func = std::function<int64_t(int64_t)>;

	class Monkey
	{
	public:
		int64_t m_worryDiviser = 0;
		int64_t m_testDiviser = 0;

		void setItems(const LI64& items) { m_items = items; }
		void setOperation(const Func& operation) { m_operation = operation; }
		void setTest(const Func& test) { m_test = test; }
		void addItem(int64_t item) { m_items.push_back(item); }

		LPI64 inspect()
		{
			LPI64 result;
			m_inspected += m_items.size();
			while (!m_items.empty())
			{
				int64_t item = m_items.front();
				m_items.pop_front();

				int64_t newItem = m_operation(item) / m_worryDiviser;
				int64_t monkey  = m_test(newItem);
				result.push_back({ monkey, newItem });
			}

			return result;
		}

		int64_t inspected() const { return m_inspected; }

	private:
		LI64 m_items;
		Func m_operation, m_test;
		int64_t m_inspected = 0;
	};

	std::vector<Monkey> parse(std::istream& data, int worryDiviser)
	{
		enum Lines
		{
			MonkeyId    = 0,
			Items       = 1,
			Operation   = 2,
			TestDiviser = 3,
			TestTrue    = 4,
			TestFalse   = 5,
			Empty       = 6,
			Count       = 7
		};

		std::vector<Monkey> monkeys;

		Monkey monkey;
		int lineNumber = 0, diviser = 0, monkeyTrue = 0, monkeyFalse = 0;
		std::string line;
		while(std::getline(data, line))
		{
			cropp(line);

			Lines lineType = (Lines)(lineNumber % Lines::Count);
			switch(lineType)
			{
				case (Lines::MonkeyId) : {
					monkey = Monkey();
					monkey.m_worryDiviser = worryDiviser;
				} break;

				case (Lines::Items) : {
					std::string items = line.substr(18);
					monkey.setItems(split<int64_t>(items, ", "));
				} break;

				case (Lines::Operation) : {
					char function   = line[23];
					std::string arg = line.substr(25);

					if (arg == "old")
					{
						if (function == '*')
							monkey.setOperation([](int64_t a) { return a * a; });
						else if (function == '+')
							monkey.setOperation([](int64_t a) { return a + a; });
					}
					else
					{
						int64_t b = convert<int>(arg);
						if (function == '*')
							monkey.setOperation([b](int64_t a) { return a * b; });
						else if (function == '+')
							monkey.setOperation([b](int64_t a) { return a + b; });
					}
				} break;

				case (Lines::TestDiviser) : {
					diviser = convert<int>(line.substr(21));
					monkey.m_testDiviser = diviser;
				} break;

				case (Lines::TestTrue) : {
					monkeyTrue = convert<int>(line.substr(29));
				} break;

				case (Lines::TestFalse) : {
					monkeyFalse = convert<int>(line.substr(30));
					monkey.setTest([diviser, monkeyTrue, monkeyFalse](int64_t a)
					{
						return (a % diviser == 0) ? monkeyTrue : monkeyFalse;
					});

					monkeys.push_back(monkey);
				} break;
			}

			++lineNumber;
		}

		return monkeys;
	}
};


#endif