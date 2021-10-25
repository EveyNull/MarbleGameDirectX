#include "StringHelper.h"

std::vector<std::string>
StringHelper::split(const std::string& string, std::string delimiter)
{
    std::vector<std::string> results;
    std::size_t current, previous = 0;

    current = string.find(delimiter);
    if (current == std::string::npos)
    {
        current = string.size();
    }
    while (current != std::string::npos)
    {
        results.emplace_back(string.substr(previous, current - previous));
        previous = current + 1;
        current = string.find(delimiter, previous);
    }
    return results;
}

std::string StringHelper::join(const std::vector<std::string>& input)
{
    std::string result;
    for (const std::string& string : input)
    {
        result.append(string);
    }
    return result;
}