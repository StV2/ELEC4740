#include "Particle.h"
SYSTEM_MODE(SEMI_AUTOMATIC);

#include <string>
#include <iostream>
#include "math.h"

#define TEMP_PIN A1
#define MOTION_PIN D4
#define LIGHT_PIN A5
#define MIC_PIN A0
#define MOTION_LED_ON D6
#define MOTION_LED_OFF D5

void onDataReceived(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context);

const BleUuid serviceUuid("a5e35e50-ed11-4746-a030-8fb04379a526");
const BleUuid rxUuid("a5e35e51-ed11-4746-a030-8fb04379a526");
const BleUuid txUuid("a5e35e52-ed11-4746-a030-8fb04379a526");

BleCharacteristic txCharacteristic("tx", BleCharacteristicProperty::NOTIFY, txUuid, serviceUuid);
BleCharacteristic rxCharacteristic("rx", BleCharacteristicProperty::WRITE_WO_RSP, rxUuid, serviceUuid, onDataReceived, NULL);

void onDataReceived(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context) 
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

    BLE.addCharacteristic(txCharacteristic);
    BLE.addCharacteristic(rxCharacteristic);

    BleAdvertisingData data;
    data.appendServiceUUID(serviceUuid);
    BLE.advertise(&data);

    pinMode(LIGHT_PIN, INPUT);
    pinMode(TEMP_PIN, INPUT);
    pinMode(MOTION_PIN, INPUT);
}

double readLight()
{
    int brightness = analogRead(LIGHT_PIN);
    return 8.4715 * exp((double)(0.0025 * brightness));
}

double readTemp()
{
    return analogRead(TEMP_PIN);
    //return 97.795* log(tmpReading) -4.1948;
}

int readMotion()
{
    return digitalRead(MOTION_PIN);

        if (MOTION_PIN == HIGH)
        {
            digitalWrite(MOTION_LED_ON, HIGH); //turn green LED on
            digitalWrite(MOTION_LED_OFF, LOW);
        }
        else
        {
            digitalWrite(MOTION_LED_OFF, HIGH); //turn red LED on
            digitalWrite(MOTION_LED_ON, LOW);
        }
}

int readMic()
{
    return analogRead(MIC_PIN);
}
void loop() 
{
    String message = "SN2: \n";

    message += "Microphone: ";
    message.concat(readMic());
    message += " ADC\n";

    message += "Light: ";
    message.concat(readLight());
    message += " lux\n";

    message += "Temperature: ";
    message.concat(readTemp());
    message += " C\n";

    
    message += "Motion: ";
    message.concat(readMotion());
    message += "\n";
    //int motion = readMotion();

    /*if(motion == 1)
    {
        message += "Motion detected!\n";
    }
    else
    {
        message += "No motion detected.\n";
    }*/

    if (BLE.connected()) {
        txCharacteristic.setValue(message);
    }
    delay(2000);
}