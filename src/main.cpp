#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <LiquidCrystal_I2C.h>

RF24 radio(9, 8); // CE, CSN

// Config for LCD
LiquidCrystal_I2C lcd(0x27,16,2);

#define LED 3
#define CONST_AIR 10

#define water_tank A0
#define IN1 7
#define IN2 6

const byte address[6] = "00001";

int water_val = 0;
int msg_val = 0;

void setup() {
  pinMode(LED,OUTPUT);
  pinMode(water_tank, INPUT);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  // Initialize LCD
  lcd.init();                      
  lcd.backlight();
}

void loop() {
  // if((water_val - analogRead(water_tank)) != 0) lcd.clear();

  water_val = analogRead(water_tank);
  lcd.setCursor(0,0);
  lcd.print("tangki: ");
  lcd.setCursor(8, 0);
  lcd.print("        ");
  lcd.setCursor(8, 0);
  lcd.print(water_val);
  lcd.setCursor(13,0);
  lcd.print("cm");

  if(water_val < 100) digitalWrite(IN1,HIGH);
  else digitalWrite(IN1,LOW);

  if (radio.available()) {

    digitalWrite(LED,HIGH);
    char text[32] = "";
    radio.read(&text, sizeof(text));
    lcd.setCursor(0,1);
    lcd.print("sawah :");
    lcd.setCursor(8,1);
    
    if((msg_val - atoi(text)) != 0) lcd.clear();

    msg_val =  atoi(text);
    lcd.print(msg_val);
    lcd.setCursor(13,1);
    lcd.print("cm");

    if((msg_val < CONST_AIR) && (msg_val != 0)) digitalWrite(IN2,HIGH);
    else if((msg_val >= CONST_AIR) && (msg_val != 0)) digitalWrite(IN2,LOW);

  }
  else{
    lcd.setCursor(0,1);
    lcd.print("sawah :");
    lcd.setCursor(8,1);
    lcd.print(msg_val);
    lcd.setCursor(13,1);
    lcd.print("cm");
    lcd.setCursor(15,1);
    lcd.print(".");
  }
}