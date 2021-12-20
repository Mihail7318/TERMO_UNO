
void computed(){
    tmp.requestTemperatures();
    TEMP1 = tmp.getTempC(sensor1);
    TEMP2 = tmp.getTempC(sensor2);
    TEMP3 = tmp.getTempC(sensor3);
    TEMP4 = tmp.getTempC(sensor4);
    Serial.print(TEMP4);
    Serial.println();
  }
