#pragma once


namespace Acrylic {

	class Timestep
	{
	public:
		Timestep(float time) : m_Time(time) {}

		operator float() const { return m_Time; }

		float GetSeconds() const { return m_Time; }
		float GetMilliseconds() const { return m_Time * 1000.0f; }
		float GetFps() const { return 1 / m_Time; }
	private:
		float m_Time;
	};

}