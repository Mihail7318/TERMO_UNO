void network(){
   HTTPClient http;    //Объявить объект класса HttpClient

  http.begin("http://jsonplaceholder.typicode.com/users");      //Укажите адрес запроса
  http.addHeader("Content-Type", "text/plain");  //Укажите заголовок типа содержимого

  int httpCode = http.POST("Message from ESP8266");   //Отправьте запрос
  String payload = http.getString();                  //Получите полезную нагрузку ответа

  Serial.println(httpCode);   //Распечатать код возврата HTTP
  Serial.println(payload);    //Полезная нагрузка для ответа на запрос печати

  http.end();  //Закрыть соединение



  }
