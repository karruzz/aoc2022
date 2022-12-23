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
		auto valves = parse(data);
		int answer = 0;

		if (part == Part::One)
		{
			MISG optimal = findOptimalPath(valves, "AA", 30);
			answer = pathToReleased(valves, optimal);

			for (const auto& item : optimal)
				log(item.second, ": ", item.first);
			log();
		}
		else
		{
/*			MSI optimalTogether;
			std::string myPosition = "AA", elephantPosition = "AA";
			int myTime = 26, elephantTime = 26;
			auto valvesCopy = valves;

			while (true)
			{
				MISG myOptimal = findOptimalPath(valves, myPosition, myTime);
				std::string name = myOptimal.begin()->second;
				int minute = myOptimal.begin()->first;
				optimalTogether.insert({ name, minute });

				log("Me ", name, ":", minute);

				valves.at(name).m_rate = 0;
				myPosition = name;
				myTime = minute;

				if (allValvesOpened(valves))
					break;

				MISG elephantOptimal = findOptimalPath(valves, elephantPosition, elephantTime);
				name = elephantOptimal.begin()->second;
				minute = elephantOptimal.begin()->first;
				optimalTogether.insert({ name, minute });

				log("Eleph ", name, ":", minute);

				valves.at(name).m_rate = 0;
				elephantPosition = name;
				elephantTime = minute;

				if (allValvesOpened(valves))
					break;
			}

			answer = pathToReleased(valvesCopy, optimalTogether);*/
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
		MSI m_toValves; // name-distance
	};

	using Valves = std::map<std::string, Valve>;

	std::map<std::pair<std::string, std::string>, int> m_distancesCached;

	int pathToReleased(const Valves& valves, const MISG& path)
	{
		int released = 0;
		for (const auto& valve : path)
		{
			int miniteOpened = valve.first;
			std::string name = valve.second;
			released += valves.at(name).m_rate * miniteOpened;
		}
		return released;
	}

	int pathToReleased(const Valves& valves, const MSI& path)
	{
		int released = 0;
		for (const auto& valve : path)
		{
			std::string name = valve.first;
			int miniteOpened = valve.second;
			released += valves.at(name).m_rate * miniteOpened;
		}
		return released;
	}

	bool allValvesOpened(const Valves& valves)
	{
		int closedValves = 0;
		for (const auto& valve : valves)
		{
			if (valve.second.m_rate > 0)
				++closedValves;
		}

		log("Left valves: ", closedValves);
		return closedValves == 0;
	}

	MISG findOptimalPath(const Valves& valves, std::string startValve, int leftMinutes)
	{
		std::set<std::string> toVisit;
		for (const auto& valve : valves)
		{
			if (valve.second.m_rate > 0)
				toVisit.insert(valve.first);
		}

		struct Step
		{
			std::string m_name;
			std::set<std::string> m_toVisit;
			MISG m_visited;
			int m_released;
			int m_minute;
		};

		Step optimal;

		Step start =
		{
			.m_name = startValve,
			.m_toVisit = toVisit,
			.m_released = 0,
			.m_minute = leftMinutes
		};

		int maxReleased = 0;

		std::list<Step> bfs;
		bfs.push_back(start);

		int left = toVisit.size() + 1;
		auto startTime = std::chrono::steady_clock::now();
		while (!bfs.empty())
		{
			Step current = bfs.front();
			bfs.pop_front();
			current.m_released += valves.at(current.m_name).m_rate * current.m_minute;
			current.m_toVisit.erase(current.m_name);
			if (startValve != current.m_name)
				current.m_visited.insert({current.m_minute, current.m_name});

			if (current.m_toVisit.size() < left)
			{
				auto elapsed = std::chrono::steady_clock::now();
				left = current.m_toVisit.size();
				// std::cout << "Left " << left << ", elapsed " << std::chrono::duration_cast<std::chrono::seconds>(elapsed - startTime).count() << std::endl;
			}

			if (current.m_released > maxReleased)
			{
				maxReleased = current.m_released;
				optimal = current;
			}

			if (current.m_released * 1.3 < maxReleased)
				continue;

			std::map<int, std::string, std::greater<int>> prioritized;
			std::map<std::string, int> distances;
			for (std::string nextValve : current.m_toVisit)
			{
				int distance = dijkstraDistance(valves, current.m_name, nextValve);
				distances[nextValve] = distance;

				int hashCriteria = 1000 * valves.at(nextValve).m_rate / (distance * distance);
				while (prioritized.find(hashCriteria) != prioritized.end())
					++hashCriteria;
				prioritized[hashCriteria] = nextValve;
			}

			int j = 0;
			for (const auto& next : prioritized)
			{
				Step nextStep = current;
				nextStep.m_name = next.second;
				nextStep.m_minute = current.m_minute - distances.at(next.second) - 1;

				bfs.push_back(nextStep);

				++j;
				const int TakeOnlyFirstN = 3;
				if (j > TakeOnlyFirstN)
					break;
			}
		}

		return optimal.m_visited;
	}

	int cachedDistance(const std::string& from, const std::string& to)
	{
		auto it = m_distancesCached.find({from, to});
		if (it != m_distancesCached.end())
			return it->second;

		return -1;
	}

	void cacheDistance(const std::string& from, const std::string& to, int distance)
	{
		m_distancesCached.insert({{from, to}, distance});
		m_distancesCached.insert({{to, from}, distance});
	}

	int dijkstraDistance(const Valves& valves, const std::string& from, const std::string& to)
	{
		int cached = cachedDistance(from, to);
		if (cached != -1)
			return cached;

		MSI distances;
		MSB visited;

		const int infDistance = 100000;

		for (const auto& valve : valves)
		{
			std::string name = valve.first;
			distances[name] = infDistance;
			visited[name] = false;
		}

		distances.at(from) = 0;

		for (int i = 0; i < visited.size(); ++i)
		{
			int distance = infDistance;
			std::string next;
			for (const auto& d : distances)
			{
				if (!visited.at(d.first) && d.second < distance)
				{
					next = d.first;
					distance = d.second;
				}
			}

			if (distances.at(next) == infDistance)
				break;

			visited.at(next) = true;
			for (const auto& adjacentValve : valves.at(next).m_toValves)
			{
				std::string adjacentName = adjacentValve.first;
				int adjacentDistance = adjacentValve.second;

				int distanceToNext = distances.at(next) + adjacentDistance;
				if (distanceToNext < distances.at(adjacentName))
				{
					distances.at(adjacentName) = distanceToNext;
				}
			}
		}

		int distance = distances.at(to);
		cacheDistance(from, to, distance);
		return distance;
	}

	Valves parse(std::istream& data)
	{
		Valves valves;

		std::string line;
		while(std::getline(data, line))
		{
			cropp(line);

			static const std::regex sensorRegex("Valve ([A-Z]+) has flow rate=([0-9]+); tunnel[s]? lead[s]? to valve[s]? ([ ,A-Z]+)");
			std::smatch sm;
			std::regex_match(line, sm, sensorRegex);

			Valve valve = {
				.m_name = sm[1].str(),
				.m_rate = convert<int>(sm[2].str()),
			};

			VS connectedToValves = splitToVec<std::string>(sm[3].str(), ", ");
			for (const auto& to : connectedToValves)
				valve.m_toValves.insert({ to, 1 });

			valves.insert({ sm[1].str(), valve });
		}

		m_distancesCached.clear();

		return valves;
	}
};


#endif