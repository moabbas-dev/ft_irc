/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StringUtils.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfatfat <jfatfat@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/17 14:39:07 by jfatfat           #+#    #+#             */
/*   Updated: 2025/01/17 14:39:08 by jfatfat          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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