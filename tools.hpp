#ifndef TOOLS_HPP
#define TOOLS_HPP 

#include <iostream>
#include <vector>

class Tools{
public:
	static std::vector<std::string> splitBySpace(std::string statement);
	static std::vector<std::string> split(std::string statement , const char* delimeters);
	static int findCharIndex(std::string s, char c);
	static bool isNumber(const std::string& s);
private:
	Tools();
};

#endif