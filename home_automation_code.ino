//L = unlock the home
//l = lock the home
//F = fan on
//f = fan off
//B = bulb1 on
//b = bulb1 off
//T = bulb2 on
//t = bulb2 off
//******************************************************************
#include "dht.h"
#define dht_apin A1 // Analog Pin sensor is connected to
dht DHT;
#include <Adafruit_Fingerprint.h>
#include<Keypad.h>
#include <SPI.h>                           //RF
#include <MFRC522.h>                      //RF
#define SS_PIN 53                        //RF
#define RST_PIN 49                      // RF
//********************************************************************88
//////////////////////keypad

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 4, 3, 2}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

//*************************************************************************************
int gate=1;
String p ="";
////////////////////////////keypad

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
int pass_length = 0;
///////////////////////////
int  wifi_led1p = 0;
int wifi_lockp = 0;
int wifi_fanp  = 0;
int wifi_led2p = 0;
//////////////////////////
int  rfled1p = 0;
int rfgatep = 0;
int rffanp  = 0;
int rfled2p = 0;
//////////////////////////
int val;
float cel;
int tempPin = A1;  // temp 
int Pir = 21;  // temp 
int PIR = 0;
int temp2 = 0;
//////////////////////pin
int sensor1, sensor2, personNo = 0;
int s1 = 48, s2 = 46;
int gatelight = 20, roomlight1 = 19;
int fan = 10, led2 = 11, led1 = 12;
int L = 0, l =0 , f = 0; 
int gate_pin1 =22,gate_pin2 =23;
// state of led1,led2,fan  ******
////////////////////////pin
const int rs = 45, en = 44, d4 = 43, d5 = 42, d6 = 41, d7 = 40;                         //for lcd
#include <LiquidCrystal.h>
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int Time = 0 , Time1 = 0,Time4 = 0,Time_gate = 0;                                                                    // Time for lcd, Time1 for RFID
int locked = 0;
int g = 0, currenttime, attempt = 0;                                                                     //state for gate **
char password[4] = {'1', '2', '3', '4'};
int temp = 0;
float h,t;
int delayTime = 1000;

////////////////////////////
SoftwareSerial mySerial(24, 25);  /////////////////SoftwareSerial 23 24 
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

String textMessage;
void setup()
{
    /////////////////////////fi
  finger.begin(57600);        //
  finger.getTemplateCount();  //finger
  
  if (finger.verifyPassword()) 
  {
    //  Serial.println("Found fingerprint sensor!");
  } else
  {
    //  Serial.println("Did not find fingerprint sensor :(");
    //while (1) { delay(1);}
    
  }

  ///////////////fi
  SPI.begin();      // Initiate  SPI bus     RF
  mfrc522.PCD_Init();   // Initiate MFRC522  RF
  //myservo.attach(23);
  
  Serial.begin(9600);
  Serial1.begin(9600);//bluetooth
  Serial2.begin(9600);// wifi

  //gsm
   Serial.print("AT+CMGF=1\r");
  //gsm
  
  
  pinMode(25, INPUT);//rf remote
  pinMode(27, INPUT);//
  pinMode(29, INPUT);//
  
  pinMode(14, INPUT);//wifi
  pinMode(15, INPUT);//
  pinMode(16, INPUT);//
  pinMode(17, INPUT);//
  pinMode(s1, INPUT);
  pinMode(s2, INPUT);
  pinMode(gatelight, OUTPUT);
  pinMode(roomlight1, OUTPUT);
  pinMode(fan, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led1, OUTPUT);
  digitalWrite(fan,LOW);
  digitalWrite(gatelight,LOW);
  Serial.println("Bluetooth gates are open.");
 

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.cursor();
  gate = 0;
  //closeDoor();
}

//*********************************************************************************

void loop()
{

  if (temp == 0)
  {
    temp = 1;
    Serial.println("Enter the Password or use our RFID card ");
    Serial1.println("Enter the Password or use our RFID card ");
  }
  if (locked == 0)
  {
    keylocked();
    lockedHome();
     //getFingerprintIDez();
  }
  else
  {
    USART();
    bluetooth();
    key();
    gsm();
    wifi_firebase();
    
  }
  thief();
  if (locked == 1)
  {
    Gate();
  }
  ledfan();
  RF();
  lcdNormal();
  //gsm();
  //controlDoor();
  //key();
  //    Serial.println(current);
  //  wifi();
  rfremote();
  Dht();
  //wifi_firebase();
  wifi_write_to_firebase();

  }
////////////////////////////////gsm
/*void SIM900power()
{
  pinMode(9, OUTPUT); 
  digitalWrite(9,LOW);
  delay(1000);
  digitalWrite(9,HIGH);
  delay(2000);
  digitalWrite(9,LOW);
  delay(3000);
}
*/

void gsm(){
  if(Serial.available()>0){
    textMessage = Serial.readString();
   textMessage.toUpperCase();   
    delay(10);
  } 
  if(textMessage.indexOf("LAMPON")>=0)
  {
    L = 1;// Turn on lamp on /*---------------------Yaha daal do chacha---------------------*/
  
    textMessage = "";   
  }
  else if(textMessage.indexOf("LAMPOFF")>=0)
  {
     /*------------------------------yaha daal do chacha--------------------*/
    L = 0;
    textMessage = ""; 
  }
  else if(textMessage.indexOf("LIGHTON")>=0)
  {
    l = 1;// Turn on LIGHT /*--------------------------------yaha daal do chacha-------------------*/        
   
    textMessage = "";   
  }
   else if(textMessage.indexOf("LIGHTOFF")>=0)
   {
     l = 0;// Turn off LIGHT /*-------------------------------------yaha daal do chacha-------------------*/
    
    textMessage = ""; 
  }
  else if(textMessage.indexOf("FANON")>=0)
  {
    f = 1;// Turn on fan /*--------------------------------------yaha daal do chacha------------------------------*/
    
    textMessage = "";   
  }
  else if(textMessage.indexOf("FANOFF")>=0)
  {
    f =0;// Turn off fan /*--------------------------------------yaha daaal do chacha ------------------*/
    
    textMessage = ""; 
  }
   else if(textMessage.indexOf("LOCK")>=0)
   {
    locked = 0;
    // LOCKS HOME /*-------------------------------------------yaha daal do chacha-----------------*/
    
    textMessage = "";   
  }
  else if(textMessage.indexOf("ALLOFF")>=0)
  {
    // Turn off all fan, light, lamp /*------------yaha daal do chacha--------------------------*/
    l =0;
    L= 0;
    f =0;
      
    textMessage = ""; 
  }
}



////////////////////////////////gsm
//////////////////////////////////////////fi
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  // found a match!*--------------------------------------------likh do chacha yaha----------*/
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID; 
}

/////////////////////////////////////////////////////////////fi  



uint8_t getFingerprintID() 
{
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
    locked = 1;                      //////////////////////////
    lcd.print("home is unlock");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR)
  {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  // found a match!/*-----------------Check ----Yaha ---daal do*/
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence); 

  return finger.fingerID;
}


///////////////////////////////////////////////fi






//**************************************************************************************************
void controlDoor()
{
  if(gate == 0)
  {
    closeDoor();
  }
  else 
  {
    openDoor();
  }
}
//***************************************************************************************************
void closeDoor()
{
   if(currenttime < Time_gate + 2000)
  {
    digitalWrite(gate_pin1,HIGH);
    digitalWrite(gate_pin2,LOW);
   
  }
  else
  {
    digitalWrite(gate_pin1,LOW);
    digitalWrite(gate_pin2,LOW);
    //digitalWrite(gatelight,HIGH);
  }
}

void openDoor()
{
  
   if(currenttime < Time_gate + 2000)
  {
    digitalWrite(gate_pin1,LOW);
    digitalWrite(gate_pin2,HIGH);
     //digitalWrite(gatelight,HIGH);
  }
  else
  {
    digitalWrite(gate_pin1,LOW);
    digitalWrite(gate_pin2,LOW);
     //digitalWrite(gatelight,LOW);
  }
}

void Gate()
{
  person_in();
  person_out();
}

//*************************************************************************************************************

void bluetooth()
{

  if (Serial1.available())
  {
    char a = Serial1.read();                                //l == led2,L == led1, f == fan;***************


    if (a == 'l')
    {
      locked = 0;
      temp = 1;
      Serial.println("Home is Locked");
      Serial1.println('l');
    }
   /* else if (a == 'L')
    {
      locked = 1;
      temp = 1;
      Serial.println("Home is unLocked");
      Serial1.println('L');
    }*/
    else if (a == 'F')
    {

      f = 1;
      Serial1.println("Fan is on");
      Serial.println('F');
      lcd.clear();  lcd.print(" fan is on ");       Time = millis();
    }


    else if (a == 'T')
    {
      l = 1;
      Serial.println("Led2 is on");
      Serial1.println('T');
      lcd.clear();  lcd.print(" Led2 is on ");        Time = millis();
    }
    else if (a == 't')
    {
      l = 0;
      Serial.println("Led2 is off");
      Serial1.println('t');
      lcd.clear();  lcd.print(" Led2 is off ");       Time = millis();
    }

    else if (a == 'B')
    {

      L = 1;
      Serial.println("Led1 is on");
      Serial1.println('B');
      lcd.clear();  lcd.print(" Led1 is on ");        Time = millis();
    }
    else if (a == 'b')
    {
      Serial.println("Led1 is off");
      Serial1.println('b');
      lcd.clear();  lcd.print(" Led1 is off ");        Time = millis();
      L = 0;
    }
    else if (a == 'f' )
    {
      f = 0;
      Serial.println("Fan is off");
      Serial1.println("f");
      lcd.clear();  lcd.print(" fan is off ");     Time = millis();
    }
    else if (a == 'G')
    {
      digitalWrite(gatelight, 0);
      gate = 0;
    }
    else if (a == 'g')
    {
      digitalWrite(gatelight, 1);
      gate = 1;
    }
  }
}


//*******************************************************************************************************

void person_in()
{
  sensor1 = digitalRead(s1);
  sensor2 = digitalRead(s2);

  if (sensor1 == 1 )
  {
    Serial.println("gate is open in");
    Serial1.println('g');
    lcd.clear();   lcd.print("gate is open in");
    Time = millis();
    openDoor();
    digitalWrite(gatelight, 1);
    gate = 1;
    int j = 0;
    while (j < 3000)
    {
      sensor2 = digitalRead(s2);
      if (sensor2 == 1)
      {
        personNo++;
        digitalWrite(roomlight1, 1);
        Serial.print("total person in home =");
        Serial1.println('G');
        lcd.clear();  lcd.print("total person in");    lcd.setCursor(0, 1);   lcd.print("home :");   lcd.print(personNo);
        Time = millis();
        digitalWrite(gatelight, 0);
        gate = 0;
        closeDoor();
        Serial.println(personNo);

        delay(1000);
        break;
      }
      else if (j > 2998)
      {
        digitalWrite(gatelight, 0);
        gate = 0;
        closeDoor();
        Serial.println("Gate is closed");
        Serial1.println("G**");
        
        lcd.clear();   lcd.print("gate is closed");
        Time = millis();
      }
      delay(1);
      j++;
    }
  }
}

//***************************************************************************************************

void person_out()
{
  sensor1 = digitalRead(s1);
  sensor2 = digitalRead(s2);

  if (sensor2 == 1 )
  {

    Serial.println("gate is open out");
    Serial1.print('g');
    lcd.clear();   lcd.print("gate is open out");     Time = millis();
    digitalWrite(gatelight, 1);
    gate = 1;
    openDoor();
    int j = 0;
    while (j < 4000)
    {
      sensor1 = digitalRead(s1);
      if (sensor1 == 1)
      {
        personNo--;
        digitalWrite(roomlight1, 1);
        lcd.clear();  lcd.print("total person in");    lcd.setCursor(0, 1);   lcd.print("home :");   lcd.print(personNo);
        Time = millis();                                                                                                // time for print in lcd upper lines  things
        digitalWrite(gatelight, 0);
        gate = 0;
        closeDoor();
        Serial.print("total person in home");
        Serial.println(personNo);
        Serial1.print('G');
        //Serial1.println(personNo);
        delay(1000);
        break;
      }
      else if (j > 3998)
      {
        digitalWrite(gatelight, 0);
        gate = 0;
        closeDoor();
        Serial1.print('G');
        Serial.println("Gate is closed");
        lcd.clear();
        lcd.print("Gate is closed");
        Time = millis();
        
      }
      delay(1);
      j++;
    } 
  }
}

//**********************************************************************************************

void ledfan()
{
  if (l == 1)
  {
    digitalWrite(led2, 1);
    Serial1.println('T');
  }
  else
  {
    Serial1.println('t');
    digitalWrite(led2, 0);
  }
  if (f == 1)
  {
    Serial1.println('F');
    digitalWrite(fan, 1);
  }
  else
  {
    Serial1.println('f');
    digitalWrite(fan, 0);
  }
  if (L == 1)
  {
    Serial1.println('B');
    digitalWrite(led1, 1);
  }
  else
  {
    Serial1.println('b');
    digitalWrite(led1, 0);
  }
}

//****************************************************************************************************************

void thief()                            // used in loop
{
  sensor2 = digitalRead(s2);
  if (personNo == 0 && (sensor2 == 1))
  
  {    // || PIR == 1
    Serial.println("there is a thief in your home....");
    Serial1.println('Z');                                                           /////******************************************************#############
    lcd.clear();    lcd.print(" ALERT ALERT ");
    Time = millis();
    for (int i = 0; i < 8; i++)
    {
      digitalWrite(12,HIGH);
       delay(1000);
      //digitalWrite(11,HIGH);
      //digitalWrite(12,HIGH);
      delay(500);
      digitalWrite(12,LOW);
      //digitalWrite(11,LOW);
      //digitalWrite(fan, 0);
      delay(500);
       Serial.println("there is a thief in your home....");
       Serial2.println("Z");
    }
    Serial1.println('z');
    Serial2.println('z');
  }
}

//******************************************************************************************


void lcdNormal()                             // normaly print state of light and fan
{ // used in loop
  currenttime = millis();
  if(locked == 1)
  {
    if (currenttime > Time + 3000)
    {
      if((millis()%5000)<3000)
      {
        lcd.clear();   lcd.print("L1  L2 fan temp"); lcd.setCursor(0, 1);
        if (L == 1)

        {
          lcd.print("on ");
          //Serial.println("currenttime");
        }
        else
        lcd.print("off ");

        if (l == 1)
          lcd.print("  on  ");
        else
          lcd.print("off ");

        if (f == 1)
          lcd.print("on ");
        else
          lcd.print("off ");
          lcd.print(t);
      }
    /*else
    {
      lcd.clear();
      lcd.print(p);   // 
    }
    */}
  }
  else if(currenttime > Time + 3000) 
  {
    lcd.clear();
    lcd.print("password :");
    
  }
 
}
//*******************************************************************************************************
void lockedHome()                           // bluetooth lock syestem used in loop
{
  //Serial.println("Enter the password");
  char Enterpassword[4];
  int p = 0, j;
  //Serial.println("");

  if (Serial1.available())
  {
    char c = Serial1.read();
    //Serial.println(c);
    if (c == '1' || '2' || '3' || '4' || '5' || '6' || '7' || '8' || '9' || '0')
    {
      Enterpassword[pass_length] = c;
      Serial.print(c);
      pass_length++;
    }
    if (pass_length == 4)
    {
      for (int i = 0; i < 4; i++)
      {
        if (Enterpassword[i] == password[i])
        {
          p = 1;
        }
        else
        {
          p = 0;
          Serial.println("incorrect password");
          pass_length = 0;
          break;
        }
        pass_length = 0;
      }
    }
  }


  if (p == 1)
  {
    Serial.println("home is unlocked ");
    Serial.println(locked);
    Serial1.println('L');
    //Serial1.println(locked);
    locked = 1;

  }



}
//*************************************************************************************************************
void RF()                                     // used in loop
{

  if (attempt >= 3)
  {

    if (temp2 == 0)
    {
      Serial.print("Home is fully secured ");
      lcd.clear(); lcd.print("Home is fully secured ");
      Time = millis();
      Serial1.print("Home is fully secured ");
      temp2 = 1;
    }
    if (currenttime > Time1 + 10000)
      attempt = 0;
  }
  else
  {
    if (currenttime > Time1 + delayTime)
    {
      if (locked == 0)
      {
        int read_rfid;
        read_rfid = rfid_security();
        if (read_rfid == 1)
        {
          Time1 = millis();                                                                                   // for RFID
          Serial.println("Acess Granted");      Serial.println("Tiwari Sir");      Serial.println();
          Serial1.println("Acess Granted");    //  Serial1.println("Tiwari Sir");
          Serial1.println('L');
          locked = 1;
          attempt = 0;

          lcd.clear();  lcd.print("Acess Granted ");    lcd.setCursor(0, 1);   lcd.print("home is unlocked");
          Time = millis();                                                                                    // for lcd
        }
        else if ( read_rfid == 2)
        {
          Time1 = millis();                                                                                        //forRFID
          Serial.println("Acess Granted");      Serial.println("HAWK Entering");      Serial.println();
          Serial1.println("Acess Granted");     // Serial1.println("HAWK Entering");
          Serial1.println('L');
          lcd.clear();  lcd.print("Acess Granted ");    lcd.setCursor(0, 1);   lcd.print("home is unlocked");
          Time = millis();                                                                                          //for lcd
          read_rfid = 0;
          locked = 1;

          attempt = 0;
        }
        else if (read_rfid == -1)
        {
          Time1 = millis();                                                                                //for RFID
          Serial.println("Acess Denied");   //   Serial.println("Chaman Chutiya");      Serial.println();
          Serial1.println("Acess Denied");   //   Serial1.println("Chaman Chutiya");

          lcd.clear();  lcd.print("Acess Denied ");    lcd.setCursor(0, 1);  // lcd.print("Chaman Chutiya");
          Time = millis();                                                                                  //for lcd
          attempt++;
          temp2 = 0;
        }

      }


      else if (locked == 1)
      {
        int read_rfid;
        read_rfid = rfid_security();
        if (read_rfid == 1)
        {
          Serial.println("Hmoe is locked");      //Serial.println("Tiwari Sir");
          Serial.println();
          Serial1.println("Home is locked");     // Serial1.println("Tiwari Sir");
          Serial1.println('l');
          locked = 0 ;  Time1 = millis();

          lcd.clear();  lcd.print("Home is locked ");    /*lcd.setCursor(0, 1);   lcd.print("");  */ Time = millis();
        }
        else if ( read_rfid == 2)
        {
          Serial.println("Home is locked");     // Serial.println("HAWK Entering");
          Serial.println();
          Serial1.println("Home is locked");    //  Serial1.println("HAWK Entering");
          Serial1.println('l');
          lcd.clear();  lcd.print("Home is locked ");   /* lcd.setCursor(0, 1);   lcd.print("home is unlocked");*/   Time = millis();
          read_rfid = 0;
          locked = 0; Time1 = millis();
        }
        /*else if (read_rfid == -1)
          {
          Serial.println("Acess Denied");      Serial.println("Chaman Chutiya");      Serial.println();
          Serial1.println("Acess Denied");      Serial1.println("Chaman Chutiya");      Serial1.println();
          lcd.clear();  lcd.print("Acess Denied ");    lcd.setCursor(0, 1);   lcd.print("Chaman Chutiya");   Time = millis();
          }*/
      }//else if locked=1;
    }//if current
  }//else
}//RF

//**************************************************************************************************
int rfid_security()               // used in RF function
{
  int status_rfid;
  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return 3;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return 3;
  }
  String content = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    //Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    //Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  /*Serial.println();
    Serial.print("Message : ");
  */
  content.toUpperCase();
  if (content.substring(1) == "12 BB 58 10")// tiwari chacha ka rfid tag
  {
    return 1;
  }

  else if (content.substring(1) == "12 62 31 2F") //change here the UID of the card/cards that you want to give access
  {
    return 2;
  }
  else {
    return -1;
  }
  mfrc522.PICC_HaltA();
}

//********************************************************************************************************************

void USART()
{

  while (Serial.available())
  {
    String a = Serial.readStringUntil('\n');                                //l == led2,L == led1, f == fan;***************
    Serial.print(a);

    if (a == "lock the home")
    {
      locked = 0;
      Serial.println("Home is Locked");
      //lite_le =0;
      lcd.clear();    lcd.print("Home is locked");     Time = millis();
    }

    else if (a == "fan on")
    {

      f = 1;
      Serial.println("Fan is on");
      lcd.clear();  lcd.print(" fan is on ");    Time = millis();
    }
    else if (a == "fan off")
    {
      f = 0;
      Serial.println("Fan is off");
      lcd.clear();  lcd.print(" fan is off ");  Time = millis();

    }

    else if (a == "led2 on")
    {
      l = 1;
      Serial.println("Led2 is on");
      lcd.clear();  lcd.print(" Led2 is on ");        Time = millis();
    }
    else if (a == "led2 off")
    {
      l = 0;
      Serial.println("Led2 is off");
      lcd.clear();  lcd.print(" Led2 is off ");     Time = millis();
    }

    else if (a == "led1 on")
    {
      L = 1;
      Serial.println("Led1 is on");
      lcd.clear();  lcd.print(" Led1 is on ");        Time = millis();
    }
    else if (a == "led1 off" )
    {
      L = 0;
      Serial.println("Led1 is off");
      lcd.clear();  lcd.print(" Led1 is off ");     Time = millis();
    }

    else if (a == "open gate" )
    {
      digitalWrite(gatelight, 1);
      gate = 1;
      Serial.println("open gate");
      lcd.clear();  lcd.print(" Led1 is off ");     Time = millis();
    }
    else if (a == "close gate" )
    {
      digitalWrite(gatelight, 0);
      gate = 0;
      Serial.println("gate is closed");
      lcd.clear();  lcd.print(" Led1 is off ");     Time = millis();
    }

  }

}

//*************************************************************************************************

void key()
{
  char customKey = customKeypad.getKey();
  if (customKey)
  {
    char c = customKey;

    //Serial.println(customKey);
    if (customKey == 'A')
    {
      l = 1 - l;
      Serial.println(customKey);
      if (l == 1)
        Serial.println("LED1 is On");
      else
        Serial.println("LED1 is Off");
    }
    else if (customKey == 'B')
    {
      L = 1 - L;
      Serial.println(customKey);
      if (L == 1)
        Serial.println("LED2 is On");
      else
        Serial.println("LED2 is Off");
    }
    else if (customKey == 'C')
    {
      f = 1 - f;
      Serial.println(customKey);
      if (f == 1)
        Serial.println("Fan is On");
      else
        Serial.println("Fan is Off");
    }
  }
}

//***************************************************************************************************

void keylocked()
{
  char Enterpassword[4];
  int p = 0;

  char customKey = customKeypad.getKey();
  if (customKey)
  {
    char c = customKey;

    if (locked == 0)
    {
      Enterpassword[pass_length] = c;
      Serial.print("*");
      lcd.print("*");
      Time = millis();
      pass_length++;

      if (pass_length == 4)
      {
        Serial.println();
        for (int i = 0; i < 4; i++)
        {
          // Serial.print(Enterpassword[i]);
          if (Enterpassword[i] == password[i])
          {
            p = 1;
          }
          else
          {
            p = 0;
            Serial.println("incorrect password");
            lcd.clear();
            lcd.print("incorrect password");
            Time = millis();
            pass_length = 0;
            break;
          }
          pass_length = 0;
        }
        if (p == 1)
        {
          Serial.println("home is unlocked ");
          lcd.clear();  lcd.print("Acess Granted ");    lcd.setCursor(0, 1);   lcd.print("home is unlocked");
          Time = millis();
          //Serial.println(locked);
         // Serial1.println('L');
          //Serial1.println(locked);
          locked = 1;
        }
      }
    }
  }
}
//*********************************************************************************************
  
/*  void wifi()
{
  
   int wifi_led1 = digitalRead(14);
   int wifi_lock = digitalRead(15);
   int wifi_fan = digitalRead(16);
   int wifi_led2 = digitalRead(17);

  if(wifi_led1 !=wifi_led1p)
  {
    if(L == 0)
    {
      L = 1;
      Serial.println("Led1 is on");
    }
    else
    {
      L = 0;
      Serial.println("Led1 is off");
    }
    wifi_led1p = wifi_led1;
  }
  
   if(wifi_led2 != wifi_led2p)
  {
    if (l == 0)
    {
      l = 1;
      Serial.println("Led2 is on");
    }
    else
    {
      l = 0;
      Serial.println("Led2 is off");      
    }
    wifi_led2p = wifi_led2;
  }
  
   if(wifi_fan != wifi_fanp)
  {
    if(f == 0)
    {
      f= 1;
      Serial.println("Fan is on");
    }
    else
    {
      f = 0;
      Serial.println("Fan is off");
    }
    wifi_fanp = wifi_fan;
  }
  
   if(wifi_lock != wifi_lockp)
  {
    locked=0;
    Serial.println("home is locked");
    lcd.clear();  
    lcd.print("home is locked"); Time=millis();
    wifi_lockp = wifi_lock;
  }
}
*/
//********************************************************************************************

void tem()
{
val = analogRead(tempPin);
float mv = ( val/1024.0)*5000; 
cel = mv/10;
float farh = (cel*9)/5 + 32;
}

//*********************************************************************************************

void rfremote()
{
   int rfled1 = digitalRead(25);
 //  int rfgate = digitalRead();
   int rffan = digitalRead(29);
   int rfled2 = digitalRead(27);

  if(rfled1 !=rfled1p)
  {
    if(L == 0)
    {
      L = 1;
      Serial.println("Led1 is on");
    }
    else
    {
      L = 0;
      Serial.println("Led1 is off");
    }
    rfled1p = rfled1;
  }
  
   if(rfled2 != rfled2p)
  {
    if (l == 0)
    {
      l = 1;
      Serial.println("Led2 is on");
    }
    else
    {
      l = 0;
      Serial.println("Led2 is off");      
    }
    rfled2p = rfled2;
  }
  
   if(rffan != rffanp)
  {
    if(f == 0)
    {
      f= 1;
      Serial.println("Fan is on");
    }
    else
    {
      f = 0;
      Serial.println("Fan is off");
    }
    rffanp = rffan;
  }
 }

void Dht() {
  DHT.read11(dht_apin);
  currenttime = millis();
  if(currenttime < Time4 + 500)
  {//Serial.print("Current humidity = ");
     h = DHT.humidity;
     t = DHT.temperature; 
   Time4 = millis();
    PIR = digitalRead(Pir);
  } 
}
/*
void Gsm()
{
  if(SIM900.available()>0)
  {
    textMessage = SIM900.readString();
    Serial.print(textMessage);    
    delay(10);
   
    //Appliances state change
      if(textMessage.indexOf("Turn on light1")>=0)
      {
        Serial.println("Turn on light1");  
        //sendSMS("Light1 is on");
        l = 1;
        textMessage = "";   
      }
      else if(textMessage.indexOf("Turn off light1")>=0)
      {
        Serial.println("Turn off light1");
        //sendSMS("Light1 is off");
        l =0;
        textMessage = ""; 
      }
      else if(textMessage.indexOf("Turn on light2")>=0)
      { 
        Serial.println("Turn on light2");  
        //sendSMS("light2 is on");
        L = 1;
        textMessage = "";   
      }
      else if(textMessage.indexOf("Turn on fan")>=0)
      {
        Serial.println("Turn off light2");
        //sendSMS("Light2 is off");
        L = 0;
        textMessage = ""; 
      }
      else if(textMessage.indexOf("Turn on fan")>=0)
      {
        Serial.println("Turn on fan");
        //sendSMS("Fan is on");
        f = 1;
        textMessage = ""; 
      }
      else if(textMessage.indexOf("Turn off fan")>=0)
      {
        Serial.println("Turn off fan");
        //sendSMS("Fan is off");
        f = 0;
        textMessage = ""; 
      }
      if(textMessage.indexOf("Lock the home")>=0)
      {
        Serial.println("L");
        //sendSMS("Fan is on");
        locked = 0;
        textMessage = ""; 
      }
      if(textMessage.indexOf("Unlock the home")>=0)
      {
        Serial.println("U");
        //sendSMS("Home is unlocked");
        locked = 1; 
        textMessage = ""; 
      }
  } 
}  

/*void sendSMS(String message)
{
  // AT command to set SIM900 to SMS mode
  SIM900.print("AT+CMGF=1\r"); 
  delay(100);

  // UPDATE THE RECIPIENT'S MOBILE NUMBER
  // USE INTERNATIONAL FORMAT CODE FOR MOBILE NUMBERS
  SIM900.println("AT + CMGS = \"+918840516827\""); 
  delay(100);
  // Send the SMS
  SIM900.println(message); 
  delay(100);

  // End AT command with a ^Z, ASCII code 26
  SIM900.println((char)26); 
  delay(100);
  SIM900.println();
  // Give module time to send SMS
 // delay(5000); 
}
*/


void wifi_firebase()
{
  if(Serial2.available()>=0)
  {
    String a = Serial2.readStringUntil('\n');
    char s = a.charAt(0);
    
    if(s == 'd')
    {
      p = a.substring(5,19);
      
    }
    else if(s == 'T' && l == 0)
    {
      l = 1;
      Serial.println("##Led2 is on");
      Serial1.println('T');
      lcd.clear();  lcd.print(" Led2 is on ");        Time = millis();

    }
    else if(s == 't' && l == 1)
    {
      l = 0;
      Serial.println("##Led2 is off");
      Serial1.println('t');
      lcd.clear();  lcd.print(" Led2 is off ");       Time = millis();

    }
    else if(s == 'B' && L == 0)
    {
      L = 1;
      Serial.println("##Led1 is on");
      Serial1.println('B');
      lcd.clear();  lcd.print(" Led1 is on ");        Time = millis();

    }
    else if(s == 'b' && L == 1)
    {
      L = 0;
      Serial.println("##Led1 is off");
      Serial1.println('b');
      lcd.clear();  lcd.print(" Led1 is off ");        Time = millis();

    }
    else if(s == 'F' && f == 0)
    {
      f = 1;
      Serial.println("##Fan is on");
      Serial1.println('F');
      lcd.clear();  lcd.print(" fan is on ");       Time = millis();

    }
    else if(s == 'f' && f == 1)
    {
      f = 0;
      Serial.println("##fan is off");
      Serial1.println('f');
      lcd.clear();  lcd.print("# fan is off ");        Time = millis();

    }
    else if(s == 'l' && locked == 1)
    {
      locked = 0;
      temp = 1;
      Serial.println("##Home is Locked");
      Serial1.println('l');
      lcd.clear();  lcd.print("##Home is locked");        Time = millis();
    }        
    else if(s == 'L' && locked == 0)
    {
      locked = 1;
      Serial.println("##Home is Unlocked");
      Serial1.println('l');
      lcd.clear();  lcd.print("##Home is Unlocked");        Time = millis();
    }        
  }
}


void wifi_write_to_firebase()
{
  //Serial2.print('T');
  //int v = 45;
  //Serial2.println(v);
  if(locked == 1)
    Serial2.println('L');//##############################################################
  else
    Serial2.println('l');
  
  if(L == 1)
    Serial2.println('B');
  else
    Serial2.println('b');
  
  if(l == 1)
    Serial2.println('T');
  else
    Serial2.println('t');      
  
  if(f == 1)
    Serial2.println('F');
  else
    Serial2.println('f');      
  
}

