/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#line 1 "/home/stephen/Documents/elec4740/final_final_final/ch/ch/src/ch.ino"
#include "Particle.h"
void onDataReceivedSN1(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context);
void onDataReceivedSN2(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context);
void setup();
void loop();
#line 2 "/home/stephen/Documents/elec4740/final_final_final/ch/ch/src/ch.ino"
SYSTEM_MODE(SEMI_AUTOMATIC);

const BleUuid serviceUuidSN1("47d866da-fa60-4bfb-b10e-35aecbd85ba1");
const BleUuid rxUuidSN1("47d866db-fa60-4bfb-b10e-35aecbd85ba1");
const BleUuid txUuidSN1("47d866dc-fa60-4bfb-b10e-35aecbd85ba1");

const BleUuid serviceUuidSN2("a5e35e50-ed11-4746-a030-8fb04379a526");
const BleUuid rxUuidSN2("a5e35e51-ed11-4746-a030-8fb04379a526");
const BleUuid txUuidSN2("a5e35e52-ed11-4746-a030-8fb04379a526");

#define SCAN_RESULT_COUNT 255

BleScanResult scanResults[SCAN_RESULT_COUNT];

BleCharacteristic peerTxCharacteristicSN1;
BleCharacteristic peerRxCharacteristicSN1;
BlePeerDevice peerSN1;

BleCharacteristic peerTxCharacteristicSN2;
BleCharacteristic peerRxCharacteristicSN2;
BlePeerDevice peerSN2;

const unsigned long SCAN_PERIOD_MS = 2000;
unsigned long lastScan = 0;

void onDataReceivedSN1(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context) 
{
    for (size_t i = 0; i < len; i++) 
    {
        Serial.write(data[i]);
    }
}

void onDataReceivedSN2(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context) 
{
    for (size_t i = 0; i < len; i++) 
    {
        Serial.write(data[i]);
    }
}

void setup() 
{
    Serial.begin();
	BLE.on();
	peerTxCharacteristicSN1.onDataReceived(onDataReceivedSN1, &peerTxCharacteristicSN1);
	peerTxCharacteristicSN2.onDataReceived(onDataReceivedSN2, &peerTxCharacteristicSN2);                 
}

void loop()
{
    if(!(peerSN1.connected() && peerSN2.connected()))
    {
    	if(millis() - lastScan >= SCAN_PERIOD_MS) 
        {
    		// Time to scan
    		lastScan = millis();

    		size_t count = BLE.scan(scanResults, SCAN_RESULT_COUNT);

			  if(count > 0) 
            {
				for(uint8_t i = 0; i < count; i++) 
                {
					BleUuid foundServiceUuid;
					size_t svcCount = scanResults[i].advertisingData.serviceUUID(&foundServiceUuid, 1);
                    
					if(svcCount > 0 && foundServiceUuid == serviceUuidSN1) 
                    {
                        Serial.println("SN1 found!");
						peerSN1 = BLE.connect(scanResults[i].address);
						if (peerSN1.connected()) 
                        {
                            Serial.println("SN1 connected!");
							peerSN1.getCharacteristicByUUID(peerTxCharacteristicSN1, txUuidSN1);
							peerSN1.getCharacteristicByUUID(peerRxCharacteristicSN1, rxUuidSN1);
						}
					}
                    if(svcCount > 0 && foundServiceUuid == serviceUuidSN2) 
                    {
                        Serial.println("SN2 found!");
						peerSN2 = BLE.connect(scanResults[i].address);
						if (peerSN2.connected()) 
                        {
                            Serial.println("SN2 connected!");
							peerSN2.getCharacteristicByUUID(peerTxCharacteristicSN2, txUuidSN2);
							peerSN2.getCharacteristicByUUID(peerRxCharacteristicSN2, rxUuidSN2);
						}
					}
				}
			}
    }
  }
}