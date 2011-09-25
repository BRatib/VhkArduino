/**
 * This sketch is used to test the Timers in the VhkArduino library.
 */

#include <VhkArduino.h>

Timer32 t;
MilliSecondTimer mst;
MicroSecondTimer ust;
AckTimer at;

void timer32Test(uint32_t settime, uint32_t duration, uint32_t now) {
	t.set(100, 4294967296);
	Serial.print("Set: "); Serial.print(settime);
	Serial.print(" Duration: "); Serial.print(duration);
	Serial.print(" Now: "); Serial.print(now);
	Serial.print(" Remaining: "); Serial.print(t.remaining(now));
	Serial.print(" Expired: "); Serial.println(t.expired(now));
}

void setup() {
	Serial.begin(57600);
	Serial.println("Timer32 Test");
	timer32Test(4294967296, 100, 98);
	timer32Test(4294967296, 100, 99);
	timer32Test(4294967296, 100, 100);
	timer32Test(4294967296, 0, 4294967296);
	timer32Test(0, 100, 3);
	
	Serial.println("\nMilliSecondTimer Test");
	mst.set(100);
	Serial.print("mst expired: "); Serial.print(mst.expired());
	Serial.print(" remaining: "); Serial.println(mst.remaining());
	delay(100);
	Serial.print("mst expired: "); Serial.print(mst.expired());
	Serial.print(" remaining: "); Serial.println(mst.remaining());
	
	Serial.println("\nMicroSecondTimer Test");
	ust.set(10000);
	Serial.print("ust expired: "); Serial.print(ust.expired());
	Serial.print(" remaining: "); Serial.println(ust.remaining());
	delayMicroseconds(10000);
	Serial.print("ust expired: "); Serial.print(ust.expired());
	Serial.print(" remaining: "); Serial.println(ust.remaining());
	
	Serial.println("\nAckTimer Test");
	at.set(100);
	Serial.print("at Armed: "); Serial.println(at.isArmed());
	delay(at.remaining());
	Serial.print("at Armed: "); Serial.println(at.isArmed());
	
	at.set(100);
	Serial.print("at Armed: "); Serial.println(at.isArmed());
	at.disarm();
	Serial.print("at Armed: "); Serial.println(at.isArmed());
	
}

void loop() {}
