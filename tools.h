#ifndef TOOLS_H
#define TOOLS_H

#include <algorithm>
#include <array>
#include <atomic>
#include <cassert>
#include <chrono>
#include <cmath>
#include <fstream>
#include <future>
#include <iomanip>
#include <iostream>
#include <list>
#include <memory>
#include <numeric>
#include <optional>
#include <queue>
#include <regex>
#include <set>
#include <stack>
#include <string>
#include <thread>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

/* helpful macros */

#define xstr(s) stringify(s)
#define stringify(s) #s

//#define taskfilecat(day) task0##day.h
#define taskfilecat(day) task##day.h
#define taskfile(day) xstr(taskfilecat(day))

/* helpful types */

using VC    = std::vector<char>;
using VVC   = std::vector<VC>;
using VVVC  = std::vector<VVC>;

using LC    = std::list<char>;
using VLC   = std::vector<LC>;

using VI    = std::vector<int>;
using LI    = std::list<int>;
using LI64  = std::list<int64_t>;
using VVI   = std::vector<VI>;
using LVI   = std::list<VI>;

using VS    = std::vector<std::string>;
using LS    = std::list<std::string>;

using PI    = std::pair<int, int>;
using LPI   = std::list<PI>;

using PI64  = std::pair<int64_t, int64_t>;
using LPI64 = std::list<PI64>;

using MSC   = std::map<std::string, char>;
using MSI   = std::map<std::string, int>;
using MIS   = std::map<int, std::string>;
using MII   = std::map<int, int>;
using UMII  = std::unordered_map<int, int>;
using MISG  = std::map<int, std::string, std::greater<int>>;

using MIIG  = std::map<int, int, std::greater<int>>;

using MSS   = std::map<std::string, std::string>;
using MSB   = std::map<std::string, bool>;
using MCI   = std::map<char, int>;

struct Point
{
	int row = 0;
	int col = 0;
};

struct PointXY
{
	int x = 0;
	int y = 0;

	bool operator==(const PointXY& p) const { return x == p.x && y == p.y; }
	bool operator!=(const PointXY& p) const { return !(*this == p); }

	bool operator<(const PointXY& p) const
	{
		if ( x != p.x ) return x < p.x;
		return y < p.y;
	}
};

struct PointXYZ
{
	int x = 0;
	int y = 0;
	int z = 0;

	bool operator==(const PointXYZ& p) const { return x == p.x && y == p.y && z == p.z; }
	bool operator!=(const PointXYZ& p) const { return !(*this == p); }

	bool operator<(const PointXYZ& p) const
	{
		if ( x != p.x ) return x < p.x;
		if ( y != p.y ) return y < p.y;
		return z < p.z;
	}
};

void swap(PointXY& p1, PointXY& p2)
{
	PointXY tmp = p1;
	p1 = p2;
	p2 = tmp;
}

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

// eliminate Windows/Linux endings differences
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
int64_t convert<int64_t>(const std::string& token) { return std::stoi(token); }

template<>
char convert<char>(const std::string& token) { return token[0]; }

template<template <typename> class Container, typename T>
static void print(const Container<Container<T>>& twoDimensionalContainer, int width = 0, char delimeter = 0)
{
	for (const auto& dimensionOne : twoDimensionalContainer)
	{
		for (const auto& t : dimensionOne)
		{
			if (width) std::cout << std::setw(width) << std::setfill('0');
			std::cout << t;
			if (delimeter) std::cout << delimeter;
		}
		std::cout << std::endl;
	}
}

template<template <typename> class Container, typename T>
static void printOne(const Container<T>& oneDimensionalContainer, int width = 0, char delimeter = 0)
{
	for (const auto& t : oneDimensionalContainer)
	{
		if (width) std::cout << std::setw(width) << std::setfill('0');
		std::cout << t;
		if (delimeter) std::cout << delimeter;
	}
	std::cout << std::endl;
}

template<typename T>
std::list<T> splitToList(const std::string& line, const std::string& delimeter)
{
	using S = std::string;
	auto gettoken = [](const S& s, size_t& begin, S& token, const S& delimeter) -> bool
	{
		size_t end = s.size();
		if (begin >= end) return false;

		size_t pos = s.find(delimeter, begin);
		if (pos != S::npos)
			end = pos;

		token = s.substr(begin, end - begin);
		begin = end + delimeter.size();
		return true;
	};

	size_t pos = 0;
	std::string token;
	std::list<T> tokens;
	while(gettoken(line, pos, token, delimeter))
		tokens.push_back(convert<T>(token));

	return tokens;
}

template<typename T>
std::vector<T> splitToVec(const std::string& line, const std::string& delimeter)
{
	std::list<T> tokens = splitToList<T>(line, delimeter);

	std::vector<T> tokensVec;
	for (const auto& token : tokens)
		tokensVec.push_back(token);

	return tokensVec;
}

#endif
