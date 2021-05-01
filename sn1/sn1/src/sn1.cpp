/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#line 1 "/home/stephen/Documents/elec4740/final_final_final/sn1/sn1/src/sn1.ino"
#include "Particle.h"
void setup();
double readLight();
unsigned int readDistance();
void loop();
#line 2 "/home/stephen/Documents/elec4740/final_final_final/sn1/sn1/src/sn1.ino"
SYSTEM_MODE(SEMI_AUTOMATIC);

#include <string>
#include <iostream>
#include "math.h"
#include "DHT.h"

//pins
#define DHT_PIN D2
#define TRG_PIN D8
#define ECH_PIN D7
#define LIGHT_PIN A1

//Constants
#define SOUND 340.29
#define DHTTYPE 11
DHT dht(DHT_PIN, DHTTYPE);

void onDataReceived(const uint8_t* data, size_t len, const BlePeerDevice& peer, void* context);

const BleUuid serviceUuid("47d866da-fa60-4bfb-b10e-35aecbd85ba1");
const BleUuid rxUuid("47d866db-fa60-4bfb-b10e-35aecbd85ba1");
const BleUuid txUuid("47d866dc-fa60-4bfb-b10e-35aecbd85ba1");

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
    //Ultrasonic Pins
    pinMode(TRG_PIN, OUTPUT); // Sets the trigPin as an Output
    pinMode(ECH_PIN, INPUT);
    dht.begin();
}

double readLight()
{
    int brightness = analogRead(LIGHT_PIN);
    return 97.795* log(brightness) -4.1948;
}

unsigned int readDistance()
{
    digitalWrite(TRG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRG_PIN, LOW);
    long duration = pulseIn(ECH_PIN, HIGH);
    return (unsigned int) (0.5 * duration * SOUND * 1e-3);
}

void loop() 
{
    String message = "SN1: \n";

    message += "Light: ";
    message.concat(readLight());
    message += " lux\n";

    message += "Distance: ";
    message.concat(readDistance());
    message += " mm\n";

    float humidity;
    float temperature;
    //do
    //{
        humidity = dht.readHumidity();
        temperature = dht.readTemperature();
    //}
    //while(isnan(humidity) || isnan(temperature));
    
    message += "Humidity: ";
    message.concat(humidity);
    message += "% Temperature: ";
    message.concat(temperature);
    message += " C\n";

    if (BLE.connected()) {

        txCharacteristic.setValue(message);
    }
    delay(1000);
}