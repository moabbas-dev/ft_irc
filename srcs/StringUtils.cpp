/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StringUtils.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfatfat <jfatfat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 14:40:06 by jfatfat           #+#    #+#             */
/*   Updated: 2025/01/17 14:40:07 by jfatfat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/StringUtils.hpp"

std::vector<std::string> split(const std::string& str, const std::string& delimiters)
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

bool hasDuplicates(const std::string& str)
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

std::string getNumberAsString(unsigned long long nb)
{
	std::ostringstream oss;
    oss << nb;
    return oss.str();
}

bool isValidPass(const std::string &pass)
{
	if(pass.empty())
		return false;
	for (size_t i = 0; i < pass.size(); ++i)
	{
		if (!std::isalnum(pass[i]))
			return false;
	}
	return true;
}

bool hasUniqueCharsInEachString(const std::vector<std::string> &vec)
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