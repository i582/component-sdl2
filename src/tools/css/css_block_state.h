#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <map>


#include "color/color.h"
#include "utils/utils.h"

#include "css_variant/css_variant.h"

namespace CSS
{

    using std::map;


    class css_block_state
    {
    private:
        map<string, css_variant> _styles;


    public:
        css_block_state();

        explicit css_block_state(bool general);

    public:

        void mergeWith(css_block_state& block);

        void mergeWithBaseIs(css_block_state& block);


        void set(const string& attribute, const string& value);

        template <typename T>
        void set(const string& attribute, T value);


        template<typename T>
        T get(const string& key);
    };


    template<typename T>
    void CSS::css_block_state::set(const std::string& attribute, T value)
    {
        if (attribute.empty())
            return;

        _styles[attribute] = value;
    }


    template<typename T>
    T css_block_state::get(const string& key)
    {
        T result;

        if (_styles.find(key) == _styles.end())
        {
            throw std::logic_error(string("ERROR: ") + key + " is NOT found in this map!");
        }

        try
        {
            result = _styles[key].get<T>();
        }
        catch (const std::exception& e)
        {
            cout << e.what() << endl;
            return T(0);
        }

        return result;
    }


}