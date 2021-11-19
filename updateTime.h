void time_update(){
//например, чтобы установить 27 января 2017 года в 12:56:
  timeClient.begin();
  delay(10);
  timeClient.update();
  rtc.adjust(DateTime(2017, 1, 27, timeClient.getHours(), timeClient.getMinutes(), timeClient.getSeconds()));
  timeClient.end();
  }
