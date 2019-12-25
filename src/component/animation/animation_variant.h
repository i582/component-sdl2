#pragma once

#pragma once

#include "string"
#include "variant"
#include "../../tools/css/color/color.h"

namespace Kit
{
	using std::string;
	using std::variant;
	using std::visit;
	using std::get;

	using CSS::Color;

	template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
	template<class... Ts> overload(Ts...)->overload<Ts...>;


	enum class animation_variant_type
	{
		INT,
		DOUBLE,
		COLOR,

		UNDEFINED
	};

	using raw_animation_variant = variant<int, double, Color>;

	class animation_variant
	{
	private:
		raw_animation_variant variant;

	public:
		template<typename T>
		animation_variant(T);

		animation_variant() {};

	public:
		animation_variant& operator=(const animation_variant& animation_variant_type);
		animation_variant& operator=(const raw_animation_variant& raw_variant);

		template<typename T>
		animation_variant& operator=(const T& value);

		template<typename T>
		T get();

		animation_variant_type type() const;

		int to_int(bool no_exception = false) const;
		double to_double(bool no_exception = false) const;
		Color to_color(bool no_exception = false) const;
	};

	template<typename T>
	inline animation_variant::animation_variant(T value)
	{
		variant = value;
	}

	template<typename T>
	inline animation_variant& animation_variant::operator=(const T& value)
	{
		variant = value;
		return *this;
	}

	template<typename T>
	inline T animation_variant::get()
	{
		return std::get<T>(variant);
	}

}