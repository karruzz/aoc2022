#ifndef TASK07_H
#define TASK07_H

#include "task06.h"

template<>
class Task<7> : public ITask
{
public:
	Task()
	{
		m_testAnswers[Part::One] = "95437";
		m_testAnswers[Part::Two] = "24933642";
	}

	std::string solve(std::istream& data, Part p) override
	{
		Dir root = parse(data);
		if (p == Part::One)
		{
			const int64_t lessThan = 100000;
			int64_t size = 0;
			totalSizeOfLessThan(root, lessThan, size);
			return std::to_string(size);
		}
		else if (p == Part::Two)
		{
			const int64_t freeSpace = 70000000 - root.m_size;
			const int64_t needSpace = 30000000 - freeSpace;
			int64_t closest = root.m_size;
			findClosest(root, needSpace, closest);
			return std::to_string(closest);
		}

		return "";
	}

protected:
	std::string testData(Part part) override
	{
		return
			"$ cd /\n"
			"$ ls\n"
			"dir a\n"
			"14848514 b.txt\n"
			"8504156 c.dat\n"
			"dir d\n"
			"$ cd a\n"
			"$ ls\n"
			"dir e\n"
			"29116 f\n"
			"2557 g\n"
			"62596 h.lst\n"
			"$ cd e\n"
			"$ ls\n"
			"584 i\n"
			"$ cd ..\n"
			"$ cd ..\n"
			"$ cd d\n"
			"$ ls\n"
			"4060174 j\n"
			"8033020 d.log\n"
			"5626152 d.ext\n"
			"7214296 k\n";
	}

private:
	struct File
	{
		std::string m_name;
		int64_t m_size = 0;
	};

	struct Dir : File
	{
		std::list<Dir> m_dirs;
		std::list<File> m_files;
	};

	Dir m_root;

	void findClosest(Dir& root, int64_t need, int64_t& closest)
	{
		if (root.m_size > need && root.m_size < closest)
			closest = root.m_size;

		for (auto& dir: root.m_dirs)
			findClosest(dir, need, closest);
	}

	void totalSizeOfLessThan(Dir& root, int64_t lessThan, int64_t& size)
	{
		if (root.m_size <= lessThan)
			size += root.m_size;

		for (auto& dir: root.m_dirs)
			totalSizeOfLessThan(dir, lessThan, size);
	}

	void updateSize(Dir& root)
	{
		root.m_size = 0;

		for (const auto& file: root.m_files)
			root.m_size += file.m_size;

		for (auto& dir: root.m_dirs)
		{
			updateSize(dir);
			root.m_size += dir.m_size;
		}
	}

	void debugPrint(const Dir& root, int level = 0)
	{
		std::string prefix = std::string(level, ' ');
		log(prefix, "- ", root.m_name, " (dir, size=", root.m_size, ")");

		for (const auto& dir: root.m_dirs)
			debugPrint(dir, level + 2);

		prefix = std::string(level + 2, ' ');
		for (const auto& file: root.m_files)
			log(prefix, "- ", file.m_name, " (file, size=", file.m_size, ")");
	}

	Dir parse(std::istream& data)
	{
		Dir root;
		root.m_name = "/";

		std::stack<Dir*> walk;
		walk.push(&root);
		Dir* currentDir = walk.top();

		std::string line;
		while(std::getline(data, line))
		{
			cropp(line);

			bool isCommand = (line[0] == '$');
			bool isData = !isCommand;
			if (isCommand)
			{
				std::string command = line.substr(2, 2);
				if (command == "cd")
				{
					std::string cdArgument = line.substr(5);
					if (cdArgument == "/") continue;

					bool moveOut = (cdArgument == "..");
					bool moveIn = !moveOut;

					if (moveIn)
					{
						for (auto& dir: currentDir->m_dirs)
						{
							if (dir.m_name == cdArgument)
							{
								walk.push(&dir);
								break;
							}
						}
						currentDir = walk.top();
					}
					else if (moveOut)
					{
						walk.pop();
						currentDir = walk.top();
					}
				}
				else if (command == "ls")
				{
					continue;
				}
			}
			else if (isData)
			{
				bool isDir = line.substr(0, 3) == "dir";
				bool isFile = !isDir;

				if (isDir)
				{
					Dir newDir;
					newDir.m_name = line.substr(4);
					currentDir->m_dirs.push_back({ newDir });
				}
				else if (isFile)
				{
					static const std::regex fileRegex("([0-9]+) ([a-z.]+)");
					std::smatch sm;
					std::regex_match(line, sm, fileRegex);

					currentDir->m_files.push_back({ .m_name = sm[2].str(),
					                                .m_size = convert<int>(sm[1].str()) });
				}
			}
		}

		updateSize(root);

		return root;
	}
};


#endif