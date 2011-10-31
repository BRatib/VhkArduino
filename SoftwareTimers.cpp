#if ARDUINO < 100
#include <WProgram.h>
#else
#include <Arduino.h>
#endif

#include "SoftwareTimers.h"

/**
 * Indicate if the timer is still armed.
 * @return True if timer has not been disarmed (No ACK receiverd yet) and False if the timer is disarmed.
 */
bool AckTimer::isArmed() {
	return armed;
};

/**
 * Disarms the timer.
 * You should call this when an ack was received or if the timer has expired.
 */
void AckTimer::disarm() {
	armed = false;
};

/**
 * Arms the timer and sets the timer to expire ms Milliseconds from now.
 * @param ms should be > 0 since a delay of 0 ms makes no sense!.
 */
void AckTimer::set(uint32_t ms) {
	armed = true;
	MilliSecondTimer::set(ms);
};

/**
 * Indicate if the timer has expired.
 * @return True if the timer has expired, otherwise false.
 */
bool MicroSecondTimer::expired() {
	return Timer32::expired(micros());
};

/**
 * Calculate the number of MicroSeconds remaining untill expiration.
 * @return the number of MicroSeconds left till expiration or 0 if the timer has expired.
 */
uint32_t MicroSecondTimer::remaining() {
	return Timer32::remaining(micros());
};

/**
 * Sets the timer to expire us microseconds from now.
 * @param us should be > 0 since a delay of 0 us makes no sense!.
 */
void MicroSecondTimer::set(uint32_t us) {
	Timer32::set(us, micros());
};

/**
 * Indicate if the timer has expired.
 * @return True if the timer has expired, otherwise false.
 */
bool MilliSecondTimer::expired() {
	return Timer32::expired(millis());
};

/**
 * Calculate the number of MilliSeconds remaining untill expiration.
 * @return the number of MilliSeconds left till expiration or 0 if the timer has expired.
 */
uint32_t MilliSecondTimer::remaining() {
	return Timer32::remaining(millis());
};

/**
 * Sets the timer to expire ms milliseconds from now.
 * @param ms should be > 0 since a delay of 0 ms makes no sense!.
 */
void MilliSecondTimer::set(uint32_t ms) {
	Timer32::set(ms, millis());
};

/**
 * Indicate if the timer has expired.
 * @return True if the timer has expired, otherwise false.
 */
bool Timer32::expired(uint32_t now) {
	return now >= (setTime + delay);
};

/**
 * Calculate the number if time-units left untill expiration.
 * @return the number of time units remaining untill expiration or 0 if the timer has expired.
 */
uint32_t Timer32::remaining(uint32_t now) {
	return expired(now) ? 0 : (setTime + delay) - now;
};

/**
 * Sets the timer to expire a given number of time units from now.
 * @param duration should be > 0 since a delay of 0 makes no sense!.
 */
void Timer32::set(uint32_t duration, uint32_t now) {
	setTime = now; delay = duration;
};
