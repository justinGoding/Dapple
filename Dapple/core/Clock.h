#pragma once

#include <chrono>

typedef std::chrono::time_point<std::chrono::high_resolution_clock> Timestamp;
typedef std::chrono::milliseconds::rep Duration;

class Clock
{
public:
	Clock() {};

	Timestamp startTime() { return m_start; }
	Timestamp lastFrameTime() { return m_lastTimeStamp; }
	Duration deltaTime() { return m_deltaTime; }
	Duration totalElapsedTime() { return m_totalElapsedTime; }

	Timestamp timestamp() { return std::chrono::high_resolution_clock::now(); }
	Duration elapsedTime(Timestamp time) 
	{
		Timestamp now = std::chrono::high_resolution_clock::now();
		return std::chrono::duration<float, std::milli>(now - time).count();
	}

private:
	void _init()
	{
		m_start = std::chrono::high_resolution_clock::now();
		m_lastTimeStamp = m_start;
		m_currentTimeStamp = m_start;
		m_deltaTime = std::chrono::duration<float, std::milli>(m_currentTimeStamp - m_start).count();
		m_totalElapsedTime = m_deltaTime;
	}

	void _update()
	{
		m_lastTimeStamp = m_currentTimeStamp;
		auto time = std::chrono::high_resolution_clock::now();
		m_deltaTime = std::chrono::duration<float, std::milli>(time - m_lastTimeStamp).count();
		m_currentTimeStamp = time;
		m_totalElapsedTime = std::chrono::duration<float, std::milli>(time - m_start).count();
	}

private:
	Timestamp m_start;
	Timestamp m_lastTimeStamp;
	Timestamp m_currentTimeStamp;
	Duration m_deltaTime;
	Duration m_totalElapsedTime;

	friend class Application;
};