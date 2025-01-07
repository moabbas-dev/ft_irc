#include <iostream>
#include <vector>

std::string trimString(const std::string& str)
{
    std::string::size_type start = 0;
    while(start < str.length() && std::isspace(str[start]))
        ++start;
    std::string::size_type end = str.length();
    while(end > start && std::isspace(str[end - 1]))
        --end;

    return str.substr(start ,end - start);
}

int main(void)
{
	std::cout << (trimString("Hello\r\nWorld\r\n")) << std::endl;
	// std::string = "Hello\rWorld\r";
	// std::cout << (isCorrectModeString("-op") ? "Yes" : "No") << std::endl;
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