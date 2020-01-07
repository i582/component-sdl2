#include "utils.h"

using namespace CSS;

int Utils::to_integer(const string& str)
{
    return stoi(str);
}

double Utils::to_double(const string& str)
{
    return stod(str);
}

vector<string>* Utils::split(string str, char symbol)
{
    vector<string>* result = new vector<string>;
    string temp;

    for (size_t i = 0; i < str.length(); i++)
    {
        char current = str[i];

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

vector<string>* Utils::split(string str, string pattern, char separator)
{
    vector<string>* separators = Utils::split(pattern, separator);
    vector<string>* result = new vector<string>;
    string temp;
    bool is_sep = false;

    for (size_t i = 0; i < str.length(); i++)
    {
        char current = str[i];

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


bool Utils::is_integer(string str)
{
    for (size_t i = 0; i < str.length(); i++)
    {
        if (str.at(i) < '0' || str.at(i) > '9')
        {
            return false;
        }
    }

    return true;
}

bool Utils::is_number(string str)
{
    for (size_t i = 0; i < str.length(); i++)
    {
        if ((str.at(i) < '0' || str.at(i) > '9') && str.at(i) != '.')
        {
            return false;
        }
    }

    return true;
}
