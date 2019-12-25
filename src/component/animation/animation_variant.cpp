#include "animation_variant.h"

using namespace Kit;

Kit::animation_variant& Kit::animation_variant::operator=(const animation_variant& animation_variant)
{
	this->variant = animation_variant.variant;
	return *this;
}

Kit::animation_variant& Kit::animation_variant::operator=(const raw_animation_variant& raw_variant)
{
	this->variant = raw_variant;
	return *this;
}

Kit::animation_variant_type Kit::animation_variant::type() const
{
	animation_variant_type type = animation_variant_type::UNDEFINED;

	std::visit(overload{
		[&](const int d) {
			type = animation_variant_type::INT;
		},
		[&](const double b) {
			type = animation_variant_type::DOUBLE;
		},
		[&](const Color& c) {
			type = animation_variant_type::COLOR;
		}
		}, variant);

	return type;
}

int Kit::animation_variant::to_int(bool no_exception) const
{
	if (type() == animation_variant_type::INT)
		return std::get<int>(variant);
	else if (no_exception == false)
		throw std::logic_error("Error, variant currently contains no int");
}

double Kit::animation_variant::to_double(bool no_exception) const
{
	if (type() == animation_variant_type::DOUBLE)
		return std::get<double>(variant);
	else if (no_exception == false)
		throw std::logic_error("Error, variant currently contains no double");
}

Color Kit::animation_variant::to_color(bool no_exception) const
{
	if (type() == animation_variant_type::COLOR)
		return std::get<Color>(variant);
	else if (no_exception == false)
		throw std::logic_error("Error, variant currently contains no color");
}
