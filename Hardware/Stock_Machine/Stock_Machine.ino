//https://www.makerguides.com/character-i2c-lcd-arduino-tutorial/
//https://how2electronics.com/iot-weighing-scale-hx711-load-cell-esp8266/
//https://www.hackster.io/diy-hacking/arduino-keyless-door-lock-system-with-keypad-and-lcd-bcad2e
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <Keypad_I2C.h>
#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "HX711.h"

////////////////////////  Wi-Fi ///////////////////////////
const char* ssid     = "";
const char* password = "";
///////////////////////////////////////////////////////////


////////////////////// Ultrasonic /////////////////////////
const int pingPin = D3;
int inPin = D4;
long duration, cm;
int Ultrasonic_Scale = 5.0; // 1 Wire pack / 5cm
char Input_Wire[10];
//int Init_Wire; // Init Wire at Web Portal
int Last_height; 
int tmp_last_height;
///////////////////////////////////////////////////////////


///////////////// HX711 circuit wiring ////////////////////
#define LOADCELL_DOUT_PIN  D5
#define LOADCELL_SCK_PIN  D6
 
HX711 scale;
///////////////////////////////////////////////////////////



///////////////////////// Keypad //////////////////////////
#define I2CADDR 0x20  
const byte ROWS = 4; 
const byte COLS = 4;  

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {0, 1, 2, 3}; 
byte colPins[COLS] = {4, 5, 6, 7}; 

Keypad_I2C keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS, I2CADDR, PCF8574 );

///////////////////////////////////////////////////////////

/////// LCD ///////
LiquidCrystal_I2C lcd(0x27, 16, 2);
///////////////////

//////////////////// Global variables /////////////////////
/* 
-Type of stock (0-3)
-Mode(1,2)
-In-Out
-Quantity
*/
uint8_t Mode_type; 
uint8_t Stock_type;

#define Stock_In 0 // state
#define Stock_Out 1 // state  
uint8_t InOut_Mode; 
float Stock_Quantity;

bool Ready_to_send = false;
///////////////////////////////////////////////////////////

int Inuse_Scale;
int Scale_TYPE1 = 213333;
int Scale_TYPE2 = 213333;
int Scale_TYPE3 = 213333;

//State
#define PASSWORD 0// state enter password
#define PRIMARY_MODE 1// state select Ultrasonic or Load cell
#define MODE1 2 // state Ultrasonic
#define MODE2 3 // state Load cell
#define TYPE 4 // state 
#define QUANTITY 5 // state 
uint8_t State = PASSWORD;

char Password[] = "1234";
char Input_Pass[10];
int i = 0;




void WIFI_Connect()
{
  WiFi.disconnect();
  delay(1000);
  WiFi.mode(WIFI_STA);
  
  Serial.print("WiFi connecting ......"); 
  WiFi.begin(ssid, password);
  
  // Wait for connection
  for (int i = 0; i < 25; i++)
  {
    if ( WiFi.status() != WL_CONNECTED ) {
      Serial.print(".");
      delay(500);
    }
    else{
      Serial.println("\nWiFi connected");  
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
      Display_Pass();
      State = PASSWORD;
      return;
    }
  }
}
bool HTTP_Post() {

  char payload_buffer[10];
  sprintf(payload_buffer,"%.2f",Stock_Quantity);
  
  HTTPClient http; 
  
  Serial.println("Posting to 172.20.10.10 port 3000"); 
  Serial.println("payload = {\"Mode\":\""+String(Mode_type)+"\",\"Type\":\""+String(Stock_type)+"\",\"Quantity\":\""+String(payload_buffer)+"\"}"); 

  //Request for Stock-In
  if(InOut_Mode == Stock_In){
    Serial.println("Request for Stock-In");
    http.begin("http://172.20.10.10:3000/api/hardware/stockin"); 
  }
  //Request for Stock-Out
  else{
    Serial.println("Request for Stock-Out");
    http.begin("http://172.20.10.10:3000/api/hardware/stockout"); 
  }
  
  http.addHeader("Content-Type", "application/json");
  
  int httpCode = http.POST("{\"Mode\":\""+String(Mode_type)+"\",\"Type\":\""+String(Stock_type)+"\",\"Quantity\":\""+String(payload_buffer)+"\"}");
  String payload = http.getString();                                     
  
  Serial.print(F("HTTP POST Result: "));
  Serial.println(httpCode);   //Print HTTP return code
  Serial.println(payload);

  if(httpCode == 200)
  {
    // Allocate JsonBuffer
    // Use arduinojson.org/assistant to compute the capacity.
    const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(2) + 60;
    DynamicJsonDocument doc(capacity);
  
    // Parse JSON object
    DeserializationError error = deserializeJson(doc, payload);
    if (error) {
      Serial.print(F("deserializeJson() failed"));
      http.end();
      return false;
    }  
    // Decode JSON/Extract values
    Serial.println(F("Response:"));
    Serial.println(doc["status"].as<bool>());
    
    Serial.println(doc["massage"].as<char*>());
    http.end();
    return doc["status"].as<bool>();
  }
  else
  {
    Serial.println("Error in response");
    http.end();
    return false;
  }
}


void Display_InitWire(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Init Wire");
  lcd.setCursor(0, 1);
  }
void Display_Pass(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter Passcode");
  lcd.setCursor(0, 1);
  Serial.println("Enter Passcode");
  }
void Display_Primary_Mode(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Mode1 (1)");
  lcd.setCursor(0, 1);
  lcd.print("Mode2 (2)");
  Serial.println("Enter PRIMARY_MODE");
  }
void Display_Mode1(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Stock In (1)");
  lcd.setCursor(0, 1);
  lcd.print("Stock Out (2)");
  }
void Display_Mode2(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Stock In (1)");
  lcd.setCursor(0, 1);
  lcd.print("Stock Out (2)");
  }
void Display_Type(){
  lcd.setCursor(0, 1);
  lcd.print("SELECT TYPE 1-3");
  }

  
// Init Number of Wire
bool InitWire(char key){
      char lcd_buffer[20];
               
//      if(key && key != '#' && key != '*' && key != 'A' && key != 'B' && key != 'C' && key != 'D')
//      {
//        Input_Wire[i++]=key;
//        lcd.print(key);
//        Serial.print(key);       
//      }
//      else if(key == '#')
//      {
//        Serial.println(" ");
//        lcd.clear();
//        lcd.setCursor(0, 0);
//        lcd.print("Calibrating ...");
//        Serial.println("Calibrating ...");
        
//        for(int j = 0 ; j < i ; j++){
//          Init_Wire = atoi(Input_Wire); 
//        }

        for(int i = 0 ; i < 10 ; i++){
          digitalWrite(pingPin, LOW);
          delayMicroseconds(2);
          digitalWrite(pingPin, HIGH);
          delayMicroseconds(5);
          digitalWrite(pingPin, LOW);
          duration = pulseIn(inPin, HIGH);
          cm = duration/ 29 / 2;
          Last_height += cm;
          delay(100);
        }
        Last_height = Last_height/10; 

        
        lcd.clear();
        lcd.setCursor(0, 0);
        sprintf(lcd_buffer,"Init Height: %d cm.",Last_height);
        Serial.println(lcd_buffer);
        lcd.print(lcd_buffer);
//        lcd.setCursor(0, 1);
//        sprintf(lcd_buffer,"Init Wire: %d ",Init_Wire);
//        Serial.println(lcd_buffer);
//        lcd.print(lcd_buffer);

//        memset(Input_Wire, 0, sizeof(Input_Wire));
//        i = 0;

        delay(2000);
        return true;
        
//      }
//      return false;

  }
  
//Checking Password
void State_PASSWORD(char key){        
      if(key && key != '#')
      {
        Input_Pass[i++]=key;
        lcd.print("*");
        Serial.print(key);
      }
      else if(key == '#')
      {
        Serial.println(" ");
        if(strcmp(Password, Input_Pass))
        {
          lcd.clear();
          Serial.println("Incorrect Passcode");
          lcd.setCursor(0, 0);
          lcd.print("Incorrect!!!!");
          delay(2000);
          Display_Pass();
        }
        else
        {
          Serial.println("Password OK...");
          Display_Primary_Mode();
          State = PRIMARY_MODE;
        }
        memset(Input_Pass, 0, sizeof(Input_Pass));
        i = 0;
      }
  }

void State_PRIMARY_MODE(char key){        
       if(key == '1' ){
          Serial.println("Mode1");
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Mode1");
          Mode_type = 1;
          delay(1000);
          Display_Mode1();
          State = MODE1;
       }
       else if(key == '2'){
          Serial.println("Mode2");
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Mode2");
          Mode_type = 2;
          delay(1000);
          Display_Mode2();
          State = MODE2;
       }
       else if(key !=  NO_KEY){
          Serial.println("Incorrect Mode");
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Incorrect Mode");
          delay(2000);
          Display_Pass();
          State = PASSWORD;
       }
  }

void State_MODE1(char key){ 
       if(key == '1' ){
          Serial.println("Mode Stock In ");
           lcd.clear();
           lcd.setCursor(0, 0);
           lcd.print("Stock In");
           Stock_type = 0;
           InOut_Mode = Stock_In;
           Inuse_Scale = Ultrasonic_Scale;
           State = QUANTITY;
       }
       else if(key == '2'){
          Serial.println("Mode Stock Out ");
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Stock Out");
          Stock_type = 0;
          InOut_Mode = Stock_Out;
          Inuse_Scale = Ultrasonic_Scale;
          State = QUANTITY;
       }
       else if(key !=  NO_KEY){
          Serial.println("Incorrect Mode");
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Incorrect Mode");
          delay(2000);
          Display_Pass();
          State = PASSWORD;
       }
  
  }
    
void State_MODE2(char key){ 
       if(key == '1' ){
          Serial.println("Mode Stock In ");
           lcd.clear();
           lcd.setCursor(0, 0);
           lcd.print("Stock In");
           InOut_Mode = Stock_In;
           Display_Type();
           State = TYPE;
       }
       else if(key == '2'){
          Serial.println("Mode Stock Out ");
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Stock Out");
          InOut_Mode = Stock_Out;
          Display_Type();
          State = TYPE;
       }
       else if(key !=  NO_KEY){
          Serial.println("Incorrect Mode");
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Incorrect Mode");
          delay(2000);
          Display_Pass();
          State = PASSWORD;
       }
  
  }
void State_TYPE(char key){ 
       if(key == '1' ){
           Serial.println("TYPE 1 ");
           lcd.clear();
           lcd.setCursor(0, 0);
           lcd.print("TYPE 1 ");
           lcd.setCursor(0, 1);
           Stock_type = 1;
           Inuse_Scale = Scale_TYPE1;
           State = QUANTITY;
       }
       else if(key == '2'){
          Serial.println("TYPE 2 ");
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("TYPE 2 ");
          lcd.setCursor(0, 1);
          Stock_type = 2;
          Inuse_Scale = Scale_TYPE2;
          State = QUANTITY;
       }
       else if(key == '3'){
          Serial.println("TYPE 3 ");
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("TYPE 3 ");
          lcd.setCursor(0, 1);
          Stock_type = 3;
          Inuse_Scale = Scale_TYPE3;
          State = QUANTITY;
       }
       else if(key !=  NO_KEY){
          Serial.println("Incorrect Type");
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Incorrect Type");
          delay(2000);
          Display_Pass();
          State = PASSWORD;
       }
  
  }
void State_QUANTITY(char key){
    char lcd_buffer[20];
    if(key == '*'){
      if(Mode_type == 2){
        for(int retry = 0 ; retry < 10 ; retry++){
          if (scale.is_ready()) {
              lcd.setCursor(0, 1);          
              long weight = scale.get_value(5);
              Stock_Quantity = (float)weight/Inuse_Scale;
              sprintf(lcd_buffer,"Quantity:%.2fkg.",(float)weight/Inuse_Scale);
              lcd.print(lcd_buffer);
              Serial.println(lcd_buffer);
              Ready_to_send = true;
              return;
            } 
          else {
              delay(1000);
            }
        }
        Serial.println("Loadcell Error");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Loadcell Error");
        delay(2000);
        Display_Pass();
        State = PASSWORD;
      }
      
      else{
        int Avg_cm = 0;
        int Quantity;
        for(int i = 0 ; i < 10 ; i++){
          digitalWrite(pingPin, LOW);
          delayMicroseconds(2);
          digitalWrite(pingPin, HIGH);
          delayMicroseconds(5);
          digitalWrite(pingPin, LOW);
          duration = pulseIn(inPin, HIGH);
          cm = duration/ 29 / 2;
          Avg_cm += cm;
          delay(100);
        }
        Avg_cm = Avg_cm/10;
        tmp_last_height = Avg_cm;
        Quantity = round((abs(Last_height-Avg_cm)/(float)Inuse_Scale));
        Stock_Quantity = Quantity;
        
        Serial.print("Last_height : ");
        Serial.print(Last_height);
        Serial.print(" , Avg_cm : ");
        Serial.print(Avg_cm);
        Serial.print(" , Quantity : ");
        Serial.print(Quantity);
        Serial.println("");
        
        lcd.setCursor(0, 1);          
        sprintf(lcd_buffer,"Quantity: %d ",Quantity);
        Serial.println(lcd_buffer);
        lcd.print(lcd_buffer);
        Ready_to_send = true;
      }
      
    }
    if(key == '#' && Ready_to_send){
      Serial.println("Sending Data ");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Sending Data...");
      
      if(HTTP_Post()){
        lcd.setCursor(0, 1);
        lcd.print("Success!!");
        if(Mode_type == 1){
          Last_height = tmp_last_height;
          }
        delay(2000);
      }
      else{
        lcd.setCursor(0, 1);
        lcd.print("Fail!!");
        delay(2000);
      }

      Ready_to_send = false;
      Display_Pass();
      State = PASSWORD;
    }
  }  

void setup(){
  Serial.begin(115200);
  Serial.println("\nSETUP");
  
  // Ultrasonic
  pinMode(pingPin, OUTPUT);
  pinMode(inPin, INPUT);

  memset(Input_Pass, 0, sizeof(Input_Pass));
  
  Wire.begin();  
  keypad.begin( makeKeymap(keys) ); 
  
  lcd.begin();
  lcd.backlight();
  
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Wi-Fi Connecting");
  WiFi.mode(WIFI_STA);
  Serial.println("WiFi connecting ......"); 
  if (WiFi.begin(ssid, password)) {
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
        }
  }
  Serial.println("\nWiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());


  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Calibrating ...");
  Serial.println("Load-Cell Calibrating ...");
  while(true){
    if (scale.is_ready()) {
      scale.tare(50);
      break;   
      } 
    else {
      delay(1000);
      }
  }
  Display_InitWire();
  Serial.println("Init Wire");
  while(true){
    char key = keypad.getKey(); 
    if(InitWire(key)){
      break;
      }
    delay(100);  
  }
  
  Display_Pass();  

}
void loop(){
  char key = keypad.getKey();
  
  if (WiFi.status() != WL_CONNECTED)
  { 
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Wi-Fi Connecting");
    Serial.println("\nLoss connection!!!");
    WIFI_Connect();
  }
  else{   
    switch (State) {
     case PASSWORD:
         State_PASSWORD(key);
         break;
  
     case PRIMARY_MODE:
         State_PRIMARY_MODE(key);
         break;
         
     case MODE1:
         State_MODE1(key);
         break;
             
     case MODE2:
         State_MODE2(key);
         break;
  
     case TYPE:
         State_TYPE(key);
         break;
  
     case QUANTITY:
         State_QUANTITY(key);
         break;
  
    }
  }

}
