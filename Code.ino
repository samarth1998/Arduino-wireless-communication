#include <SoftwareSerial.h>

SoftwareSerial BTSerial(10, 11); // RX | TX

void setup()
{
 pinMode(9, OUTPUT);  // this pin will pull the HC-05 pin 34 (key pin) HIGH to switch module to AT mode
 digitalWrite(9, HIGH);
 pinMode(3, OUTPUT);
 digitalWrite(3, HIGH);
 Serial.begin(9600);
 Serial.println("Enter AT commands:");
 BTSerial.begin(38400);  // HC-05 default speed in AT command more
}

void loop()
{
 String cardInfo = "";
 byte input = 0;
 int access = 0;
 String pass = "1";
 String fail = "0";

 // Keep reading from HC-05 and send to Arduino Serial Monitor
 while (BTSerial.available()){
   input = BTSerial.read();
   cardInfo.concat(String (input, HEX));
 }
 delay(100);
 
 if (Serial.available() && input == 0){
   BTSerial.write(Serial.read());
   delay(100);
 }
 else if (input != 0){
   Serial.println(cardInfo.substring(0));
 }
 
 //The UID of the card/cards that have access
 if (cardInfo.substring(0) == "98146810") || cardInfo.substring(0) == "9929485"){ 
     Serial.println(cardInfo);
     Serial.println("Authorized access");
     Serial.println();
     access = 1;
     digitalWrite(3, LOW);
     BTSerial.write(access);
     delay(1000);
     digitalWrite(3, HIGH);
 }

 //If unauthorized cards given
 else if (input != 0) {
   Serial.println(" Access denied");
   access = 0;
   digitalWrite(3, HIGH);
   BTSerial.write(access);
   delay(1000);
 }
 delay(100);
}
/*
* AUTHOR: Zhimin Lin
* Original author: Hazim Bitar (techbitar)
* Description: codes for the reader of the wireless RFID lock
* DATE: June 1
* Link: techbitar.com
*/

#include <SoftwareSerial.h>

#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
SoftwareSerial BTSerial(5, 6); // RX | TX

void setup()
{
 pinMode(4, OUTPUT);  // pull the HC-05 pin 34 (key) HIGH to switch to AT mode
 digitalWrite(4, HIGH);
 Serial.begin(9600);
 Serial.println("Enter AT commands:");
 BTSerial.begin(38400);  // HC-05 default speed in AT command more
 SPI.begin();      // Initiate  SPI bus
 mfrc522.PCD_Init();   // Initiate MFRC522
}

void loop()
{
 String access;
 String content= "";
 String ret = "";
 char input;
 while (BTSerial.available()){
   input = BTSerial.read();
   Serial.print(input);
   ret.concat (String (input, HEX));
 }
 while (Serial.available()){
   BTSerial.write(Serial.read());
 }
 delay(1000);
 access = ret;
 if (access != ""){
   if (access == "1"){
     digitalWrite(3, HIGH);
     delay(1500);
     digitalWrite(3, LOW);
   }
   else{
     for (int i = 0; i < 3; i++){
       digitalWrite(3, HIGH);
       delay(200);
       digitalWrite(3, LOW);
     }
   }
 }
 
 if ( ! mfrc522.PICC_IsNewCardPresent()){
   return;
 }
 // Select one of the cards
 if ( ! mfrc522.PICC_ReadCardSerial()){
   return;
 }
 byte letter;
 for (byte i = 0; i < mfrc522.uid.size; i++){
   Serial.print(mfrc522.uid.uidByte[i], HEX);
   BTSerial.write(mfrc522.uid.uidByte[i]);
   content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
   content.concat(String(mfrc522.uid.uidByte[i], HEX));
 }
 Serial.println();
 
   //BTSerial.write(content);
   //delay(2000);
}

