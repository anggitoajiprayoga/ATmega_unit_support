void push_traccar() {
  Serial.println();
  Serial.println("Push to Traccar");
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
  Serial.print(apn);
  byte count_gprs = 0;
  while (!modem.gprsConnect(apn, gprsUser, gprsPass) and count_gprs < 3)
  {
    count_gprs++;
    Serial.println("fail, count return: " + String(count_gprs));
  }
  Serial.println("Connected to GPRS: " + String(apn));
  Serial.println();

  if (modem.isGprsConnected()) {
    Serial.println("GPRS connected");
  }

  Serial.print("Connecting to ");
  Serial.print(server_traccar);
  Serial.print("...");
  if (client.connect(server_traccar, port_traccar)) {
    Serial.println("OK");
  }
  else {
    Serial.println("failed, automatic restart device to 5 second");
    delay(5000);
    rst();
  }

  client.print(String("GET /?id=") + cn + "&serial%20number=" + sn + "&lat=" + String(latd) + "&lon=" + String(longi) + "&altitude=" + String(altitude) + "&course=" + String(dir) + "&hdop=" + String(hdop.value()) + 
               "&speed=" + String(sped) + "&satellites=" + String(satelites) + "&engine%20condition=" + String(condition_realtime) + " HTTP/1.1\r\n" +
               "Host: " + server_traccar + "\r\n" +
               "Connection: keep-alive\r\n\r\n");
  Serial.print(String("GET /?id=") + cn + "&serial%20number=" + sn + "&lat=" + String(latd) + "&lon=" + String(longi) + "&altitude=" + String(altitude) + "&course=" + String(dir) + "&hdop=" + String(hdop.value()) + 
               "&speed=" + String(sped) + "&satellites=" + String(satelites) + "&engine%20condition=" + String(condition_realtime) + " HTTP/1.1\r\n" +
               "Host: " + server_traccar + "\r\n" +
               "Connection: keep-alive\r\n\r\n");

  uint32_t t = millis();
  // output server response
  while (client.available() && millis() - t < 5000) {
    Serial.print((char)client.read());
  }

  // Shutdown
  client.stop();
  Serial.println(F("Server disconnected"));

  modem.gprsDisconnect();
  Serial.println(F("GPRS disconnected"));
}
