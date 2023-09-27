void monitor_print() {
  if ((unsigned long)(millis() - previousMillisDebug) >= 5000) {
    previousMillisDebug = millis();

    Serial.println();
    Serial.println("cn: " + cn + " | sn: " + sn);
    Serial.println("condition: " + condition + " | condition_realtime: " + condition_realtime + " | xcondition: " + xcondition);
    Serial.println("time_full: " + time_full + " | time_full_realtime: " + time_full_realtime);
    Serial.println("on_opr: " + time_on_hm + " | off_opr: " + time_off_hm + " | duration_opr: " + duration_hm);
    Serial.println("on_idle: " + time_on_idle + " | off_idle: " + time_off_idle + " | duration_idle: " + duration_idle);
    Serial.println("on_off: " + time_on_standby + " | off_idle: " + time_off_standby + " | duration_off: " + duration_standby);
    Serial.println("on_opr_realtime: " + time_on_hm_realtime + " | off_opr_realtime: " + time_off_hm_realtime + " | duration_opr_realtime: " + duration_hm_realtime);
    Serial.println("on_idle_realtime: " + time_on_idle_realtime + " | off_idle_realtime: " + time_off_idle_realtime + " | duration_idle_realtime: " + duration_idle_realtime);
    Serial.println("on_off_realtime: " + time_on_standby_realtime + " | off_off_realtime: " + time_off_standby_realtime + " | duration_standby_realtime: " + duration_standby_realtime);
    Serial.println("longd: " + String(longi) + " | latd: " + String(latd) + " | altitude: " + String(altitude) + " | dir: " + String(dir) + " | hdop: " + String(hdop.value()) + " | Speed: " + String(sped) + " km/h");
    Serial.println("packet: " + paketData);
    Serial.println("liveLocations: " + liveLocations);
    Serial.print("mqtt state: " + String(mqtt.state()));
    Serial.println();

    String packet = "{\"longitude\":\"" + String(longi) + "\",\"latitude\":\"" + String(latd) + "\",\"altitude\":\"" + String(altitude) + "\",\"heading\":\"" + String(dir) + "\",\"speed\":\"" + String(gps.speed.kmph(), 1) + "\"}";
    Serial_esp.println(packet);
  }
}

void json_parse() {
  while (Serial_esp.available()) {
    paketData = Serial_esp.readStringUntil('\n');
  }

  StaticJsonDocument<1024> doc;
  String input = paketData;
  deserializeJson(doc, input);
  JsonObject obj = doc.as<JsonObject>();

  cn = obj["cn"].as<String>();
  sn = obj["sn"].as<String>();
  condition = obj["condition"].as<String>();
  time_full = obj["time_full"].as<String>();
  time_on_hm = obj["time_on_hm"].as<String>();
  time_off_hm = obj["time_off_hm"].as<String>();
  duration_hm = obj["duration_hm"].as<String>();
  time_on_idle = obj["time_on_idle"].as<String>();
  time_off_idle = obj["time_off_idle"].as<String>();
  duration_idle = obj["duration_idle"].as<String>();
  shift = obj["shift"].as<String>();
  time_on_standby = obj["time_on_standby"].as<String>();
  time_off_standby = obj["time_off_standby"].as<String>();
  duration_standby = obj["duration_standby"].as<String>();

  condition_realtime = obj["condition_http"].as<String>();
  time_full_realtime = obj["time_full_http"].as<String>();
  time_on_hm_realtime = obj["time_on_hm_http"].as<String>();
  time_off_hm_realtime = obj["time_off_hm_http"].as<String>();
  duration_hm_realtime = obj["duration_hm_http"].as<String>();
  time_on_idle_realtime = obj["time_on_idle_http"].as<String>();
  time_off_idle_realtime = obj["time_off_idle_http"].as<String>();
  duration_idle_realtime = obj["duration_idle_http"].as<String>();
  time_on_standby_realtime = obj["time_on_standby_http"].as<String>();
  time_off_standby_realtime = obj["time_off_standby_http"].as<String>();
  duration_hm_realtime = obj["duration_hm_http"].as<String>();
  duration_idle_realtime = obj["duration_idle_http"].as<String>();
  duration_standby_realtime = obj["duration_standby_http"].as<String>();

  if (cn == "null") {
    cn = "";
  }
  if (sn == "null") {
    sn = "";
  }
  if (condition == "null") {
    condition = "";
  }
  if (time_off_hm == "null") {
    time_off_hm = "";
  }
  if (time_off_idle == "null") {
    time_off_idle = "";
  }
  if (time_off_standby == "null") {
    time_off_standby = "";
  }
}

void mainn() {
  if (condition != "" and xcondition != condition) {
    if (condition == "engine operation") {
      if (time_off_hm != "") {
        pay_run = "{\"sn\":\"" + sn + "\",\"cn\":\"" + cn + "\",\"dateTime\":\"" + time_full + "\",\"timeZone\":\"+8:00\", \"cdt\":\"" + condition + "\",\"dst\":\"" + time_on_hm + "\",\"dsp\":\"" + time_off_hm + "\",\"teop\":\"" + duration_hm + "\",\"tei\":\"00:00:00\",\"teo\":\"00:00:00\",\"data\":{\"dateTime\":\"" + time_full + "\",\"timeZone\":\"+8:00\",\"lat\":" + latd + ",\"lng\":" + longi + ",\"alt\":" + altitude + ",\"spd\":" + sped + ",\"hdg\":" + dir + "}}";
        Serial.println("pay_condition: " + pay_run);
        pub(topic_pay, pay_run);
        Serial.println("POST PAYLOAD engRun");
        xcondition = condition;
      }
    }
    if (condition == "engine idle") {
      if (time_off_idle != "") {
        pay_idle = "{\"sn\":\"" + sn + "\",\"cn\":\"" + cn + "\",\"dateTime\":\"" + time_full + "\",\"timeZone\":\"+8:00\", \"cdt\":\"" + condition + "\",\"dst\":\"" + time_on_idle + "\",\"dsp\":\"" + time_off_idle + "\",\"teop\":\"" + duration_idle + "\",\"tei\":\"00:00:00\",\"teo\":\"00:00:00\",\"data\":{\"dateTime\":\"" + time_full + "\",\"timeZone\":\"+8:00\",\"lat\":" + latd + ",\"lng\":" + longi + ",\"alt\":" + altitude + ",\"spd\":" + sped + ",\"hdg\":" + dir + "}}";
        Serial.println("pay_condition: " + pay_idle);
        pub(topic_pay, pay_idle);
        Serial.println("POST PAYLOAD engIdle");
        xcondition = condition;
      }
    }
    if (condition == "engine off") {
      if (time_off_standby != "") {
        pay_off = "{\"sn\":\"" + sn + "\",\"cn\":\"" + cn + "\",\"dateTime\":\"" + time_full + "\",\"timeZone\":\"+8:00\", \"cdt\":\"" + condition + "\",\"dst\":\"" + time_on_standby + "\",\"dsp\":\"" + time_off_standby + "\",\"teop\":\"" + duration_standby + "\",\"tei\":\"00:00:00\",\"teo\":\"00:00:00\",\"data\":{\"dateTime\":\"" + time_full + "\",\"timeZone\":\"+8:00\",\"lat\":" + latd + ",\"lng\":" + longi + ",\"alt\":" + altitude + ",\"spd\":" + sped + ",\"hdg\":" + dir + "}}";
        Serial.println("pay_condition: " + pay_off);
        pub(topic_pay, pay_off);
        Serial.println("POST PAYLOAD engOff");
        xcondition = condition;
      }
    }
  }

  if (millis() - lastMillis > 1000) {
    count_second++;
    count_traccar++;
    lastMillis = millis();
  }

  if (count_second > 5) {
    if (time_full_realtime != "null") {
      liveLocations = "{\"sn\":\"" + sn + "\",\"cn\":\"" + cn + "\",\"dateTime\":\"" + time_full_realtime + "\",\"timeZone\":\"+8:00\", \"lat\":" + latd + ",\"lng\":" + longi + ",\"alt\":" + altitude + ",\"spd\":" + sped + ",\"hdg\":" + dir + "}";
      pub(topic_liveLocations, liveLocations);
      Serial.println("POST PAYLOAD Realtime");
      count_second = 0;
    }
  }

  if (count_traccar > 60) {
    push_traccar();
    Serial.println("Push traccar");
    count_traccar = 0;
  }

}
