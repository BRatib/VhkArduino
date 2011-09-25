#ifndef SoftwareTimers_h
#define SoftwareTimers_h
#endif

#if ARDUINO < 100
#include <WProgram.h>
#else
#include <Arduino.h>
#endif

class Timer32 {
protected:
	uint32_t setTime; // The moment the timer was set.
	uint32_t delay; // The time the timer should wait.
public:
	Timer32() {};
	Timer32(uint32_t duration, uint32_t now) { set(duration, now); };
	
	bool expired(uint32_t now);
	uint32_t remaining(uint32_t now);
	void set(uint32_t duration, uint32_t now);
};

class MicroSecondTimer : public Timer32 {
public:
	MicroSecondTimer() {};
	MicroSecondTimer(uint32_t us) { set(us); };
	
	bool expired();
	uint32_t remaining();
	void set(uint32_t us);
};

class MilliSecondTimer : public Timer32 {
public:
	MilliSecondTimer() {};
	MilliSecondTimer(uint32_t ms) { set(ms); };
	
	bool expired();
	uint32_t remaining();
	void set(uint32_t ms);
};

class AckTimer : public MilliSecondTimer {
protected:
	bool armed;
public:
	void disarm();
	bool isArmed();
	void set(uint32_t ms);
};
