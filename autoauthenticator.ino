#include <WiFi.h>
#include <Wire.h>
#include <HTTPClient.h>
#define LED_BUILTIN 2
#define AP_SSID "samurai"
#define AP_PASSWORD "" // leaves empty if open wifi

TaskHandle_t Task1;
TaskHandle_t Task2;

void setup() {
  
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);

 // Connecting to the WIFI
  WiFi.begin(AP_SSID, AP_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN,LOW);
  }
  digitalWrite(LED_BUILTIN, HIGH);
  delay(300);
  digitalWrite(LED_BUILTIN,LOW);
  Serial.println("Connected to the WiFi network");

  xTaskCreatePinnedToCore(
                    Task1code,   // Task function. 
                    "Task1",     // name of task. 
                    10000,       // Stack size of task 
                    NULL,        // parameter of the task 
                    1,           // priority of the task 
                    &Task1,      // Task handle to keep track of created task 
                    0);          // pin task to core 0 


  xTaskCreatePinnedToCore(
                    Task2code,   // Task function.
                    "Task2",     // name of task.
                    10000,       // Stack size of task
                    NULL,        // parameter of the task
                    1,           // priority of the task
                    &Task2,      // Task handle to keep track of created task
                    1);          // pin task to core 1
   
}

void Task1code(void * pvParameters) //This code runs on 1st core(CORE0) of the esp32
{
  
  // Login to Captive Portal
  loginportal:
  
  HTTPClient login;
  login.begin("Captive portal URL"); //put your own url
  login.addHeader("Accept", "*/*");
  login.addHeader("Accept-Language", "en-US,en;q=0.9");
  login.addHeader("Connection", "keep-alive");
  login.addHeader("Content-Type", "application/x-www-form-urlencoded");
  login.addHeader("Origin", "<HEADER URL>");
  login.addHeader("Referer", "<HEADER URL>");
  login.addHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/119.0.0.0 Safari/537.36 Edg/119.0.0.0");
  String body = "mode=''&username=<USERNAME>&password=<YOUR PASS>";
  int res = login.POST(body);
  Serial.println(res);
  
  if (res == 200) {
    Serial.println("Authentication successful");
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN,LOW);
    //This additional code is only nessesary to keep the connection alive. If it is not required then comment the code below until "void Task2code" function
    while (true){
           delay(3000);
           HTTPClient livemode;
           livemode.begin("<URL TO INVOKE THE CAPTIVE PORTAL SERVER"); //put your own url
           login.addHeader("Accept", "*/*");
           login.addHeader("Accept-Language", "en-US,en;q=0.9");
           login.addHeader("Connection", "keep-alive");
           login.addHeader("Content-Type", "application/x-www-form-urlencoded");
           login.addHeader("Referer", "<HEADER URL>");
           login.addHeader("User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/119.0.0.0 Safari/537.36 Edg/119.0.0.0");
           int gt = livemode.GET();
           if(gt!= 200){
            break;
           }
           Serial.println(gt);
           Serial.println("Connection link alive");
           digitalWrite(LED_BUILTIN, HIGH);
           delay(100);
           digitalWrite(LED_BUILTIN, LOW);
           }
           goto loginportal;
  } else {
    Serial.println("Authentication not successful");
    delay(1000);
    goto loginportal;
  }
  
}

void Task2code(void * pvParameters) //This code runs on 2nd core(CORE1) of esp32 
{
for(;;) {
  HTTPClient http;
  http.begin("https://www.google.com");
  int resp = http.GET();
  if( resp = 200) {
    Serial.println(resp); // status code
    Serial.println(http.getString()); // API response
  }
   else {
    Serial.println("Request Failed");
  }
  delay(2000);
  }}


void loop() {
  // put your main code here, to run repeatedly:

}
