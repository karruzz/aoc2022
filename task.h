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

	virtual std::string solve(std::istream& data, Part p) = 0;

	bool test(Part p)
	{
		std::stringstream ss(testData(p));
		return m_testAnswers.find(p) != m_testAnswers.end()
		    && solve(ss, p) == m_testAnswers.at(p);
	}

protected:
	virtual std::string testData(Part p = Part::One) = 0;
	std::map<Part, std::string> m_testAnswers;
};


template<int N>
class Task : public ITask
{ };


#endif