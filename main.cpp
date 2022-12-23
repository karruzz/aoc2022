#include "tools.h"

#define DAYS 16
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

	int day =           (argc > 1) ? convert<int>(argv[1]) : DAYS;
	std::string input = (argc > 2) ? argv[2] : getFilename(day);

	log("Day ", day);
	for (const auto part : { Part::One, Part::Two })
	{
		log("*** Part ", (int)part, " ***");
		if (!tasks[day]->test(part))
		{
			log("Test failed");
			break;
		}
		else
		{
			log("Test passed");
		}

		std::fstream data(input);
		log("Answer:\n", tasks[day]->solve(data, part));
	}

	return 0;
}
