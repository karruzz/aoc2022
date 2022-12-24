#ifndef TASK16_H
#define TASK16_H

#include "task15.h"

template<>
class Task<16> : public ITask
{
public:
	Task()
	{
		m_testAnswers[Part::One] = "1651";
		m_testAnswers[Part::Two] = "1707";
	}

	std::string solve(std::istream& data, Part part) override
	{
		auto valvesAndAdjacence = parse(data);
		auto valves = std::get<0>(valvesAndAdjacence);
		auto adjacency = std::get<1>(valvesAndAdjacence);
		auto distances = floydWarshallDistances(adjacency);

		int answer = 0;
		if (part == Part::One)
		{
			MIIG optimal = findOptimalPath(valves, distances, {0}, 30);
			answer = convertPathToReleased(valves, optimal, 30);
		}
		else
		{
			MIIG optimal = findOptimalPath(valves, distances, {0, 0}, 26);
			answer = convertPathToReleased(valves, optimal, 26);
		}

		return std::to_string(answer);
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
	struct Valve
	{
		std::string m_name;
		int m_rate;
	};

	using Valves = std::map<int, Valve>;

	int convertPathToReleased(const Valves& valves, const MIIG& path, int totalMinutes, bool showLog = false)
	{
		int released = 0;
		for (const auto& valve : path)
		{
			int valveIndex = valve.first;
			int miniteOpened = valve.second;
			released += valves.at(valveIndex).m_rate * miniteOpened;

			if (showLog)
				log(valves.at(valveIndex).m_name,"-", valves.at(valveIndex).m_rate, ": ", totalMinutes - miniteOpened, ", ", miniteOpened, ", released - ", released);
		}
		return released;
	}

	MIIG findOptimalPath(const Valves& valves, const VVI& distances, VI startNodes, int leftMinutes, bool showLog = false)
	{
		std::set<int> toVisit;
		for (const auto& valve : valves)
		{
			if (valve.second.m_rate > 0)
				toVisit.insert(valve.first);
		}

		struct Step
		{
			VI m_index;
			std::set<int> m_toVisit;
			MIIG m_visited;
			int m_released;
			VI m_minute;
		};

		Step optimal;

		Step startStep =
		{
			.m_index = startNodes,
			.m_toVisit = toVisit,
			.m_released = 0,
			.m_minute = VI(startNodes.size(), leftMinutes)
		};

		int maxReleased = 0;

		std::list<Step> bfs;
		bfs.push_back(startStep);

		int leftValves = toVisit.size() + 1;
		auto startTime = std::chrono::steady_clock::now();
		int negatives = 0;
		while (!bfs.empty())
		{
			Step current = bfs.front();
			bfs.pop_front();

			for (int i = 0; i < current.m_index.size(); ++i)
			{
				if (current.m_toVisit.find(current.m_index[i]) != current.m_toVisit.end())
				{
					current.m_released += valves.at(current.m_index[i]).m_rate * current.m_minute[i];
					current.m_toVisit.erase(current.m_index[i]);
					current.m_visited.insert({current.m_index[i], current.m_minute[i]});
				}
			}

			if (current.m_toVisit.size() < leftValves)
			{
				leftValves = current.m_toVisit.size();
				if (showLog)
				{
					auto elapsed = std::chrono::steady_clock::now();
					std::cout << "Left " << leftValves << ", elapsed " << std::chrono::duration_cast<std::chrono::seconds>(elapsed - startTime).count() << std::endl;
				}
			}

			if (current.m_released > maxReleased)
			{
				maxReleased = current.m_released;
				optimal = current;
			}

			// potentially faulty solutions
			if (current.m_released * 1.3 < maxReleased)
				continue;

			for (int i = 0; i < current.m_index.size(); ++i)
			{
				int index = current.m_index[i];

				MIIG prioritized;
				for (int nextValve : current.m_toVisit)
				{
					int distance = distances[index][nextValve];

					// multiplied on 10000 to keep it int, so can be used as a key in a map
					int hashCriteria = 10000 * valves.at(nextValve).m_rate / (distance * distance);
					while (prioritized.find(hashCriteria) != prioritized.end())
						++hashCriteria;

					prioritized[hashCriteria] = nextValve;
				}

				int j = 0;
				for (const auto& next : prioritized)
				{
					Step nextStep = current;
					nextStep.m_index[i] = next.second;
					nextStep.m_minute[i] = current.m_minute[i] - distances[index][next.second] - 1;

					if (nextStep.m_minute[i] < 0)
						continue;

					bfs.push_back(nextStep);

					++j;
					const int TakeOnlyFirstN = 3;
					if (j >= TakeOnlyFirstN)
						break;
				}
			}
		}

		return optimal.m_visited;
	}

	VVI floydWarshallDistances(const VVI& input)
	{
		const int inf = 100000;
		VVI dist = input;

		for (auto& row : dist)
			for (auto& value : row)
				if (value == 0)
					value = inf;

		int V = input.size();
		for (int k = 0; k < V; k++)
			for (int i = 0; i < V; i++)
				for (int j = 0; j < V; j++)
					if (dist[i][j] > (dist[i][k] + dist[k][j])
					&& (dist[k][j] != inf && dist[i][k] != inf))
					{
						dist[i][j] = dist[i][k] + dist[k][j];
					}

		return dist;
	}

	std::tuple<Valves, VVI> parse(std::istream& data)
	{
		Valves valves;
		VVI adjacency;

		std::string line;
		int index = 0;
		MSI valveNameToIndex;

		auto generateIndex = [&index, &valveNameToIndex](std::string valve)
		{
			if (valveNameToIndex.find(valve) != valveNameToIndex.end()) return;

			valveNameToIndex.insert({valve, index});
			++index;
		};

		LS lines;
		while(std::getline(data, line))
		{
			cropp(line);
			lines.push_back(line);
		}

		generateIndex("AA"); // "AA" is always a start node (index 0)
		adjacency = VVI(lines.size(), VI(lines.size(), 0));
		for (const auto& line : lines)
		{
			static const std::regex sensorRegex("Valve ([A-Z]+) has flow rate=([0-9]+); tunnel[s]? lead[s]? to valve[s]? ([ ,A-Z]+)");
			std::smatch sm;
			std::regex_match(line, sm, sensorRegex);

			std::string valveName = sm[1].str();

			generateIndex(valveName);
			int valveIndex = valveNameToIndex.at(valveName);

			Valve valve = { .m_name = valveName, .m_rate = convert<int>(sm[2].str()) };
			valves.insert({valveIndex, valve});

			VS connectedToValves = splitToVec<std::string>(sm[3].str(), ", ");
			for (const auto& to : connectedToValves)
			{
				generateIndex(to);
				int toIndex = valveNameToIndex.at(to);
				adjacency[valveIndex][toIndex] = 1;
			}
		}

		return { valves, adjacency };
	}
};


#endif