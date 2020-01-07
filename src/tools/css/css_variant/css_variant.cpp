#include "css_variant.h"

using namespace CSS;

CSS::css_variant& CSS::css_variant::operator=(const css_variant& css_variant)
{
    this->variant = css_variant.variant;
    return *this;
}

CSS::css_variant& CSS::css_variant::operator=(const raw_css_variant& raw_variant)
{
    this->variant = raw_variant;
    return *this;
}

CSS::css_variant_type CSS::css_variant::type() const
{
    css_variant_type type = css_variant_type::UNDEFINED;

    std::visit(overload{
            [&](const string& s)
            {
                type = css_variant_type::STRING;
            }, [&](const int d)
            {
                type = css_variant_type::INT;
            }, [&](const double b)
            {
                type = css_variant_type::DOUBLE;
            }, [&](const Color& c)
            {
                type = css_variant_type::COLOR;
            }
    }, variant);

    return type;
}

int CSS::css_variant::to_int(bool no_exception) const
{
    if (type() == css_variant_type::INT)
        return std::get<int>(variant);
    else if (no_exception == false)
        throw std::logic_error("Error, variant currently contains no int");
}

double CSS::css_variant::to_double(bool no_exception) const
{
    if (type() == css_variant_type::DOUBLE)
        return std::get<double>(variant);
    else if (no_exception == false)
        throw std::logic_error("Error, variant currently contains no double");
}

string CSS::css_variant::to_string(bool no_exception) const
{
    if (type() == css_variant_type::STRING)
        return std::get<string>(variant);
    else if (no_exception == false)
        throw std::logic_error("Error, variant currently contains no string");
}

Color CSS::css_variant::to_color(bool no_exception) const
{
    if (type() == css_variant_type::COLOR)
        return std::get<Color>(variant);
    else if (no_exception == false)
        throw std::logic_error("Error, variant currently contains no color");
}
