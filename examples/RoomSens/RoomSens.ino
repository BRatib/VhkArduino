#include <JeeLib.h>
#include <VhkArduino.h>
#include <util/atomic.h>

#define SERIAL 0
#define NODEID 2

#define RETRIES		5
#define CYCLE_TIME	60000
#define ACK_TIME	32

#define PIR_PORT	1 // defined if PIR is connected to a port's DIO pin.
#define LDR_PORT	1 // defined if LDR is connected to a port's AIO pin.
#define SHT11_PORT	4 // defined if SHT11 is connected to a port.
//#define LUX_PORT	3 // defined if TSL2561 is connected to a port.
//#define BMP_PORT	3 // defined if BMP085 is connected to a port.

#if SHT11_PORT
#include <PortsSHT11.h>
#endif

#if BMP_PORT
#include <PortsBMP085.h>
#endif

struct Report {
	byte		lowBattery	:1;		// Supply voltage below 3.1 volts 0...1.
	#if PIR_PORT
	byte		moved		:1;		// Motion detection 0...1.
	#endif
	#if LDR_PORT
	byte		light;				// Light value 0...255.
	#endif
	#if SHT11_PORT
	uint16_t	humidity; 			// Humidity 0%...100% in 0.01 steps.
	int16_t		temperature;		// Temperature -100...100 degree celcius in 0.01 steps.
	#endif
	#if LUX_PORT
	uint32_t	lux			:24;	// Light in Lux
	#endif
	#if BMP_PORT
	int16_t		bmptemp;
	int32_t		pressure;
	#endif
};
Report report;

uint8_t myNodeID;
MilliSecondTimer mst;

ISR(WDT_vect) { Sleepy::watchdogEvent(); }

#if SHT11_PORT
SHT11 sht11(SHT11_PORT);
#endif

#if LDR_PORT
Port ldr(LDR_PORT);
#endif

#if LUX_PORT 
PortI2C luxBus(LUX_PORT);
LuxPlug lux(luxBus, 0x39);
byte luxHighGain = false;
#endif

#if BMP_PORT && LUX_PORT && LUX_PORT == BMP_PORT
BMP085 psensor(luxBus, 3);
#elif BMP_PORT
PortI2C bmpBus(BMP_PORT);
BMP085 psensor(bmpBus, 3);
#endif

#if PIR_PORT
// Calculate the correct PCINT
#if PIR_PORT == 1
#define PIR_PCINT_NUMBER PCINT20
#elif PIR_PORT == 2
#define PIR_PCINT_NUMBER PCINT21
#elif PIR_PORT == 3
#define PIR_PCINT_NUMBER PCINT22
#elif PIR_PORT == 4
#define PIR_PCINT_NUMBER PCINT23
#endif

// Pir code here
class PIR : public Port {
	public:
		byte pending;
		PIR(byte portnum) : Port (portnum) {
			digiWrite(1);
			PCICR |= (1<<PCIE2);
			PCMSK2 |= (1<<PIR_PCINT_NUMBER);
			MCUCR = (1<<ISC00) | (1<<ISC01);
		}
		
		byte moved() {
			return !digiRead();
		}
		
		void poll() {
			pending = true;
		}
};
PIR pir(PIR_PORT);

ISR(PCINT2_vect){
	pir.poll();
};
#endif

void takeMeasurement() {
	#if LUX_PORT
	lux.begin();
    lux.setGain(luxHighGain);
	MilliSecondTimer luxTimer(1000); // Wait for proper powerup.
	#endif
	
	#if SHT11_PORT
	sht11.measure(SHT11::HUMI);		
	sht11.measure(SHT11::TEMP);    
	#endif
	
	#if SHT11_PORT || BMP_PORT
	MilliSecondTimer sht11Timer(32); // Set a timer to wait 32 ms.
	#endif
	
	#if LDR_PORT
	ldr.digiWrite2(1);  // enable AIO pull-up
    report.light = ~ ldr.anaRead() >> 2;
    ldr.digiWrite2(0);  // disable pull-up to reduce current draw
    #endif
    
    #if PIR_PORT
    report.moved = pir.moved();
    #endif
	
	#if SHT11_PORT
	Sleepy::loseSomeTime(sht11Timer.remaining()); // Wait for sensors.
	float h, t;
	sht11.calculate(h, t);
	report.humidity = h*100 + 0.5;
	report.temperature = t*100 + 0.5;
	#endif
	
	#if BMP_PORT
	psensor.startMeas(BMP085::TEMP);
	Sleepy::loseSomeTime(32);
	psensor.getResult(BMP085::TEMP);
	psensor.startMeas(BMP085::PRES);
	Sleepy::loseSomeTime(32);
	psensor.getResult(BMP085::PRES);
	psensor.calculate(report.bmptemp, report.pressure);
	#endif
	
	#if LUX_PORT
    Sleepy::loseSomeTime(luxTimer.remaining()); // Wait for proper powerup
    lux.getData();
    float luxtemp = lux.calcLux();
    if(luxHighGain) {
		luxtemp = luxtemp/16.0f;
	}
	report.lux = luxtemp*100.0f + 0.5;
	if(report.lux < 1000) {
		luxHighGain = true;
	} else {
		luxHighGain = false;
	}
	lux.poweroff();
    #endif
	
	report.lowBattery = rf12_lowbat();
}

void sendReport() {
	#if SERIAL
	#if LDR_PORT
	Serial.print(report.light, DEC); Serial.print(F(" "));
	#endif
	#if SHT11_PORT
	Serial.print(report.humidity, DEC); Serial.print(F(" "));
	Serial.print(report.temperature, DEC); Serial.print(F(" "));
	#endif
	#if PIR_PORT
	Serial.print(report.moved, DEC); Serial.print(F(" "));
	#endif
	#if LUX_PORT
	Serial.print(report.lux, DEC); Serial.print(F(" "));
	#endif
	#if BMP_PORT
	Serial.print(report.bmptemp, DEC); Serial.print(F(" "));
	Serial.print(report.pressure, DEC); Serial.print(F(" "));
	#endif
	Serial.print(report.lowBattery, DEC); Serial.println(F(" "));
	Serial.flush();
	#endif
	
	rf12_sleep(RF12_WAKEUP);
	AckTimer at(ACK_TIME);
	for(int i=0; i<RETRIES && at.isArmed(); i++) {
		transmitReport();
		at.set(ACK_TIME);
		while(at.isArmed() && !at.expired()) {
			if(rf12_recvDone() && rf12_crc==0 && rf12_hdr == (RF12_HDR_DST | RF12_HDR_CTL | myNodeID)) {
				at.disarm();
			}
		}
	}
	delay(10);
	rf12_sleep(RF12_SLEEP);
}

void transmitReport() {
	while (!rf12_canSend()) {
		rf12_recvDone();
	}
	rf12_sendStart(RF12_HDR_ACK, &report, sizeof report);
}

void setup() {
	myNodeID = rf12_config();
	//myNodeID = rf12_initialize(NODEID, RF12_868MHZ, 128);
	
	#if SERIAL
	Serial.begin(57600);
	Serial.println(F("[RoomSens v1]"));
	#endif
	
	#if SHT11_PORT
	sht11.enableCRC();
	#endif
	
	#if LUX_PORT
	//lux.begin();
	//lux.setGain(luxHighGain);
	#endif
	
	#if BMP_PORT
	psensor.getCalibData();
	#endif
	
	mst.set(1);
}

void loop() {
	if(mst.expired() 
	#if PIR_PORT
	|| pir.pending
	#endif
	) {
		mst.set(CYCLE_TIME);
		takeMeasurement();
		sendReport();
	}
	
	Sleepy::loseSomeTime(mst.remaining());
}
