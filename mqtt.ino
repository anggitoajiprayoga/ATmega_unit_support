void setup_mqtt() {
  Serial.println();
  Serial.println("Searching for provider.");
  byte count_network = 0;
  while (!modem.waitForNetwork() and count_network < 3)
  {
    count_network++;
    Serial.println("fail, count return: " + String(count_network));
  }
  Serial.println("Connected to provider.");
  Serial.println("Signal Quality: " + String(modem.getSignalQuality()));
  Serial.println();

  Serial.println("Connecting to GPRS network.");
  byte count_gprs = 0;
  while (!modem.gprsConnect("internet", "", "") and count_gprs < 3)
  {
    count_gprs++;
    Serial.println("fail, count return: " + String(count_gprs));
  }
  Serial.println("Connected to GPRS: " + String(apn));
  Serial.println();

  mqtt.setServer((char*) broker.c_str(), port.toInt());
  mqtt.setCallback(callback);

  Serial.println("Connecting to MQTT Broker: " + String(broker));
  byte count_mqtt = 0;
  while (mqttConnect() == false and count_mqtt < 3) {
    count_mqtt++;
    Serial.println("fail, count return: " + String(count_mqtt));
  }
  Serial.print("mqtt state: " + String(mqtt.state()));
  Serial.println();

  if (count_mqtt == 3) {
    Serial.println("failed, automatic restart device to 5 second");
    delay(5000);
    rst();
  }
}

void loop_mqtt() {
  while (!mqtt.connected()) {
    Serial.println("disconnect, waiting to reconnect");
    reconnect();
    readGPS();
    json_parse();
    monitor_print();
  }
  mqtt.loop();
}

void pub(String topic, String _jsonDataString)
{
  char _jsonDataStringChar[_jsonDataString.length() + 1];
  char topicChar[topic.length() + 1];
  _jsonDataString.toCharArray(_jsonDataStringChar, _jsonDataString.length() + 1);
  topic.toCharArray(topicChar, topic.length() + 1);

  mqtt.publish(topicChar, _jsonDataStringChar);
  Serial.println();
  Serial.println(topic);
  Serial.println(_jsonDataString);
  Serial.println(_jsonDataString.length());
  Serial.println();
}

void reconnect() {
  //  while (!mqtt.connected()) {
  //    Serial.print("Attempting MQTT connection...");
  //    if (mqtt.connect(cn.c_str(), username.c_str(), password.c_str())) {
  //      Serial.println("connected");
  //      mqtt.publish("outTopic", "hello world");
  //      mqtt.subscribe("restart");
  //    }
  //    else {
  //      Serial.print("failed, rc=");
  //      Serial.print(mqtt.state());
  //      Serial.println(" try again in 1 seconds");
  //      delay(1000);
  //    }
  //    readGPS();
  //    json_parse();
  //    monitor_print();
  //  }
  reconnect_mqtt();
  readGPS();
  json_parse();
  monitor_print();
}

void reconnect_mqtt() {
  Serial.println();
  Serial.println("Searching for provider.");
  byte count_network = 0;
  while (!modem.waitForNetwork() and count_network < 3)
  {
    readGPS();
    json_parse();
    monitor_print();
    count_network++;
    Serial.println("fail, count return: " + String(count_network));
  }
  Serial.println("Connected to provider.");
  Serial.println("Signal Quality: " + String(modem.getSignalQuality()));
  Serial.println();

  Serial.println("Connecting to GPRS network.");
  byte count_gprs = 0;
  while (!modem.gprsConnect("internet", "", "") and count_gprs < 3)
  {
    readGPS();
    json_parse();
    monitor_print();
    count_gprs++;
    Serial.println("fail, count return: " + String(count_gprs));
  }
  Serial.println("Connected to GPRS: " + String(apn));
  Serial.println();

  mqtt.setServer((char*) broker.c_str(), port.toInt());
  mqtt.setCallback(callback);

  Serial.println("Connecting to MQTT Broker: " + String(broker));
  byte count_mqtt = 0;
  while (mqttConnect() == false and count_mqtt < 3) {
    readGPS();
    json_parse();
    monitor_print();
    count_mqtt++;
    Serial.println("fail, count return: " + String(count_mqtt));
  }
  Serial.print("mqtt state: " + String(mqtt.state()));
  Serial.println();

  if (count_mqtt == 3) {
    Serial.println("failed, automatic restart device to 5 second");
    delay(5000);
    rst();
  }
}

boolean mqttConnect()
{
  if (!mqtt.connect(cn.c_str(), username.c_str(), password.c_str()))
  {
    Serial.print(".");
    return false;
  }
  Serial.println("Connected to broker.");
  return mqtt.connected();
}

void callback(char* topic, byte* payload, unsigned int length)
{
  String pay = parsing_subscribe(topic, payload, length);
  Serial.print("Data Payload Subscribe : ");
  Serial.println(pay);
  if (pay == keyword_subscribe)
  {
    Serial.println("Oke - SUKSES");
    delay(2000);
    rst();
  }
}

String parsing_subscribe(char* topic, byte* payload, unsigned int length)
{
  String _receivedTopic = String(topic);
  String payloadString, parsedStringx;
  for (int i = 0; i < length; i++)
  {
    payloadString += char(payload[i]);
  }
  parsedStringx = payloadString;
  return parsedStringx;
}
