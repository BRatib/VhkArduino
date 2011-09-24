/**
 * This sketch is used to test the Timers in the VhkArduino library.
 */

#include <VhkArduino.h>

Timer32 t;

void timerTest(uint32_t settime, uint32_t duration, uint32_t now) {
	t.set(100, 4294967296);
	Serial.print("Set: "); Serial.print(settime);
	Serial.print(" Duration: "); Serial.print(duration);
	Serial.print(" Now: "); Serial.print(now);
	Serial.print(" Remaining: "); Serial.print(t.remaining(now));
	Serial.print(" Expired: "); Serial.println(t.expired(now));
}

void setup() {
	Serial.begin(57600);
	Serial.println("[TimerTest]");
	timerTest(4294967296, 100, 98);
	timerTest(4294967296, 100, 99);
	timerTest(4294967296, 100, 100);
	timerTest(4294967296, 0, 4294967296);
	timerTest(0, 100, 3);
}

void loop() {}
