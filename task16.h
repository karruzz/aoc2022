#ifndef TASK16_H
#define TASK16_H

#include "task.h"

template<>
class Task<16> : public ITask
{
public:
	Task()
	{
		m_testAnswers[Part::One] = "26";
	}

protected:
	std::string testData(Part part) override
	{
		return
			"Valve AA has flow rate=0; tunnels lead to valves DD, II, BB\n"
			"Valve BB has flow rate=13; tunnels lead to valves CC, AA\n"
			"Valve CC has flow rate=2; tunnels lead to valves DD, BB\n"
			"Valve DD has flow rate=20; tunnels lead to valves CC, AA, EE\n"
			"Valve EE has flow rate=3; tunnels lead to valves FF, DD\n"
			"Valve FF has flow rate=0; tunnels lead to valves EE, GG\n"
			"Valve GG has flow rate=0; tunnels lead to valves FF, HH\n"
			"Valve HH has flow rate=22; tunnel leads to valve GG\n"
			"Valve II has flow rate=0; tunnels lead to valves AA, JJ\n"
			"Valve JJ has flow rate=21; tunnel leads to valve II\n";
	}

private:

	std::list<Sensor> parse(std::istream& data)
	{
		std::list<Sensor> sensors;

		std::string line;
		while(std::getline(data, line))
		{
			cropp(line);

			static const std::regex sensorRegex("Sensor at x=([-0-9]+), y=([-0-9]+): closest beacon is at x=([-0-9]+), y=([-0-9]+)");
			std::smatch sm;
			std::regex_match(line, sm, sensorRegex);
			sensors.push_back({ .m_position = { .x = convert<int>(sm[1].str()), .y = convert<int>(sm[2].str()) }
			                  , .m_beacon   = { .x = convert<int>(sm[3].str()), .y = convert<int>(sm[4].str()) } });
		}

		return sensors;
	}
};


#endif