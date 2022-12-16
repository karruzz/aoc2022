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

		m_isTest = true;
		bool result = (m_testAnswers.find(part) != m_testAnswers.end())
		           && (solve(ss, part) == m_testAnswers.at(part));

		m_isTest = false;
		return result;
	}

protected:
	virtual std::string testData(Part part = Part::One) = 0;
	std::map<Part, std::string> m_testAnswers;
	bool isTesting() { return m_isTest; }

private:
	bool m_isTest = false;
};


template<int N>
class Task : public ITask
{ };


#endif