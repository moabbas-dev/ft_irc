#include <iostream>
#include <vector>

static std::vector<std::string> split(const std::string& str, const std::string& delimiters)
{
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t end = str.find_first_of(delimiters);

    while (end != std::string::npos)
	{
        if (end != start)
            tokens.push_back(str.substr(start, end - start));
        start = end + 1;
        end = str.find_first_of(delimiters, start);
    }
	
    if (start < str.length()) {
        tokens.push_back(str.substr(start));
    }

    return tokens;
}

static bool hasDuplicates(const std::string& str)
{
    for (size_t i = 0; i < str.length(); ++i)
	{
        for (size_t j = i + 1; j < str.length(); ++j)
		{
            if (str[i] == str[j])
                return true;
        }
    }
    return false;
}

static std::vector<char> getStringsSymbols(const std::string &modeStr)
{
	std::vector<char> symbols;
	for (size_t i = 0; i < modeStr.length(); ++i)
	{
		if (modeStr[i] == '+' || modeStr[i] == '-')
			symbols.push_back(modeStr[i]);
	}
	return symbols;
}

static bool hasUniqueCharsInEachString(const std::vector<std::string> &vec)
{
    for (size_t i = 0; i < vec.size(); ++i)
	{
        const std::string& str1 = vec[i];
        for (size_t j = 0; j < vec.size(); ++j)
		{
            if (i != j)
			{
                const std::string& str2 = vec[j];
                for (size_t k = 0; k < str1.length(); ++k)
				{
                    for (size_t l = 0; l < str2.length(); ++l)
					{
                        if (str1[k] == str2[l])
                            return false;
                    }
                }
            }
        }
    }
    return true;
}

static bool checkDuplications(const std::vector<std::string> &modeSubStrs, const std::vector<char> &symbols)
{
	for (std::vector<std::string>::const_iterator it = modeSubStrs.begin(); it != modeSubStrs.end(); ++it)
	{
		if (hasDuplicates(*it))
			return false;
	}
	std::vector<std::string> setStrings;
	std::vector<std::string> removeStrings;
	for (size_t i = 0; i < symbols.size(); ++i)
	{
		if (symbols[i] == '+')
			setStrings.push_back(modeSubStrs[i]);
		else if (symbols[i] == '-')
			removeStrings.push_back(modeSubStrs[i]);
	}
	if (hasUniqueCharsInEachString(setStrings) && hasUniqueCharsInEachString(removeStrings))
		return true;
	return false;
}

static bool isCorrectModeString(const std::string &modeString)
{
	if (modeString.empty())
		return false;
	if (modeString[0] != '-' && modeString[0] != '+')
		return false;
	for (size_t i = 1; i < modeString.size(); ++i)
	{
		if (modeString[i] != '+' && modeString[i] != '-' && modeString[i] != 'i'
			&& modeString[i] != 't' && modeString[i] != 'k' &&
				modeString[i] != 'o' && modeString[i] != 'l')
			return false;
	}
	for (size_t i = 0; i < modeString.size() - 1; ++i)
	{
		if (modeString[i] == modeString[i + 1])
			return false;
	}
	std::vector<std::string> modeSubStrs = split(modeString, "+-");
	std::vector<char> symbols = getStringsSymbols(modeString);
	if (!checkDuplications(modeSubStrs, symbols))
		return false;
	return true;
}

int main(void)
{
	std::cout << (isCorrectModeString("-op") ? "Yes" : "No") << std::endl;
	// std::cout << (isCorrectModeString("-i") ? "Yes" : "No") << std::endl;
	// std::cout << (isCorrectModeString("+o") ? "Yes" : "No") << std::endl;
	// std::cout << (isCorrectModeString("-o") ? "Yes" : "No") << std::endl;
	// std::cout << (isCorrectModeString("+k") ? "Yes" : "No") << std::endl;
	// std::cout << (isCorrectModeString("-k") ? "Yes" : "No") << std::endl;
	// std::cout << (isCorrectModeString("+t") ? "Yes" : "No") << std::endl;
	// std::cout << (isCorrectModeString("-t") ? "Yes" : "No") << std::endl;
	// std::cout << (isCorrectModeString("+l") ? "Yes" : "No") << std::endl;
	// std::cout << (isCorrectModeString("-l") ? "Yes" : "No") << std::endl;
}