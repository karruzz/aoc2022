#ifndef TOOLS_H
#define TOOLS_H

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <memory>
#include <numeric>
#include <queue>
#include <regex>
#include <set>
#include <stack>
#include <string>
#include <tuple>
#include <vector>

/* helpful macros */

#define xstr(s) stringify(s)
#define stringify(s) #s

#define taskfilecat(day) task0##day.h
//#define taskfilecat(day) task##day.h
#define taskfile(day) xstr(taskfilecat(day))

/* helpful types */

using VC  = std::vector<char>;
using VVC = std::vector<VC>;

using LC  = std::list<char>;
using VLC = std::vector<LC>;

using VI  = std::vector<int>;
using VVI = std::vector<VI>;

using VS  = std::vector<std::string>;

using PI  = std::pair<int, int>;

using VS  = std::vector<std::string>;

using MSC = std::map<std::string, char>;
using MSI = std::map<std::string, int>;
using MCI = std::map<char, int>;

struct Point
{
	int row = 0;
	int col = 0;
};

struct PointXY
{
	int x = 0;
	int y = 0;
};

/* helpful function */

template<class... Args>
void log(Args... args)
{
	(std::cout << ... << args) << std::endl;
}

std::string getFilename(int day)
{
	std::stringstream ss;
	ss << "data/input" << std::setw(2) << std::setfill('0') << day << ".txt";
	return ss.str();
}

void cropp(std::string& line)
{
	if (!line.empty() && line.back() == '\r')
		line.pop_back();
}

template<typename T>
T convert(const std::string& token) { return T(); }

template<>
std::string convert<std::string>(const std::string& token) { return token; }

template<>
int convert<int>(const std::string& token) { return std::stoi(token); }

template<>
char convert<char>(const std::string& token) { return token[0]; }

template<template <typename> class Container, typename T>
static void print(const std::vector<Container<T>>& twoDimensionalContainer, int width = 0, char delimeter = 0)
{
	if (width) std::cout << std::setw(width) << std::setfill('0');

	for (const auto& dimensionOne : twoDimensionalContainer)
	{
		for (const auto& t : dimensionOne)
		{
			std::cout << t;
			if (delimeter) std::cout << delimeter;
		}
		std::cout << std::endl;
	}
}

template<typename T>
std::vector<T> split(const std::string& line, char delimeter)
{
	std::vector<T> tokens;

	std::istringstream ss(line);
	std::string token;

	while(std::getline(ss, token, delimeter))
		tokens.push_back(convert<T>(token));

	return tokens;
}

#endif