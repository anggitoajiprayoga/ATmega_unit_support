#define TINY_GSM_MODEM_SIM7600
#include <TinyGPS++.h>
#include <Wire.h>
#include <ArduinoJson.h>
#include <TinyGsmClient.h>
#include <PubSubClient.h>
#include <Arduino.h>
#include <stdio.h>
#include <string.h>

#define Serial_gps Serial1
#define Serial_gsm Serial2
#define Serial_esp Serial3
#define power_key 29

const char apn[]      = "Internet";
const char gprsUser[] = "";
const char gprsPass[] = "";

const char server_traccar[] = "fms.ppa-mhu.net";
const int  port_traccar = 5055;

String id = "";
String username = "device-client";
String password = "mYdvJaMNFYP3twjN";
String broker = "ppa-mhu.net";
String port = "1883";

String keyword_subscribe = "restart";
String topic_sub = "restart";
String topic_liveLocations = "fms/liveLocations";
String topic_pay = "fms/unitSupport";
String liveLocations;
String pay_condition;
String pay_run, pay_idle, pay_off;

String final_payload;
String payload_;
String xcondition;
String api;

unsigned long previousMillisDebug;
String latd, longi, sped, times, dates, satelites;
String altitude, direction, dir;

String statusGPS;
int hourGPS;
int minuteGPS;
int secondGPS;
int count_second, count_traccar;

bool en_opr = 1;
bool en_idle = 1;
bool en_off = 1;

String cn;
String sn;
String condition_realtime;
String condition;
String time_full;
String shift;
String shift_realtime;
String time_on_hm;
String time_off_hm;
String duration_hm;
String time_on_idle;
String time_off_idle;
String duration_idle;
String time_on_standby;
String time_off_standby;
String duration_standby;
String time_full_realtime;
String time_on_hm_realtime;
String time_off_hm_realtime;
String duration_hm_realtime;
String time_on_idle_realtime;
String time_off_idle_realtime;
String duration_idle_realtime;
String time_on_standby_realtime;
String time_off_standby_realtime;
String duration_standby_realtime;
String fuel_voltage;

String Gsm;
String cmd;
String paketData;
String payload;
String final_hm, final_idle, final_standby;

unsigned long lastMillis = 0;
long interval_realtime = 6000;
long prev_check;
long prev_time, prev_task;
long prev_realtime;
long previousMillis;
long intfterval_update = 10000;

TinyGPSPlus gps;
TinyGPSCustom pdop(gps, "GPGSA", 15); // $GPGSA sentence, 15th element
TinyGPSCustom hdop(gps, "GPGSA", 16); // $GPGSA sentence, 16th element
TinyGPSCustom vdop(gps, "GPGSA", 17); // $GPGSA sentence, 17th element
TinyGsm modem(Serial_gsm);
TinyGsmClient client(modem);
PubSubClient mqtt(client);

void(*rst) (void) = 0;
void setup()
{
  Serial.begin(57600);
  Serial_gps.begin(9600);
  Serial_esp.begin(57600);
  setup_gsm();
  setup_mqtt();
}

void loop() {
  if ((millis() - prev_task) > 5000) {
    loop_mqtt();
    prev_task = millis();
  }
  readGPS();
  json_parse();
  mainn();
  monitor_print();
}
