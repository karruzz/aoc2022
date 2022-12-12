#ifndef TASK_H
#define TASK_H

#include "tools.h"

enum class Part
{
	One = 1,
	Two = 2
};


class ITask
{
public:
	virtual ~ITask() {}

	virtual std::string solve(std::istream& data, Part part) = 0;

	bool test(Part part)
	{
		std::stringstream ss(testData(part));
		return m_testAnswers.find(part) != m_testAnswers.end()
		    && solve(ss, part) == m_testAnswers.at(part);
	}

protected:
	virtual std::string testData(Part part = Part::One) = 0;
	std::map<Part, std::string> m_testAnswers;
};


template<int N>
class Task : public ITask
{ };


#endif