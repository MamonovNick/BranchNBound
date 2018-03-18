#pragma once
#include <string>

//Class for define single arc in graph
class Arc
{
public:
	int first;
	int second;
	bool is_include;

	Arc() : first(-1), second(-1), is_include(false){};
	Arc(int f, int s, bool t) : first(f), second(s), is_include(t) {};
	~Arc() = default;
	Arc* clone(bool);
	std::string toString() { return (is_include ? "[" : "*[") + std::to_string(first) + " , " + std::to_string(second) + "]"; }
};

inline Arc* Arc::clone(bool t = false)
{
	return new Arc(first, second, t);
}
