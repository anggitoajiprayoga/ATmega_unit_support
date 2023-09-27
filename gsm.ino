/******************** GSM Communication Starts ********************/
void setup_gsm() {
  Serial_gsm.begin(115200);
  Serial.println("Initializing modem...");
  pinMode(power_key, OUTPUT);
  digitalWrite(power_key, LOW);
  delay(500);
  digitalWrite(power_key, HIGH);
  delay(1000);
  modem.restart();
  String modemInfo = modem.getModemInfo();
  Serial.print("Modem Info: ");
  Serial.println(modemInfo);
}

void ShowSerialData() {
  while (Serial_gsm.available() != 0) {
    //    Serial.write(Serial_gsm.read());
    Gsm = Serial_gsm.readStringUntil('\n');
  }
}
