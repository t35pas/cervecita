#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Arduino_JSON.h>
#include "SPIFFS.h"
#include <OneWire.h>            //Sensor T
#include <DallasTemperature.h>  //Sensor T
#include <ESP32Servo.h> //SERVO MOTOR
#include <PIDController.hpp> //PID

// Replace with your network credentials
const char* ssid = "Mi red";
const char* password = "mortadela2";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Create an Event Source on /events
AsyncEventSource events("/events");

// Timer variables
//unsigned long lastTime = 0;  
//unsigned long lastTimeTemperature = 0;
//unsigned long lastTimeAcc = 0;
//unsigned long gyroDelay = 10;
//unsigned long temperatureDelay = 1000;
//unsigned long accelerometerDelay = 200;
//bool flag = true;
//bool flag2 = true;
unsigned long lastTimeTimer90 = 0; //Timer del estado 3
unsigned long lastTimeTimer60 = 0; //Timer del estado 6
unsigned long lastTimeTimer45 = 0;
unsigned long lastTimeTimer14 = 0; //Timer del estado 11
bool flagTimer90 = false;
bool flagTimer60 = false;
bool flagTimer45 = false;
bool flagTimer14 = false;
bool flagTimer20 = false;
int contadorLupulo = 0;


//Output del sistema
//PIN 4 Sensor temperatura
//PIN 12 ServoMotor
const int pinV1 = 13;  //Valvula llenado Tacho 1
const int pinV2 = 16;  //Valvula vaciado Tacho 1
const int pinV3 = 17;  //Valvula llenado Tacho 2
const int pinT = 4; //PIN de temperatura
const int pinV4 = 19;  //el pin 4 se usa en sensor de temperatura // Valvula escape Tacho 2
const int pinSN1 = 21; //Sensor Nivel Tacho 1 
const int pinB1 = 22;  //Bomba de traspaso de tachos
const int pinC1 = 23;  //Calentador Tacho 1
const int pinE1 = 24;  //Enfriador Tacho 2
const int pinSM1 = 25; //Servo Motor Tacho 1

// Create a sensor object
Adafruit_MPU6050 mpu;

float temperature;
int estado = 0;
bool boton = true,vEstado2 = false, vComenzar = false, vEstado3=false, vEstado4=false, vEstado5=false;
bool vEstado6 = false, vEstado7=false, vEstado8=false, vEstado9=false, vEstado10=false, vEstado11=false;
bool PIDstatus = false, vEstado12=false;

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

//Sensor de Temperatura
// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(4); //4 es G4 en el ESP32 PIN 4
// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

//Servo Motor
Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards
int posSM1 = 0;    // variable to store the servo position

//PID
PID::PIDParameters<double> parameters(4.0, 0.2, 1); //Kp, Ki y Kd
PID::PIDController<double> pidController(parameters);

/////////////////////////////////////////////////////////
void setup() {
  Serial.begin(115200);
  initWiFi();
  initSPIFFS();

  // Handle Web Server
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index2.html", "text/html");
  });

  server.serveStatic("/", SPIFFS, "/");

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

  server.on("/vEstado12", HTTP_GET, [](AsyncWebServerRequest *request){
    vEstado12=true;
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

  server.on("/data/img/estado1.jpg", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("Función obtener imagen");
    //request->send(SPIFFS, "/data/img/estado1.jpg");
    request->send(SPIFFS, "/data/img/estado1.jpg", "text/plain");
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

  //Sensor de temperatura
  // Start the DS18B20 sensor
  sensors.begin();

  //Define output del sistema
  pinMode(pinV1, OUTPUT);  //Valvula llenado Tacho 1
  pinMode(pinV2, OUTPUT);  //Valvula vaciado Tacho 1
  pinMode(pinV3, OUTPUT);  //Valvula llenado Tacho 2
  pinMode(pinV4, OUTPUT);  //el pin 4 se usa en sensor de temperatura // Valvula escape Tacho 2
  pinMode(pinSN1, OUTPUT); //Sensor Nivel Tacho 1 
  pinMode(pinB1, OUTPUT);  //Bomba de traspaso de tachos
  pinMode(pinC1, OUTPUT);  //Calentador Tacho 1
  pinMode(pinE1, OUTPUT);  //Enfriador Tacho 2
  pinMode(pinSM1, OUTPUT); //Servo Motor Tacho 1

  //SM01
  myservo.attach(12);  // attaches the servo on pin 11 to the servo object
  //servo no puede ir en pin 11
  
  //PID
  pidController.Input = analogRead(pinT);
}

void loop() {

  //Actualizar inputs
  //Actualizar estado
  //Escribir outputs

  actualizarEstado();
  if(PIDstatus)
    controlarPID();

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
}

void actualizarEstado(){
  //En el loop verifico las variables que me hacen cambiar de estado
  if(estado == 0 && vComenzar){
      estado1();
      estado=estado+1;
      vComenzar=false;
  }
  if(estado == 1 && (vEstado2 || (sensors.getTempCByIndex(0) > 70))){
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
  if(estado == 5 && (vEstado6 || (sensors.getTempCByIndex(0) > 99))){
    estado6();
    estado=estado+1;
    vEstado6=false;
  }
  if(estado == 6 && vEstado7){
    estado7();
    estado=estado+1;
    vEstado7=false;
  }
  if(estado == 6 && vEstado8){
    estado8();
    estado=estado+2;
    vEstado8=false;
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
  if(estado == 11 && vEstado12){
    estado12();
    estado=estado+1;
    vEstado12=false;
  }

  //Timers
  if (((millis() - lastTimeTimer90) > 5000) && (estado==3) && flagTimer90 ) { //5400000 son 90' TODO
    flagTimer90=false;
    Serial.println("Estado 3 - Finaliza Timer 90'");
    vEstado4 = true;
  }
  if (((millis() - lastTimeTimer60) > 5000) && (estado==6) && flagTimer20 ) { //3600000 son 60', ojo que son 90 ahora TODO
    flagTimer20=false;
    Serial.println("Estado 8 - Finaliza Timer 20'");
    vEstado8 = true; //El estado7 es intermedio para tirar lúpulo.
  }
  if (((millis() - lastTimeTimer60) > 10000) && (estado==8) && flagTimer60 ) { //3600000 son 60', ojo que son 90 ahora TODO
    flagTimer60=false;
    Serial.println("Estado 6 - Finaliza Timer 60'");
    vEstado9 = true;
  }
  if (((millis() - lastTimeTimer14) > 5000) && (estado==11) && flagTimer14 ) { //TODO Acomodar timer
    flagTimer14=false;
    Serial.println("Estado 11 - Finaliza Timer 14 días.");
    vEstado12 = true;
  }
}

void controlarPID(){
  pidController.Input = analogRead(pinT);
  pidController.Update();
  analogWrite(pinC1, pidController.Output);
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
  //flag = true;
  //flag2=true;
  contadorLupulo=0;
  lastTimeTimer90 = 0; //Timer del estado 3
  lastTimeTimer60 = 0; //Timer del estado ..
  lastTimeTimer14 = 0; //Timer del estado ..
  flagTimer90 = false;
  flagTimer60 = false;
  flagTimer20 = false;
  flagTimer14 = false;
  boton = true,vEstado2 = false, vComenzar = false, vEstado3=false, vEstado4=false, vEstado5=false;
  vEstado6 = false, vEstado7=false, vEstado8=false, vEstado9=false, vEstado10=false, vEstado11=false;
  vEstado12 = false;
}

void estado0(){
  String text = "Pasos previos a iniciar proceso.";
  events.send(text.c_str(),"celdaEstado_reading",millis());
  events.send("0","imagenEstado",millis());
}

void estado1(){
  Serial.println("Estado 1");
  String text = "Comienza calentamineto de agua a 70°C.";
  events.send(text.c_str(),"celdaEstado_reading",millis());
  //Prender calentador
  //Lanzar PID70
  pidController.Setpoint = 70;
  pidController.TurnOn();
  PIDstatus=true;
  events.send("1","imagenEstado",millis());
}

void estado2(){
  Serial.println("Estado 2");
  String text = "Agua a 70°C. El agua está caliente, por favor, ingrese el grano.";
  events.send(text.c_str(),"celdaEstado_reading",millis());
  events.send("2","imagenEstado",millis());
  }

void estado3(){
  Serial.println("Estado 3");
  String text = "Comienza timer de 90'.";
  events.send(text.c_str(),"celdaEstado_reading",millis());
  
  //Comenzar Timer de 90' minutos
  lastTimeTimer90 = millis();
  flagTimer90 = true;
  events.send("3","imagenEstado",millis());
}

void estado4(){
  Serial.println("Estado 4");  
  String text = "Finaliza timer de 90'. Retire la tela de filtrado con el grano.";
  events.send(text.c_str(),"celdaEstado_reading",millis());
  
  //Apago PID
  pidController.TurnOff();
  PIDstatus=false;
  events.send("4","imagenEstado",millis());
  }

void estado5(){
  Serial.println("Estado 5");
  String text = "Comienza calentamiento de mosto a 100°C";
  events.send(text.c_str(),"celdaEstado_reading",millis());
  //Lanzar PID100
  pidController.Setpoint = 100;
  pidController.TurnOn();
  PIDstatus=true;
  events.send("5","imagenEstado",millis());
  }

void estado6(){
  Serial.println("Estado 6");
  String text = "Mosto a 100°C. Inicio timer de 60'.";
  events.send(text.c_str(),"celdaEstado_reading",millis());

  //Comenzar Timer de 60' minutos
  lastTimeTimer60 = millis();
  flagTimer60 = true;
  flagTimer20 = true;
  events.send("6","imagenEstado",millis());
  }

void estado7(){
  Serial.println("Estado 7");
  String text = "Mosto a 100°C. Inicio timer de 45' para lúpulo.";
  events.send(text.c_str(),"celdaEstado_reading",millis());

  //Muevo servo a posición 1, cae primer parte del lúpulo
  posSM1 = 60;
  myservo.write(posSM1);
  
  //Iniciar Timer 45'
  //Tiene que activarlo 2 veces
  if(contadorLupulo < 3){
    lastTimeTimer45 = millis();
    flagTimer45 = true;
    contadorLupulo=contadorLupulo+1;
  }
  events.send("7","imagenEstado",millis());
}

void estado8(){
  Serial.println("Estado 8");
  String text = "Han pasado 20'. El servo ingresa lúpulo.";
  events.send(text.c_str(),"celdaEstado_reading",millis());
  
  //Activar servo
  //Muevo servo a posición 1, cae primer parte del lúpulo
  posSM1=posSM1+60;
  myservo.write(posSM1);
  events.send("8","imagenEstado",millis());
}

void estado9(){
  Serial.println("Estado 9");
  String text = "Finaliza timer de 60'. Se traspasa el mosto al tacho 2.";
  events.send(text.c_str(),"celdaEstado_reading",millis());
  
  //Apago PID
  pidController.TurnOff();
  PIDstatus=false;
  
  //Abre las válvulas de pasaje
  

  //Ver si tengo que reiniciar el servo
  //Reinicio contador de lúpulo
  contadorLupulo=0;
  events.send("9","imagenEstado",millis());
  }

void estado10(){
  Serial.println("Estado 10");
  String text = "El mosto se encuentra en el tacho 2. Ingrese las levaduras y cierre la tapa.";
  events.send(text.c_str(),"celdaEstado_reading",millis());
  events.send("10","imagenEstado",millis());
  }

void estado11(){
  Serial.println("Estado 11");
  String text = "Se inicia timer de 14 días.";
  events.send(text.c_str(),"celdaEstado_reading",millis());

  //Inicio timer 14 días
  lastTimeTimer14 = millis();
  flagTimer14 = true;
  events.send("11","imagenEstado",millis());
  }

void estado12(){
  Serial.println("Estado 12");
  String text = "Fin! A beber.";
  events.send(text.c_str(),"celdaEstado_reading",millis());
  events.send("12","imagenEstado",millis());
  }
