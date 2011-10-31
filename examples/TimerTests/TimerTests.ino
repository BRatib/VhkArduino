/**
 * This sketch is used to test the Timers in the VhkArduino library.
 */

#include <VhkArduino.h>

Timer32 t;
MilliSecondTimer mst;
MicroSecondTimer ust;
AckTimer at;

void timer32Test(uint32_t settime, uint32_t duration, uint32_t now) {
	t.set(duration, settime);
	Serial.print(F("Set: ")); Serial.print(settime);
	Serial.print(F(" Duration: ")); Serial.print(duration);
	Serial.print(F(" Now: ")); Serial.print(now);
	Serial.print(F(" Remaining: ")); Serial.print(t.remaining(now));
	Serial.print(F(" Expired: ")); Serial.println(t.expired(now));
}

void setup() {
	Serial.begin(57600);
	Serial.println(F("Timer32 Test"));
	timer32Test(0, 100, 98);
	timer32Test(0, 100, 99);
	timer32Test(0, 100, 100);
	timer32Test(0, 100, 101);
	timer32Test(0, 0, 0);
	
	timer32Test(1, 3, 1);
	timer32Test(1, 3, 2);
	timer32Test(1, 3, 3);
	timer32Test(1, 3, 4);
	timer32Test(1, 3, 5);
	timer32Test(1, 3, 6);
	timer32Test(1, 3, 7);
	
	timer32Test(4294967295, 100, 98);
	timer32Test(4294967295, 100, 99);
	timer32Test(4294967295, 100, 100);
	timer32Test(4294967295, 100, 101);
	timer32Test(4294967295, 0, 3);
	timer32Test(0, 100, 3);
	
	Serial.println(F("\nMilliSecondTimer Test"));
	mst.set(100);
	Serial.print(F("mst expired: ")); Serial.print(mst.expired());
	Serial.print(F(" remaining: ")); Serial.println(mst.remaining());
	delay(100);
	Serial.print(F("mst expired: ")); Serial.print(mst.expired());
	Serial.print(F(" remaining: ")); Serial.println(mst.remaining());
	
	Serial.println(F("\nMicroSecondTimer Test"));
	ust.set(10000);
	Serial.print(F("ust expired: ")); Serial.print(ust.expired());
	Serial.print(F(" remaining: ")); Serial.println(ust.remaining());
	delayMicroseconds(10000);
	Serial.print(F("ust expired: ")); Serial.print(ust.expired());
	Serial.print(F(" remaining: ")); Serial.println(ust.remaining());
	
	Serial.println(F("\nAckTimer Test"));
	at.set(100);
	Serial.print(F("at Armed: ")); Serial.print(at.isArmed());
	Serial.print(F(" at epired: ")); Serial.println(at.expired());
	delay(at.remaining());
	Serial.print(F("at Armed: ")); Serial.print(at.isArmed());
	Serial.print(F(" at epired: ")); Serial.println(at.expired());
	
	at.set(100);
	Serial.print(F("at Armed: ")); Serial.println(at.isArmed());
	at.disarm();
	Serial.print(F("at Armed: ")); Serial.println(at.isArmed());
	Serial.println();
}

void loop() {}
