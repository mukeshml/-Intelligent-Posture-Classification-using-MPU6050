#include <WiFi.h>
#include <WebServer.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>


Adafruit_MPU6050 mpu;

WebServer server(80);

const char* ssid = "wifi_test";         /*Enter Your SSID*/
const char* password = "passsword"; /*Enter Your Password*/

float x_acceleration, y_acceleration, z_acceleration;
float x_gyro, y_gyro, z_gyro;
float tempe;

// void MainPage() {
//   String html_page = html_page;              /*Read The HTML Page*/
//   server.send(200, "text/html", html_page);  /*Send the code to the web server*/
// }

// void Web_MPU6050() {
//   String data = "[\""+String(x_acceleration)+"\",\""+String(y_acceleration)+"\",\""+String(z_acceleration)+"\",\""+String(x_gyro)+"\",\""+String(y_gyro)+"\",\""+String(z_gyro)+"\",\""+String(tempe)+"\"]";
//   server.send(200, "text/plane", data);
// }
String server = "http://maker.ifttt.com";

String eventName = "ESP32";

String IFTTT_Key = "csRLNPdx_F-jpBBNH2nFbZYTIiOwyPo3V63dDTiD3Pe";

String IFTTTUrl="https://maker.ifttt.com/trigger/ESP32/with/key/csRLNPdx_F-jpBBNH2nFbZYTIiOwyPo3V63dDTiD3Pe";



int value1;

int value2;

int value3;

void setup(void){
  Serial.begin(115200);                 /*Set the baudrate to 115200*/
  WiFi.mode(WIFI_STA);                  /*Set the WiFi in STA Mode*/
  WiFi.begin(ssid, password);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  delay(1000);                         /*Wait for 1000mS*/
  while(WiFi.waitForConnectResult() != WL_CONNECTED){Serial.print(".");} /*Wait while connecting to WiFi*/
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("Your Local IP address is: ");
  Serial.println(WiFi.localIP());       /*Print the Local IP*/
 

  Serial.println("Adafruit MPU6050 test!");
  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }



  // Serial.println("");
  // delay(100);

  // // server.on("/", MainPage);             /*Display the Web/HTML Page*/
  // server.on("/read_Web_MPU6050", Web_MPU6050); /*Display the updated Distance value(CM and INCH)*/
  // server.begin();                       /*Start Server*/
  // delay(1000);                          /*Wait for 1000mS*/
 
}
void sendDataToSheet(void)

{

  String url = server + "/trigger/" + eventName + "/with/key/" + IFTTT_Key + "?value1=" + String((int)value1) + "&value2="+String((int)value2) +"&value3=" + String((int)value3);  

  Serial.println(url);

  //Start to send data to IFTTT

  HTTPClient http;

  Serial.print("[HTTP] begin...\n");

  http.begin(url); //HTTP



  Serial.print("[HTTP] GET...\n");

  // start connection and send HTTP header

  int httpCode = http.GET();

  // httpCode will be negative on error

  if(httpCode > 0) {

    // HTTP header has been send and Server response header has been handled

    Serial.printf("[HTTP] GET... code: %d\n", httpCode);

    // file found at server

    if(httpCode == HTTP_CODE_OK) {

      String payload = http.getString();

      Serial.println(payload);

    }

  } else {

    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());

  }

  http.end();



}


void loop(void){
  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
 
  x_acceleration = a.acceleration.x;
  y_acceleration = a.acceleration.y;
  z_acceleration = a.acceleration.z;
  x_gyro = g.gyro.x;
  y_gyro = g.gyro.y;
  z_gyro = g.gyro.z;
  tempe = temp.temperature;
 
  value1=x_acceleration;
  value2=y_acceleration;
  value3=z_acceleration;

  /* Print out the values */
  Serial.print("Acceleration X: ");
  Serial.print(x_acceleration);
  Serial.print(", Y: ");
  Serial.print(y_acceleration);
  Serial.print(", Z: ");
  Serial.print(z_acceleration);
  Serial.println(" m/s^2");

  Serial.print("Rotation X: ");
  Serial.print(x_gyro);
  Serial.print(", Y: ");

  Serial.print(y_gyro);
  Serial.print(", Z: ");
  Serial.print(z_gyro);
  Serial.println(" rad/s");

  Serial.print("Temperature: ");
  Serial.print(tempe);
  Serial.println(" degC");

  Serial.println("");
  sendDataToSheet();
  delay(1000);

  server.handleClient();
}
void handle_OnConnect() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
 
   x_acceleration = a.acceleration.x;
  y_acceleration = a.acceleration.y;
  z_acceleration = a.acceleration.z;
  x_gyro = g.gyro.x;
  y_gyro = g.gyro.y;
  z_gyro = g.gyro.z;
  tempe = temp.temperature;
  server.send(200, "text/html", SendHTML(x_acceleration,y_acceleration,z_acceleration)); 
}
String SendHTML(float x_acceleration,float y_acceleration,float z_acceleration ){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>ESP32 X Y Z Acceleration</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  ptr +="p {font-size: 24px;color: #444444;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<div id=\"webpage\">\n";
  ptr +="<h1>ESP32 X Y Z Acceleration</h1>\n";
  
  ptr +="<p>X ACCELERATION: ";
  ptr +=(float)x_acceleration;
  
  ptr +="<p>Y ACCELERATION: ";
  ptr +=(float)y_acceleration;
  
   ptr +="<p>Z ACCELERATION: ";
  ptr +=(float)z_acceleration;
  
   ptr +="<p>X POSITION: ";
  ptr +=(float)x_gyro;
  
  ptr +="<p>Y POSITION: ";
  ptr +=(float)y_gyro;
  
   ptr +="<p>Z POSITION: ";
  ptr +=(float)z_gyro;

   ptr +="<p>TEMPERATURE: ";
  ptr +=(float)tempe;
  ptr +="</div>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}