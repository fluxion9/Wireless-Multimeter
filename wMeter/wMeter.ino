#define avprobe A0
#define iprobe A1
#define rprobe A2
#define dvprobe A3
#include "ZMPT101B.h"
ZMPT101B voltageSensor(avprobe);
#include "ACS712.h"
ACS712  ACS(iprobe, 5.0, 1023, 100);
struct wMeter
{
    float dcv, acv, aca, res;
    void initializeSensors()
    {
        Serial.begin(9600);
        ACS.autoMidPoint();
        voltageSensor.calibrate();
    }
    void takeReadings()
    {
        dcv = measureVoltageDC();
        acv = voltageSensor.getVoltageAC();
        aca = ACS.mA_DC();
        res = measureRes(10000);
    }
    float measureVoltageDC(void)
    {
        float volt = analogRead(dvprobe);
        volt = (volt * 55.0) / 1023.0;
        return volt;
    }
    float measureRes(float R1)
    {
        float v2 = analogRead(rprobe);
        v2 = (v2 * 5.0) / 1023.0;
        return (v2 * R1) / (5.0 - v2);
    }
    void sendReadings(void)
    {
        String msg = "{dcv:" + String(dcv) + ",acv:" + String(acv) + ",aca:" + String(aca) + ",res:" + String(res) + "}";
        Serial.println(msg);
    }
    void runLoop()
    {
        takeReadings();
        sendReadings();
    }
};
wMeter meter;


void setup()
{
    meter.initializeSensors();
}
void loop()
{
    meter.runLoop();
}
