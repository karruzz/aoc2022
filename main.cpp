#include "tools.h"

#define DAYS 9
#include taskfile(DAYS)

std::vector<std::unique_ptr<ITask>> tasks(DAYS + 1);

template<int Day>
constexpr void registerTasks()
{
	if (tasks.empty()) tasks.resize(Day + 1);
	tasks[Day] = std::unique_ptr<ITask>(new Task<Day>());
	registerTasks<Day - 1>();
}

template<>
constexpr void registerTasks<0>() { }

int main(int argc, char* argv[])
{
	registerTasks<DAYS>();
	//tasks[DAYS] = std::unique_ptr<ITask>(new Task<DAYS>());

	int day =           (argc > 1) ? convert<int>(argv[1]) : DAYS;
	std::string input = (argc > 2) ? argv[2] : getFilename(day);

	log("Day ", day);
	for (const auto p : { Part::One, Part::Two })
	{
		if (!tasks[day]->test(p))
		{
			log((int)p, ": Test failed");
			break;
		}

		std::fstream data(input);
		log((int)p, ": Test passed, answer: ", tasks[day]->solve(data, p));
	}

	return 0;
}
