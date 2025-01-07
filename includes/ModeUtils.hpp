#ifndef MODEUTILS_HPP
# define MODEUTILS_HPP

#include <iostream>
#include <vector>
#include "StringUtils.hpp"
#include "Channel.hpp"
#include <sstream>

std::vector<char> getStringsSymbols(const std::string &modeStr);
bool checkDuplications(const std::vector<std::string> &modeSubStrs,
	const std::vector<char> &symbols);
bool isCorrectModeString(const std::string &modeString);
size_t getNbOfModeArguments(const std::string &modeString);
bool isValidLimitString(const std::string &str, Channel *channel);
std::string formatModeString(const std::string &modeStr);


#endif