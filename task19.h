#ifndef TASK19_H
#define TASK19_H

#include "task18.h"

template<>
class Task<19> : public ITask
{
public:
	Task()
	{
		m_testAnswers[Part::One] = "33";
		m_testAnswers[Part::Two] = "3472";
	}

	std::string solve(std::istream& data, Part part) override
	{
		auto blueprints = parse(data, part);

		auto startTime = std::chrono::steady_clock::now();

		int minutes = (part == Part::One) ? 24 : 32;
		std::vector<std::future<int>> results;
		for (int i = 0; i < blueprints.size(); ++i)
			results.push_back(std::async(calculate, blueprints[i], minutes, i + 1));

		int answer = (part == Part::One) ? 0 : 1;
		for (int i = 0; i < results.size(); ++i)
		{
			int optimal = results[i].get();
			log(i + 1, ", optimal ", optimal);
			if (part == Part::One)
				answer += (i + 1) * optimal;
			else
				answer *= optimal;
		}

		auto now = std::chrono::steady_clock::now();
		log("Total elapsed, ms ", std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count());

		return std::to_string(answer);
	}

protected:
	std::string testData(Part part) override
	{
		return
			"Blueprint 1: Each ore robot costs 4 ore. Each clay robot costs 2 ore. Each obsidian robot costs 3 ore and 14 clay. Each geode robot costs 2 ore and 7 obsidian.\n"
			"Blueprint 2: Each ore robot costs 2 ore. Each clay robot costs 3 ore. Each obsidian robot costs 3 ore and 8 clay. Each geode robot costs 3 ore and 12 obsidian.\n";
	}

private:
	enum Mineral
	{
		Ore = 0,
		Clay,
		Obsidian,
		Geode,
		Count,
		None
	};

	struct Cost
	{
		VI m_cost = { 0, 0, 0, 0 };
	};

	struct Blueprint
	{
		int m_number;
		std::vector<Cost> m_robotCosts;
		VI m_robotCostsMax;
	};

	static int calculate(const Blueprint& bp, int minutes, int blueprintNumber)
	{
		struct Step
		{
			uint16_t m_robots[Mineral::Count];
			uint16_t m_buildingRobots[Mineral::Count];
			uint16_t m_minerals[Mineral::Count];
			uint16_t m_minute;
			uint16_t m_totalMinutes;

			int score() const
			{
				return m_minerals[Geode];
			}

			int scoreRobots() const
			{
				return m_robots[Geode];
			}

			int bestPossibleScore()
			{
				int leftMinutes = (m_totalMinutes - m_minute);
				int bestScore = score() + scoreRobots() * leftMinutes;

				// assume each next step will be produced 1 more score-robot
				int scoreMinerals = leftMinutes * (leftMinutes - 1) / 2;
				bestScore += scoreMinerals;
				return bestScore;
			}

			bool tooManyUnusedMinerals(const Blueprint& bp)
			{
				return m_minerals[Ore] > bp.m_robotCostsMax[Ore] * 4;
			}

			// doesn't work
			bool delayedBuildingFirstClayRobot(const Blueprint& bp)
			{
				return m_minerals[Ore] > bp.m_robotCosts[Clay].m_cost[Ore]
				    && (m_robots[Clay] == 0);
			}

			// doesn't work
			bool tooManyOreRobots()
			{
				return (m_robots[Clay] == 0 && m_robots[Ore] > 1)
				    || (m_robots[Clay] == 1 && m_robots[Ore] > 1)
				    || (m_robots[Clay] >  1 && (m_robots[Ore] > m_robots[Clay] + 1));
			}
		};

		Step startStep =
		{
			.m_robots = { 1, 0, 0, 0 },
			.m_buildingRobots = { 0, 0, 0, 0 },
			.m_minerals = { 0, 0, 0, 0 },
			.m_minute = 0,
			.m_totalMinutes = minutes
		};

		// std::atomic<int> optimalScore = 0;

		int optimalScore = 0;
		int iterations = 0;

		auto doStep = [&iterations, &optimalScore, &bp](Step start) -> std::list<Step>
		{
			std::list<Step> localdfs;

			localdfs.push_back(start);
			for (int j = 0; j < 10000 && !localdfs.empty(); ++j)
			{
				++iterations;
				Step step = localdfs.front();
				localdfs.pop_front();

				++step.m_minute;
				for (int i = 0; i < Mineral::Count; ++i)
				{
					step.m_minerals[i] += step.m_robots[i];
					if (step.m_buildingRobots[i] > 0)
					{
						step.m_robots[i] += step.m_buildingRobots[i];
						step.m_buildingRobots[i] = 0;
					}
				}

				if (step.tooManyUnusedMinerals(bp))
					continue;

				if (step.bestPossibleScore() < optimalScore)
					continue;

				if (step.score() > optimalScore)
					optimalScore = step.score();

				if (step.m_minute >= step.m_totalMinutes)
					continue;

				static const std::vector<int> empty = {};
				static const std::vector<int> geodeOnly = { Geode };
				static const std::vector<int> allRobots = { Ore, Clay, Obsidian, Geode };

				// last step we don't need to start production
				// the step before the last we can produce only Geode robots
				const std::vector<int>& robotsToProduce = (step.m_minute == step.m_totalMinutes - 1)
				                               ? empty : ((step.m_minute == step.m_totalMinutes - 2)
				                               ? geodeOnly : allRobots);

				for (int robotType : robotsToProduce)
				{
					bool canBuildThisRobotType = true;
					for (int i = 0; i < Mineral::Count; ++i)
						if (step.m_minerals[i] < bp.m_robotCosts[robotType].m_cost[i])
						{
							canBuildThisRobotType = false;
							break;
						}

					if (canBuildThisRobotType)
					{
						Step next = step;
						for (int i = 0; i < Mineral::Count; ++i)
							next.m_minerals[i] -= bp.m_robotCosts[robotType].m_cost[i];

						++next.m_buildingRobots[robotType];
						localdfs.push_back(next);
					}
				}

				localdfs.push_back(step);
			}
			return localdfs;
		};

		auto startTime = std::chrono::steady_clock::now();
		std::list<Step> dfs = { startStep };

		while(!dfs.empty())
		{
			Step s = dfs.front();
			dfs.pop_front();
			std::list<Step> nextSteps = doStep(s);
			for (const auto& next : nextSteps)
				dfs.push_back(next);

			// todo: the multithread approach works slower, maybe need a pool of threads
			// const int CPUS = 6;
			// std::list<Step> steps;
			// std::list<std::future<std::list<Step>>> results;

			// for (int i = 0; i < CPUS && !dfs.empty(); ++i)
			// {
			// 	++iterations;
			// 	steps.push_back(dfs.front());
			// 	dfs.pop_front();
			// }

			// for (const auto& step : steps)
			// 	results.push_back(std::async(std::launch::async, doStep, step));

			// for (auto& result : results)
			// {
			// 	std::list<Step> nextSteps = result.get();
			// 	for (const auto& next : nextSteps)
			// 		dfs.push_back(next);
			// }
		}

		auto now = std::chrono::steady_clock::now();
		int elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();

		log(blueprintNumber, ", elapsed, ms ", elapsed, "; iterations ", iterations);
		return optimalScore;
	}

	std::vector<Blueprint> parse(std::istream& data, Part part)
	{
		std::vector<Blueprint> blueprints;

		std::string line;
		while(std::getline(data, line))
		{
			cropp(line);

			static const std::regex sensorRegex("Blueprint ([-0-9]+): Each ore robot costs ([-0-9]+) ore. Each clay robot costs ([-0-9]+) ore. Each obsidian robot costs ([-0-9]+) ore and ([-0-9]+) clay. Each geode robot costs ([-0-9]+) ore and ([-0-9]+) obsidian.");
			std::smatch sm;
			std::regex_match(line, sm, sensorRegex);

			Blueprint bp;
			bp.m_number = convert<int>(sm[1].str());
			bp.m_robotCosts.resize(Mineral::Count);

			bp.m_robotCosts[Mineral::Ore].m_cost[Mineral::Ore] = convert<int>(sm[2].str());

			bp.m_robotCosts[Mineral::Clay].m_cost[Mineral::Ore] = convert<int>(sm[3].str());

			bp.m_robotCosts[Mineral::Obsidian].m_cost[Mineral::Ore]  = convert<int>(sm[4].str());
			bp.m_robotCosts[Mineral::Obsidian].m_cost[Mineral::Clay] = convert<int>(sm[5].str());

			bp.m_robotCosts[Mineral::Geode].m_cost[Mineral::Ore]      = convert<int>(sm[6].str());
			bp.m_robotCosts[Mineral::Geode].m_cost[Mineral::Obsidian] = convert<int>(sm[7].str());

			bp.m_robotCostsMax.resize(Mineral::Count, 0);
			for (const auto& cost : bp.m_robotCosts)
				for (int i = 0; i < cost.m_cost.size(); ++i)
					if (cost.m_cost[i] > bp.m_robotCostsMax[i])
					 	bp.m_robotCostsMax[i] = cost.m_cost[i];

			blueprints.push_back(bp);

			if (part == Part::Two && blueprints.size() >= 3)
				break;
		}

		return blueprints;
	}
};


#endif