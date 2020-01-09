#pragma once

#include "css-attributes.h"

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

        template<typename T>
        void set(const string& key, T value);

        template<typename T>
        T get(const string& key);
    };


    template<typename T>
    void css_block_state::set(const string& key, T value)
    {
        if (key.empty())
            return;

        _styles[key] = value;
    }

    template<typename T>
    T css_block_state::get(const string& key)
    {
        T result;

        if (_styles.find(key) == _styles.end())
        {
            cout << "ERROR: " << key << " is NOT found in this map!" << endl;
            return T(0);
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