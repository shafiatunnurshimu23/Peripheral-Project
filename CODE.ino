#include<Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

int x = 0, y = 0;
int input = A0;
int inputt = A2;
int input2 = A1;
int state = 0, state2 = 0;

Servo myservo, myservo2;
int i;

unsigned long currentTime, presentTimeRed = 0, presentTimeYellow = 1000000, presentTimeGreen = 1000000, presentTimeYellow2 = 1000000;
int temp1 = 1, temp2 = 0, yOff = 1, yOn = 0, redTmp = 1, greenTmp = 0, yellowTmp = 0, yellowTmp2 = 0;
int red = 9;
int yellow = 8;
int green = 7;

void setup(){

  //  lcd.init();
  // Print a message to the LCD.
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Pedestrian Count");
  lcd.setCursor(0, 1);
  lcd.print(x);
  lcd.print(" = Waiting");

  myservo.attach(5);
  myservo2.attach(3);
  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT);
  
}

void loop(){
  padestrainsIn();
  padestrainsOut();
  if(x==10) {
    // padestrainsPassing();
    redTmp = 1;
    greenTmp = 0;
    yellowTmp2 = 0;
  }

  if(redTmp==1) {
    if(x>0) {
      padestrainsPassing();
    }
    else {
      presentTimeGreen = millis();
      // digitalWrite(red, LOW);
      myservo.write(0);
      myservo2.write(90);
      redTmp = 0;
      greenTmp = 1;
    }
  }

  if(temp2 == 1) {
    for(i=0; i<=95; i+=5) {
      digitalWrite(yellow, HIGH);
      myservo2.write(i);
      delay(100);
      digitalWrite(yellow, LOW);
      delay(100);
      i+=5;
    }
    myservo.write(0);
    digitalWrite(red, LOW);
    temp2 = 0;
  }

  if(greenTmp==1) {
    if(millis() - presentTimeGreen <= 10000) {
      digitalWrite(green, HIGH);
    }
    else {
      if(x>0) {
        // presentTimeRed = millis();
        presentTimeYellow2 = millis();
        digitalWrite(green, LOW);
        // temp1 = 1;
        greenTmp = 0;
        // redTmp = 1;
        yellowTmp2 = 1;
      }
      else {
        // presentTimeRed = millis();
        presentTimeGreen = millis();
        digitalWrite(green, HIGH);
        // temp1 = 1;
        greenTmp = 1;
        // redTmp = 1;  
        yellowTmp2 = 0;
      }
      
    }
  }

  if(yellowTmp2==1) {
    if(millis() - presentTimeYellow2 <= 3000) {
      digitalWrite(yellow, HIGH);
    }
    else if(millis() - presentTimeYellow2 > 3000 && millis() - presentTimeYellow2 <= 3500){
      digitalWrite(yellow, LOW);
    }
    else if(millis() - presentTimeYellow2 > 3500 && millis() - presentTimeYellow2 <= 4500){
      digitalWrite(yellow, HIGH);
    }
    else if(millis() - presentTimeYellow2 > 4500 && millis() - presentTimeYellow2 <= 5000){
      digitalWrite(yellow, LOW);
    }
    else if(millis() - presentTimeYellow2 > 5000 && millis() - presentTimeYellow2 <= 6000){
      digitalWrite(yellow, HIGH);
    }
    // else if(millis() - presentTimeYellow2 > 6000 && millis() - presentTimeYellow2 <= 6500){
    //   digitalWrite(yellow, LOW);
    // }
    else {
      presentTimeRed = millis();
      // presentTimeYellowBlink1 = millis();
      // digitalWrite(yellow, HIGH);
      digitalWrite(green, LOW);
      temp1 = 1;
      redTmp = 1;
      yellowTmp2 = 0;
    }
  }
}


void padestrainsIn() {
  int counter = digitalRead(A0); 
  if (state == 0)
  {
    switch (counter) {
      case 1 : 
        state = 1;
        lcd.setCursor (0, 1); 
        x = x + 1; 
        lcd.print(x); 
        Serial.println(x);
        break;
      case 0 : 
        state = 0; 
        break;

    }
  }

  if (counter == LOW) {
    state = 0;
  }
}

void padestrainsOut() {
  int counter2 = digitalRead(A1);
  if (state2 == 0)
  {
    switch (counter2) {
      case 1 : 
        state2 = 1;
        lcd.setCursor (0, 1); 
        x = x - 1; 
        if(x<0) {
          x = 0;
        }
        if(x==9) {
          lcd.init();
          // lcd.backlight();
          lcd.setCursor(0, 0);
          lcd.print("Crossing Road");
          lcd.setCursor(0, 1);
          lcd.print(x);
          lcd.setCursor(1, 1);
          lcd.print(" Lefts              ");
        }
        lcd.print(x); 
        Serial.println(x);
        break;
      case 0 : 
        state2 = 0; 
        break;
    }
  }
  if (counter2 == LOW) {
    state2 = 0;
  }
}

void padestrainsPassing() {
  lcd.init();
  // lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("  Start Crossing");
  lcd.setCursor(0, 1);
  lcd.print(x);
  lcd.setCursor(2, 1);
  lcd.print(" Lefts              ");

  digitalWrite(green, LOW);
  digitalWrite(yellow, HIGH);
  delay(1000);
  digitalWrite(yellow, LOW);
  delay(500);
  digitalWrite(yellow, HIGH);
  delay(1000);
  digitalWrite(yellow, LOW);
  delay(500);
  digitalWrite(yellow, HIGH);
  delay(1000);
  digitalWrite(yellow, LOW);
  
  // delay(500);
  // digitalWrite(yellow, LOW);

  digitalWrite(green, LOW);
  digitalWrite(red, HIGH);
  
  for(i=0; i<=90; i+=5) {
    digitalWrite(yellow, HIGH);
    myservo.write(i);
    delay(100);
    digitalWrite(yellow, LOW);
    delay(100);
    i+=5;
  }
  myservo.write(93);
  myservo2.write(0);
  // myservo.write(83);
  while(1) {
    padestrainsIn();
    padestrainsOut();
    if(x<=0) {
      digitalWrite(red, LOW);
      x = 0;
      lcd.init();
      lcd.backlight();
      lcd.setCursor(0, 0);
      lcd.print("Pedestrian Count");
      lcd.setCursor(0, 1);
      lcd.print(x);
      lcd.print(" = Waiting");
      temp1 = 1, temp2 = 1, redTmp = 1, greenTmp = 0, yellowTmp = 0, yellowTmp2 = 0;
      presentTimeGreen = millis();
      break;
    }
  }
}
