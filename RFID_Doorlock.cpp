// By: Teddy Sannan
// Code RFID Doorlock
// Used to lock and unlock door using keycard

#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

 
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

Servo servo_6; // Define servo name
int pos = 100; // Postion for servo
 
void setup() 
{
  servo_6.attach(6, 544, 2400); // Call servo on pin 6
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  Serial.println("Approximate your card to the reader...");
  Serial.println();

}
void loop() 
{
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();

  if (content.substring(1) == "C5 49 50 2D") //UID for card access
  {
    if (pos == 100) { // Door is lokced and needs to be unlocked
      Serial.println("Authorized access");
      Serial.println();
      delay(1000);

      pos = pos - 100; // Change pos for servo to unlock door
      servo_6.write(pos); // Rotate servo to pos
    }
    
    else if (pos == 0) { // Door is unlokced and needs to be locked
      Serial.println("Authorized access");
      Serial.println();
      delay(1000);
    
      pos = pos + 100; // Change pos for servo to lock door
      servo_6.write(pos); // Rotate servo to pos
    }
  }

 else { // Wrong UID card
    Serial.println(" Access denied");
    delay(1000);
  }
}