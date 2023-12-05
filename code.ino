// Stepper
#include <Stepper.h>                                                    //Gebruik de stepper.h bibliotheek
const int stepsPerRevolution = 200;                                     //Hoeveel stappen worden er gezet in 1 toer? 360°/1,8° = 200
int pos = 0;                                                            //Start bij positie 0°
Stepper myStepper(stepsPerRevolution, 5, 6, 9, 10);                     //Pinnen toekennen voor het sturen van de stepper
boolean moveClockwise = true;                                           //Laat de stepper in wijzersin draaien

// Bluetooth
#include <SoftwareSerial.h>                                             //Gebruik de SoftwareSerial.h bibliotheek
SoftwareSerial EEBlue(0, 1); // RX | TX                                 //Pinnen toekennen voor het sturen en ontvangen van de bluetooth

// LCD
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x38, 16, 2);

// Pompen
int Hbrug = 19;
int Vodka = 18;                                                         // Relais 6
int Cranberry = 20;                                                     // Relais 4
int Sprite = 8;                                                         // Relais 3
int Gin = 21 ;                                                          // Relais 5
int Inschakeltijd = 500;                                                // Tijd die tussen de pompen zit

//ultrasoon sensor
int pingPin = A5;
long duration, cm;
long afstand;
int newvalue;
int oldvalue;
int naast;
int midden;

int cocktail;                                                           // De variable gelinkt aan de HC-05
boolean shot;


void setup()
{
  //begin seriële (bluetooth transmissie)
  Serial.begin(9600);
  Serial1.begin(9600);

  //opstart tijd voor brainbox arduino leonardo
  delay(1750);

  // Gebruik poorten 19-23 als OUTPUT
  pinMode(18, OUTPUT);
  pinMode(19, OUTPUT);
  pinMode(20, OUTPUT);
  pinMode(21, OUTPUT);
  pinMode(8, OUTPUT);

  // Relaisbord is laag actief
  digitalWrite(18, HIGH); //LINKSACHTER VODKA
  digitalWrite(19, HIGH);
  digitalWrite(20, HIGH); //RECHTSACHTER CRANBERRY
  digitalWrite(21, HIGH); //RECHTSVOOR GIN
  digitalWrite(8, HIGH ); //LINKSVOOR SPRITE

  // Setup voor LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);

  // Setup voor Servo
  myStepper.setSpeed(15);
}

void loop()
{
  cocktail = 0;
  if (Serial1.available() > 0 )          //Indien een keuze is gemaakt via de app:
  {
    cocktail = Serial1.read();
    lcd.clear();
    Serial.println("bezig");            //App laten weten dat de cocktail gemaakt wordt
    Serial.println(cocktail);
  }

  else
  {
    lcd.setCursor(1, 0);               //Wachten tot een keuze is gemaakt via de app:
    lcd.print("Maak een keuze");
    lcd.setCursor(2, 1);
    lcd.print("via de app!");
  }
  //---------------------------------------------------------------------------------------------------------------------------------------------------------
  // Cocktails:
  switch (cocktail) {

    //Vodka sprite
    case 'a':
      Ultrasoon(10);
      VodkaPomp(5000);
      GinPomp(3000);
      CranberryPomp(0);
      SpritePomp(0);
      einde();
      break;

    // Tom Collins
    case 'b':
      Ultrasoon(10);
      VodkaPomp(0);
      GinPomp(1000);
      CranberryPomp(0);
      SpritePomp(1000);
      einde();
      break;

    // Vodka Cranberry
    case 'c':
      Ultrasoon(10);
      VodkaPomp(1000);
      GinPomp(0);
      CranberryPomp(1000);
      SpritePomp(0);
      einde();
      break;

    // Cranberry Gin
    case 'd':
      Ultrasoon(10);
      VodkaPomp(0);
      GinPomp(1000);
      CranberryPomp(2000);
      SpritePomp(0);
      einde();
      break;

    // Vodka Shot
    case 'e':
      shot = true;
      VodkaPomp(1000);
      GinPomp(0);
      CranberryPomp(0);
      SpritePomp(0);
      einde();
      break;

    // Gin Shot
    case 'f':
      shot = true;
      VodkaPomp(0);
      GinPomp(1000);
      CranberryPomp(0);
      SpritePomp(0);
      einde();
      break;

    // Custom Cocktail
    case 'g':
      Ultrasoon(10);
      costum();
      einde();
      break;

    // Spoelen
    case 'n':
      VodkaPomp(100);
      GinPomp(100);
      CranberryPomp(100);
      SpritePomp(100);
      einde();
      break;
  }
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------
void Ultrasoon(int afstand) {
  int snelheid = 20;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Center het glas");

  while (cm != afstand) {
    digitalWrite(Hbrug, LOW);
    myStepper.step(snelheid);

    pinMode(pingPin, OUTPUT);
    digitalWrite(pingPin, LOW);
    delayMicroseconds(2);
    digitalWrite(pingPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(pingPin, LOW);
    pinMode(pingPin, INPUT);
    duration = pulseIn(pingPin, HIGH);
    cm = duration / 29 / 2;
    Serial.println(cm);
    delay(100);
    if (cm < 10) {
      snelheid = 1;
    }
  }

  digitalWrite(Hbrug, HIGH);
  lcd.clear();
  cm = 0;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------
void VodkaPomp(int InschakeltijdVodka) {
  int steppercount = 1;
  int getal;
  if (InschakeltijdVodka > 0) {
    Serial.println(InschakeltijdVodka);
    getal = 1;
    if (shot == true)
    {
      while (Serial1.available() == 0)
      {
        lcd.setCursor(0, 0);
        lcd.print("Aantal?");
      }
      if (Serial1.available() > 0 )
      {
        getal = Serial1.parseInt();
        Serial.println(getal);
        if (shot == true) {
          Ultrasoon(11);
        }
      }
    }
    for (int i = 0; i < getal; i++)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("     Vodka");
      lcd.setCursor(0, 1);
      digitalWrite(Vodka, LOW);
      for (int i = 0; i <= 16; i++)
      {
        lcd.write(255);
        delay(InschakeltijdVodka / 16);
      }
      digitalWrite(Vodka, HIGH);

      if (shot == true and getal > 1)
      {
        if (steppercount < getal)
        {
          digitalWrite(Hbrug, LOW);
          myStepper.step(50);
          delay(1000);
          digitalWrite(Hbrug, HIGH);
          steppercount++;
        }
      }
    }
    lcd.clear();
  }
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------
void GinPomp(int InschakeltijdGin)  {
  int steppercount = 1;
  int getal;
  if (InschakeltijdGin > 0) {
    Serial.println(InschakeltijdGin);
    getal = 1;
    if (shot == true) {
      while (Serial1.available() == 0) {
        lcd.setCursor(0, 0);
        lcd.print("Aantal?");
      }
      if (Serial1.available() > 0 )  {
        getal = Serial1.parseInt();
        Serial.println(getal);
        if (shot == true) {
          Ultrasoon(11);
        }
      }
    }
    for (int i = 0; i < getal; i++)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("     Gin");
      lcd.setCursor(0, 1);
      digitalWrite(Gin, LOW);
      for (int i = 0; i <= 16; i++)
      {
        lcd.write(255);
        delay(InschakeltijdGin / 16);
      }
      digitalWrite(Gin, HIGH);
      delay(500);
      if (shot == true)
      {
        if (steppercount < getal)
        {
          digitalWrite(Hbrug, LOW);
          myStepper.step(50);
          delay(1000);
          digitalWrite(Hbrug, HIGH);
          steppercount++;
        }
      }
      lcd.clear();
    }
  }
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------
void CranberryPomp(int InschakeltijdCranberry) {
  if (InschakeltijdCranberry > 0)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("   Cranberry");
    lcd.setCursor(0, 1);
    digitalWrite(Cranberry, LOW);
    for (int i = 0; i <= 16; i++)
    {
      lcd.write(255);
      delay(InschakeltijdCranberry / 16);
    }
    digitalWrite(Cranberry, HIGH);
    lcd.clear();
  }
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------
void SpritePomp(int InschakeltijdSprite)  {
  if (InschakeltijdSprite > 0) {
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("     Sprite");
      Serial.println("Sprite");
      lcd.setCursor(0, 1);
      digitalWrite(Sprite, LOW);
      for (int i = 0; i <= 16; i++)
      {
        lcd.write(255);
        delay(InschakeltijdSprite / 16);
      }
      digitalWrite(Sprite, HIGH);
      lcd.clear();
    }
  }
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------
void costum()
{
  int drank = 0;
  int lcdteller = 0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("    Max drank");
  while (drank != 'z')
  {
    if (Serial1.available() > 0 )
    {
      drank = Serial1.read();
      Serial.println(drank);

      switch (drank) {
        case 'h':
          digitalWrite(Vodka, LOW);
          break;
        case 'i':
          digitalWrite(Gin, LOW);
          break;
        case 'j':
          digitalWrite(Sprite, LOW);
          break;
        case 'k':
          digitalWrite(Cranberry, LOW);
          break;
        case 'l':
          if (lcdteller <= 16)
          {
            lcd.setCursor(lcdteller, 1);
            lcd.write(255);
            lcdteller++;
          }
          break;

        case 'm':
          digitalWrite(Vodka, HIGH);
          digitalWrite(Gin, HIGH);
          digitalWrite(Sprite, HIGH);
          digitalWrite(Cranberry, HIGH);
          break;
      }
    }
  }
  lcd.clear();
  drank = 0;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------
void einde()
{
  Serial1.write("klaar");
  lcd.clear();
  lcd.print("Drankje is klaar!");
  digitalWrite(22, HIGH);
  delay(1000);
  lcd.clear();
}
