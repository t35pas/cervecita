#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Arduino_JSON.h>
#include "SPIFFS.h"

// Replace with your network credentials
const char* ssid = "Mi red";
const char* password = "mortadela2";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Create an Event Source on /events
AsyncEventSource events("/events");

// Json Variable to Hold Sensor Readings
JSONVar readings;

// Timer variables
unsigned long lastTime = 0;  
unsigned long lastTimeTemperature = 0;
unsigned long lastTimeAcc = 0;
unsigned long gyroDelay = 10;
unsigned long temperatureDelay = 1000;
unsigned long accelerometerDelay = 200;
bool flag = true;
bool flag2 = true;

// Create a sensor object
Adafruit_MPU6050 mpu;

sensors_event_t a, g, temp;

float gyroX, gyroY, gyroZ;
float accX, accY, accZ;
float temperature;
int estado = 0;
bool boton = true,vEstado2 = false, vComenzar = false, vEstado3=false, vEstado4=false, vEstado5=false;
bool vEstado6 = false, vEstado7=false, vEstado8=false, vEstado9=false, vEstado10=false, vEstado11=false;

//Gyroscope sensor deviation
float gyroXerror = 0.07;
float gyroYerror = 0.03;
float gyroZerror = 0.01;

// Init MPU6050
void initMPU(){
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");
}

void initSPIFFS() {
  if (!SPIFFS.begin()) {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  Serial.println("SPIFFS mounted successfully");
}

// Initialize WiFi
void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("");
  Serial.println(WiFi.localIP());
}

String getGyroReadings(){
  mpu.getEvent(&a, &g, &temp);

  float gyroX_temp = g.gyro.x;
  if(abs(gyroX_temp) > gyroXerror)  {
    gyroX += gyroX_temp/50.00;
  }
  
  float gyroY_temp = g.gyro.y;
  if(abs(gyroY_temp) > gyroYerror) {
    gyroY += gyroY_temp/70.00;
  }

  float gyroZ_temp = g.gyro.z;
  if(abs(gyroZ_temp) > gyroZerror) {
    gyroZ += gyroZ_temp/90.00;
  }

  readings["gyroX"] = String(gyroX);
  readings["gyroY"] = String(gyroY);
  readings["gyroZ"] = String(gyroZ);

  String jsonString = JSON.stringify(readings);
  return jsonString;
}

String getAccReadings() {
  mpu.getEvent(&a, &g, &temp);
  // Get current acceleration values
  accX = a.acceleration.x;
  accY = a.acceleration.y;
  accZ = a.acceleration.z;
  readings["accX"] = String(accX);
  readings["accY"] = String(accY);
  readings["accZ"] = String(accZ);
  String accString = JSON.stringify (readings);
  return accString;
}

String getTemperature(){
  mpu.getEvent(&a, &g, &temp);
  temperature = temp.temperature;
  return String(temperature);
}

void setup() {
  Serial.begin(115200);
  initWiFi();
  initSPIFFS();
  //initMPU();

  // Handle Web Server
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });

  server.serveStatic("/", SPIFFS, "/");

  server.on("/reset", HTTP_GET, [](AsyncWebServerRequest *request){
    gyroX=0;
    gyroY=0;
    gyroZ=0;
    request->send(200, "text/plain", "OK");
  });

  server.on("/resetX", HTTP_GET, [](AsyncWebServerRequest *request){
    gyroX=0;
    request->send(200, "text/plain", "OK");
  });

  server.on("/resetY", HTTP_GET, [](AsyncWebServerRequest *request){
    gyroY=0;
    request->send(200, "text/plain", "OK");
  });

  server.on("/resetZ", HTTP_GET, [](AsyncWebServerRequest *request){
    gyroZ=0;
    request->send(200, "text/plain", "OK");
  });

  server.on("/siguiente", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("Función SIGUIENTE");
    Serial.println("Avanza de estado");
    avanzarEstado();
    request->send(200, "text/plain", "OK");
  });

  server.on("/vComenzar", HTTP_GET, [](AsyncWebServerRequest *request){
    vComenzar=true;
    request->send(200, "text/plain", "OK");
  });


  server.on("/vEstado2", HTTP_GET, [](AsyncWebServerRequest *request){
    vEstado2=true;
    request->send(200, "text/plain", "OK");
  });

  server.on("/vEstado3", HTTP_GET, [](AsyncWebServerRequest *request){
    vEstado3=true;
    request->send(200, "text/plain", "OK");
  });

  server.on("/vEstado4", HTTP_GET, [](AsyncWebServerRequest *request){
    vEstado4=true;
    request->send(200, "text/plain", "OK");
  });

  server.on("/vEstado5", HTTP_GET, [](AsyncWebServerRequest *request){
    vEstado5=true;
    request->send(200, "text/plain", "OK");
  });
  
  server.on("/vEstado5", HTTP_GET, [](AsyncWebServerRequest *request){
    vEstado5=true;
    request->send(200, "text/plain", "OK");
  });

  server.on("/vEstado6", HTTP_GET, [](AsyncWebServerRequest *request){
    vEstado6=true;
    request->send(200, "text/plain", "OK");
  });

  server.on("/vEstado7", HTTP_GET, [](AsyncWebServerRequest *request){
    vEstado7=true;
    request->send(200, "text/plain", "OK");
  });

  server.on("/vEstado8", HTTP_GET, [](AsyncWebServerRequest *request){
    vEstado8=true;
    request->send(200, "text/plain", "OK");
  });

  server.on("/vEstado9", HTTP_GET, [](AsyncWebServerRequest *request){
    vEstado9=true;
    request->send(200, "text/plain", "OK");
  });

  server.on("/vEstado10", HTTP_GET, [](AsyncWebServerRequest *request){
    vEstado10=true;
    request->send(200, "text/plain", "OK");
  });

  server.on("/vEstado11", HTTP_GET, [](AsyncWebServerRequest *request){
    vEstado11=true;
    request->send(200, "text/plain", "OK");
  });
  
  server.on("/resetear", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("Función RESETEAR");
    Serial.println("Resetea a estado 0");
    resetear();
    request->send(200, "text/plain", "OK");
  });

  server.on("/estado", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("Función ESTADO");
    Serial.println("Obtengo el estado");
    request->send(200, "text/plain",  (String) estado);
  });

  // Handle Web Server Events
  events.onConnect([](AsyncEventSourceClient *client){
    if(client->lastId()){
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    // send event with message "hello!", id current millis
    // and set reconnect delay to 1 second
    client->send("hello!", NULL, millis(), 10000);
  });
  server.addHandler(&events);

  server.begin();
}

void loop() {

  //Actualizar inputs
  //Actualizar estado
  //Escribir outputs

  
//En el loop verifico las variables que me hacen cambiar de estado
if(estado == 0 && vComenzar){
    estado1();
    estado=estado+1;
    vComenzar=false;
  }
if(estado == 1 && vEstado2){
    estado2();
    estado=estado+1;
    vEstado2=false;
  }
if(estado == 2 && vEstado3){
    estado3();
    estado=estado+1;
    vEstado3=false;
  }

if(estado == 3 && vEstado4){
  estado4();
  estado=estado+1;
  vEstado4=false;
  }

if(estado == 4 && vEstado5){
  estado5();
  estado=estado+1;
  vEstado5=false;
  }

if(estado == 5 && vEstado6){
  estado6();
  estado=estado+1;
  vEstado6=false;
  }

if(estado == 6 && vEstado7){
  estado7();
  estado=estado+1;
  vEstado7=false;
  }
if(estado == 7 && vEstado8){
  estado8();
  estado=estado+1;
  vEstado8=false;
  }

if(estado == 8 && vEstado9){
  estado9();
  estado=estado+1;
  vEstado9=false;
  }

if(estado == 9 && vEstado10){
  estado10();
  estado=estado+1;
  vEstado10=false;
  }

if(estado == 10 && vEstado11){
  estado11();
  estado=estado+1;
  vEstado11=false;
  }

//Si esta en el estado 1 prendo un timer
//  if(estado == 1 && flag){
//    lastTime = millis();
//    flag = false;
//  }
//  if (((millis() - lastTime) > 3000) && (estado==1) && flag2 && !flag) {
//    
//    avanzarEstado();
//    flag2=false;
//  }

//  if ((millis() - lastTime) > gyroDelay) {
//    // Send Events to the Web Server with the Sensor Readings
//    events.send(getGyroReadings().c_str(),"gyro_readings",millis());
//    lastTime = millis();
//  }
//  if ((millis() - lastTimeAcc) > accelerometerDelay) {
//    // Send Events to the Web Server with the Sensor Readings
//    events.send(getAccReadings().c_str(),"accelerometer_readings",millis());
//    lastTimeAcc = millis();
//  }
//  if ((millis() - lastTimeTemperature) > temperatureDelay) {
//    // Send Events to the Web Server with the Sensor Readings
//    events.send(getTemperature().c_str(),"temperature_reading",millis());
//    lastTimeTemperature = millis();
//  }
}

void avanzarEstado(){
  estado = estado + 1;
  switch (estado)
    {
    case 0: estado0(); break;
    case 1: estado1(); break;
    case 2: estado2(); break;
    case 3: estado3(); break;
    case 4: estado4(); break;
    default: break;
    }
}

void resetear(){
  estado = 0;
  estado0();  
  flag = true;
  flag2=true;
}

void estado0(){
  String text = "Pasos previos a iniciar proceso.";
  events.send(text.c_str(),"celdaEstado_reading",millis());
}

void estado1(){
  Serial.println("Estado 1");
  String text = "Comienza calentamineto de agua a 70°C.";
  events.send(text.c_str(),"celdaEstado_reading",millis());
}

void estado2(){
  Serial.println("Estado 2");
  String text = "Agua a 70°C. El agua está caliente, por favor, ingrese el grano.";
  events.send(text.c_str(),"celdaEstado_reading",millis());
  }

void estado3(){
  Serial.println("Estado 3");
  String text = "Comienza timer de 90'.";
  events.send(text.c_str(),"celdaEstado_reading",millis());
}

void estado4(){
  Serial.println("Estado 4");  
  String text = "Finaliza timer de 90'. Retire la tela de filtrado con el grano.";
  events.send(text.c_str(),"celdaEstado_reading",millis());
  }

void estado5(){
  Serial.println("Estado 5");
  String text = "Comienza calentamiento de mosto a 100°C";
  events.send(text.c_str(),"celdaEstado_reading",millis());
  }

void estado6(){
  Serial.println("Estado 6");
  String text = "Mosto a 100°C. Inicio timer de 60'.";
  events.send(text.c_str(),"celdaEstado_reading",millis());
  }

void estado7(){
  Serial.println("Estado 7");
  String text = "Mosto a 100°C. Inicio timer de 20' para lúpulo.";
  events.send(text.c_str(),"celdaEstado_reading",millis());
  }

void estado8(){
  Serial.println("Estado 8");
  String text = "Finaliza timer de 20'. El servo ingresa lúpulo.";
  events.send(text.c_str(),"celdaEstado_reading",millis());
  }

void estado9(){
  Serial.println("Estado 9");
  String text = "Finaliza timer de 60'. Se traspasa el mosto al tacho 2.";
  //Abre las válvulas de pasaje
  events.send(text.c_str(),"celdaEstado_reading",millis());
  }

void estado10(){
  Serial.println("Estado 10");
  String text = "El mosto se encuentra en el tacho 2. Ingrese las levaduras y cierre la tapa.";
  events.send(text.c_str(),"celdaEstado_reading",millis());
  }

void estado11(){
  Serial.println("Estado 11");
  String text = "Se inicia timer de 14 días.";
  events.send(text.c_str(),"celdaEstado_reading",millis());
  }
  
void habilitarSiguiente(){
  boton = true;
  events.send("true","botonSiguiente_reading",millis());
}

void deshabilitarSiguiente(){
  boton = false;
  events.send("false","botonSiguiente_reading",millis());
}
