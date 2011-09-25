#if ARDUINO < 100
#include <WProgram.h>
#else
#include <Arduino.h>
#endif

#include "SoftwareTimers.h"

bool AckTimer::isArmed() {
	return armed;
};

void AckTimer::disarm() {
	armed = false;
};

void AckTimer::set(uint32_t ms) {
	armed = true;
	MilliSecondTimer::set(ms);
};

bool MicroSecondTimer::expired() {
	return Timer32::expired(micros());
};

uint32_t MicroSecondTimer::remaining() {
	return Timer32::remaining(micros());
};

void MicroSecondTimer::set(uint32_t us) {
	Timer32::set(us, micros());
};

// True if timer has expired.
bool MilliSecondTimer::expired() {
	return Timer32::expired(millis());
};

// Return the time remaining on the timer.
uint32_t MilliSecondTimer::remaining() {
	return Timer32::remaining(millis());
};

// Set the timer to trigger ms seconds from now.
void MilliSecondTimer::set(uint32_t ms) {
	Timer32::set(ms, millis());
};

//Timer32 methods
bool Timer32::expired(uint32_t now) {
	return now < setTime ? (now - delay) >= setTime : now >= (setTime + delay);
};

uint32_t Timer32::remaining(uint32_t now) {
	return expired(now) ? 0 : setTime - (now - delay);
};

void Timer32::set(uint32_t duration, uint32_t now) {
	setTime = now; delay = duration;
};
