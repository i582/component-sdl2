#pragma once

#include <string>
#include <variant>

#include "../color/color.h"

namespace CSS
{
    using std::variant;
    using std::visit;
    using std::get;

    template<class... Ts>
    struct overload : Ts ...
    {
        using Ts::operator()...;
    };
    template<class... Ts> overload(Ts...)->overload<Ts...>;


    enum class css_variant_type
    {
        INT,
        DOUBLE,
        STRING,
        COLOR,

        UNDEFINED
    };

    using raw_css_variant = variant<int, double, string, Color>;

    class css_variant
    {
    private:
        raw_css_variant variant;

    public:
        template<typename T>
        css_variant(T);
        css_variant() {};

    public:
        css_variant& operator=(const css_variant& css_variant);
        css_variant& operator=(const raw_css_variant& raw_variant);

        template<typename T>
        css_variant& operator=(const T& value);

        template<typename T>
        T get();

        css_variant_type type() const;

        int to_int(bool no_exception = false) const;
        double to_double(bool no_exception = false) const;
        string to_string(bool no_exception = false) const;
        Color to_color(bool no_exception = false) const;
    };

    template<typename T>
    inline css_variant::css_variant(T value)
    {
        variant = value;
    }

    template<typename T>
    inline css_variant& css_variant::operator=(const T& value)
    {
        variant = value;
        return *this;
    }

    template<typename T>
    inline T css_variant::get()
    {
        return std::get<T>(variant);
    }

}