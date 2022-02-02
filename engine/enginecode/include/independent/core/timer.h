/** \file timer.h */
#pragma once

#include <chrono>

namespace Engine
{
	/** \class Timer 
	** \brief Class for timers
	*/
	class Timer
	{
	public:
		virtual void start() = 0; //!< Start the timer
		virtual void reset() = 0; //!< Reset the timer
		virtual float getElapsedTime() = 0; //!< Get the time elapsed since the last start or restart
	};

	/** \class ChronoTimer
	** \brief Chrono timer class 
	*/ 
	class ChronoTimer : public Timer
	{
	public:
		virtual inline void start() override { m_startPoint = std::chrono::high_resolution_clock::now(); } //!< Start the chrono timer
		virtual inline void reset() override { m_startPoint = std::chrono::high_resolution_clock::now(); } //!< Reset the chrono timer
		virtual float getElapsedTime() override //!< Get the time elapsed since the last start or restart
		{
			m_endPoint = std::chrono::high_resolution_clock::now();
			std::chrono::duration<float, std::milli> elapsed = m_endPoint - m_startPoint;
			return elapsed.count() / 1000.f;
		}
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> m_startPoint; //!< Start time for the timer 
		std::chrono::time_point<std::chrono::high_resolution_clock> m_endPoint; //!< End time for the timer
	};
}