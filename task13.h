#ifndef TASK13_H
#define TASK13_H

#include "task12.h"

template<>
class Task<13> : public ITask
{
public:
	Task()
	{
		m_testAnswers[Part::One] = "13";
		m_testAnswers[Part::Two] = "140";
	}

	std::string solve(std::istream& data, Part part) override
	{
		auto isRightOrder = [this](std::string left, std::string right) -> bool
		{
			int posLeft = 0, posRight = 0;
			return (checkCorrectness(left, posLeft, right, posRight) == State::Correct);
		};

		int result = 0;
		auto packets = parse(data);

		if (part == Part::One)
		{
			for (int i = 0; i < packets.size() / 2; ++i)
				result += isRightOrder(packets[2 * i], packets[2 * i + 1]) ? (i + 1) : 0;
		}
		else
		{
			std::list<std::string> sorted = { "[[2]]", "[[6]]" };

			for (const auto& packet : packets)
			{
				auto it = sorted.begin();
				for (; it != sorted.end(); ++it)
				{
					if (isRightOrder(packet, *it))
					{
						sorted.insert(it, packet);
						break;
					}
				}

				if (it == sorted.end())
					sorted.insert(it, packet);
			}

			result = 1;
			int index = 1;
			for (const auto& packet : sorted)
			{
				if (packet == "[[2]]" || packet == "[[6]]")
					result *= index;
				++index;
			}
		}

		return std::to_string(result);
	}

protected:
	std::string testData(Part part) override
	{
		return
			"[1,1,3,1,1]\n"
			"[1,1,5,1,1]\n"
			"\n"
			"[[1],[2,3,4]]\n"
			"[[1],4]\n"
			"\n"
			"[9]\n"
			"[[8,7,6]]\n"
			"\n"
			"[[4,4],4,4]\n"
			"[[4,4],4,4,4]\n"
			"\n"
			"[7,7,7,7]\n"
			"[7,7,7]\n"
			"\n"
			"[]\n"
			"[3]\n"
			"\n"
			"[[[]]]\n"
			"[[]]\n"
			"\n"
			"[1,[2,[3,[4,[5,6,7]]]],8,9]\n"
			"[1,[2,[3,[4,[5,6,0]]]],8,9]\n";
	}

private:
	enum State
	{
		Unclear,
		Correct,
		Incorrect
	};


	size_t digitWidth(const std::string& str, int pos)
	{
		int end = 0;
		while (std::isdigit(str[pos]))
			++end, ++pos;

		return end;
	}


	void singleDigitToList(std::string& str, int pos)
	{
		str.insert(pos + digitWidth(str, pos), 1, ']');
		str.insert(pos, 1, '[');
	}


	void next(const std::string& str, int& pos)
	{
		++pos;

		while(std::isdigit(str[pos]))
			++pos;

		if (str[pos] == ',')
			++pos;
	}


	int getDigit(const std::string& str, int pos)
	{
		size_t width = digitWidth(str, pos);
		return convert<int>(str.substr(pos, width));
	}


	State checkCorrectness(std::string& left, int& posLeft,
	                       std::string& right, int& posRight)
	{
		State state = State::Unclear;

		while (posLeft < left.size() && posRight < right.size())
		{
			if (left[posLeft] == '[' && right[posRight] == '[')
			{
				state = checkCorrectness(left, ++posLeft, right, ++posRight);
				if (state == State::Unclear)
				{
					next(left, posLeft);
					next(right, posRight);
					continue;
				}

				return state;
			}
			else if (left[posLeft] == '[' && std::isdigit(right[posRight]))
			{
				singleDigitToList(right, posRight);
				continue;
			}
			else if (std::isdigit(left[posLeft]) && right[posRight] == '[')
			{
				singleDigitToList(left, posLeft);
				continue;
			}

			if (std::isdigit(left[posLeft]) && right[posRight] == ']')
			{
				return State::Incorrect;
			}
			else if (left[posLeft] == ']' && std::isdigit(right[posRight]))
			{
				return State::Correct;
			}
			else if (left[posLeft] == ']' && right[posRight] == ']')
			{
				return State::Unclear;
			}
			else if (left[posLeft] == '[' && right[posRight] == ']')
			{
				return State::Incorrect;
			}
			else if (left[posLeft] == ']' && right[posRight] == '[')
			{
				return State::Correct;
			}
			else
			{
				int numberLeft  = getDigit(left, posLeft);
				int numberRight = getDigit(right, posRight);;

				if (numberLeft < numberRight)
				{
					return State::Correct;
				}
				else if (numberLeft > numberRight)
				{
					return State::Incorrect;
				}

				next(left, posLeft);
				next(right, posRight);
			}
		}

		return state;
	}


	std::vector<std::string> parse(std::istream& data)
	{
		std::vector<std::string> packets;

		std::string line;
		while(std::getline(data, line))
		{
			cropp(line);

			if (!line.empty())
				packets.push_back(line);
		}

		return packets;
	}
};


#endif
