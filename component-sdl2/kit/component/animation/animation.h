#pragma once

#include "SDL.h"
#include "functional"
#include "variant"
#include "iostream"

#include "animation_variant.h"

namespace Kit
{
	using std::function;
	using std::variant;

	using an_variant = animation_variant;


	template<typename T>
	class animation
	{
	private:
		an_variant from;
		an_variant to;

		size_t duration;


		size_t start_time;
		size_t last_frame_time;

		an_variant current_value;

		T* variable_value;
		function<void()> callback;

		bool is_end;

	public:
		animation(T from, T to, size_t duration, T* variable_value, function<void()> callback);

	public:
		void start();
		void check();

		void next_frame();
		
		double timing(double time);

		void end();
	};

	template<typename T>
	animation<T>::animation(T from, T to, size_t duration, T* variable_value, function<void()> callback)
	{
		this->from = from;
		this->to = to;
		this->duration = duration;

		this->start_time = -1;
		this->last_frame_time = -1;

		this->current_value = from;
		this->variable_value = variable_value;
		this->callback = callback;

		this->is_end = true;
	}

	template<typename T>
	void animation<T>::start()
	{
		this->start_time = SDL_GetTicks();
		this->last_frame_time = start_time;
	
		this->is_end = false;
	}

	template<typename T>
	void animation<T>::check()
	{
		if (!is_end)
		{
			size_t current_time = SDL_GetTicks();

			if (current_time > last_frame_time + 17 && current_time - start_time <= duration)
			{
				next_frame();

				this->last_frame_time = current_time;
			}

			if (current_time - start_time > duration)
			{
				end();
			}
		}
	}

	template<typename T>
	void animation<T>::next_frame()
	{
		if (this->variable_value != nullptr)
		{
			size_t current_time = SDL_GetTicks();

			double timeFraction = (current_time - start_time) / (double)duration;
			if (timeFraction > 1) timeFraction = 1;
			
			double progress = timing(timeFraction);

			//draw(progress); // отрисовать её

			auto value_to = to.get<T>();
			auto value_from = from.get<T>();

			*this->variable_value = value_from + ((value_to - value_from) * progress);
			//cout << progress << endl;
			//cout << progress << endl;
			//(*this->variable_value) -= 5;
		}
		
		//cout << "Кадр" << endl;
	}

	template<typename T>
	double animation<T>::timing(double time)
	{
		//return time;
		return pow(time, 2);
	}

	template<typename T>
	void animation<T>::end()
	{
		cout << "End animation" << endl;
		is_end = true;
	}

}