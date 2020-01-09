#include "utils.h"

using namespace Kit;

int Utils::to_integer(const string& str)
{
    return stoi(str);
}

double Utils::to_double(const string& str)
{
    return stod(str);
}

vector<string>* Utils::split(const string& str, char symbol)
{
    string temp;

    auto result = new vector<string>;


    for (char current : str)
    {
        if (current == symbol)
        {
            result->push_back(temp);
            temp.clear();
        }
        else
        {
            temp += current;
        }
    }

    if (!temp.empty())
        result->push_back(temp);

    return result;
}

vector<string>* Utils::split(const string& str, const string& pattern, char separator)
{
    vector<string>* separators = Utils::split(pattern, separator);
    vector<string>* result = new vector<string>;
    string temp;
    bool is_sep = false;

    for (char current : str)
    {
        for (auto& separator : *separators)
        {
            if (current == separator[0])
            {
                is_sep = true;
            }
        }

        if (is_sep)
        {
            result->push_back(temp);
            temp.clear();
            is_sep = false;
        }
        else
        {
            temp += current;
        }

    }

    if (!temp.empty())
        result->push_back(temp);

    return result;
}

string Utils::to_lower(const string& str)
{
    string str_result = str;

    std::transform(str_result.begin(), str_result.end(), str_result.begin(), (int (*)(int))std::tolower);

    return str_result;
}

string Utils::to_upper(const string& str)
{
    string str_result = str;

    std::transform(str_result.begin(), str_result.end(), str_result.begin(), (int (*)(int))std::toupper);

    return str_result;
}

bool Utils::is_integer(const string& str)
{
    for (char s : str)
    {
        if (s < '0' || s > '9')
        {
            return false;
        }
    }

    return true;
}

bool Utils::is_number(const string& str)
{
    for (char s : str)
    {
        if ((s < '0' || s > '9') && s != '.')
        {
            return false;
        }
    }

    return true;
}

