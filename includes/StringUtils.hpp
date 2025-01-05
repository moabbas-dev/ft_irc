#ifndef STRINGUTILS_HPP
# define STRINGUTILS_HPP

# include <iostream>
# include <vector>
# include <sstream>

std::vector<std::string> split(const std::string& str, const std::string& delimiters);
bool hasDuplicates(const std::string& str);
std::string getNumberAsString(unsigned long long nb);
bool isValidPass(const std::string &pass);
bool hasUniqueCharsInEachString(const std::vector<std::string> &vec);

#endif