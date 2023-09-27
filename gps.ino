void readGPS() {
  while (Serial_gps.available() > 0)
  {
    gps.encode(Serial_gps.read());
    if (gps.location.isUpdated())
    {
      if (gps.location.isValid()) {
        latd = String(gps.location.lat(), 6);
        longi = String(gps.location.lng(), 6);
      }
      if (gps.course.isValid()) {
        dir = String(gps.course.deg(), 3);
      }
      if (gps.altitude.isValid()) {
        altitude = String(gps.altitude.meters(), 3);
      }
    }
    if (gps.speed.isValid()) {
      sped = String(gps.speed.kmph(), 1);
    }
  }

  if (latd == "") {
    latd = "0";
  }
  if (longi == "") {
    longi = "0";
  }
  if (dir == "") {
    dir = "0";
  }
  if (altitude == "") {
    altitude = "0";
  }
  if (sped == "") {
    sped = "0";
  }
}
