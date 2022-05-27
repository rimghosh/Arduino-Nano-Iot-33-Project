/*
 Started with example code written by Nathan Seidle from SparkFun Electronics and added
 LCD output with gram and ounce values.
 
 Setup your scale and start the sketch WITHOUT a weight on the scale
 Once readings are displayed place the weight on the scale
 Press +/- or a/z to adjust the calibration_factor until the output readings match the known weight

 Arduino pin 6 -> HX711 CLK
 Arduino pin 5 -> HX711 DOUT
 Arduino pin 5V -> HX711 VCC
 Arduino pin GND -> HX711 GND
 
 The HX711 board can be powered from 2.7V to 5V so the Arduino 5V power should be fine.
 
 The HX711 library can be downloaded from here: https://github.com/bogde/HX711
*/

//#include <LiquidCrystal.h>
//LiquidCrystal lcd(51, 50, 49, 48, 47, 46);


#include "HX711.h"

#define DOUT 2
#define CLK  3

HX711 scale(DOUT, CLK);

float calibration_factor = 2125; //-7050 worked for my 440lb max scale setup
float units;
float ounces;

void setup() {
  Serial.begin(9600);
  // set up the LCD's number of columns and rows:
  /*lcd.begin(20, 4);
  lcd.setCursor(0, 0);
  lcd.print("HX711 calibration");*/
  Serial.println("HX711 calibration sketch");
  Serial.println("Remove all weight from scale");
  Serial.println("After readings begin, place known weight on scale");
  Serial.println("Press + or a to increase calibration factor");
  Serial.println("Press - or z to decrease calibration factor");

  scale.set_scale();
  scale.tare();  //Reset the scale to 0

  long zero_factor = scale.read_average(); //Get a baseline reading
  Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
  Serial.println(zero_factor);
}

void loop() {

  scale.set_scale(calibration_factor); //Adjust to this calibration factor

  Serial.print("Reading: ");
  units = scale.get_units(), 10;
  if (units < 0) {
    units = 0.00;
  }
  ounces = units * 0.035274; //Convert grams to ounces
  Serial.print(units);
  Serial.print(" grams"); 
  Serial.print(" calibration_factor: ");
  Serial.print(calibration_factor);
  Serial.println();
  /*lcd.setCursor(0, 1);
  lcd.print("Grams: ");
  lcd.print(units);
  lcd.setCursor(0, 2);
  lcd.print("Ounce: ");
  lcd.print(ounces);
  lcd.setCursor(0, 3);
  lcd.print("Calbr: ");
  lcd.print(calibration_factor);*/

  if(Serial.available())
  {
    char temp = Serial.read();
    if(temp == '+' || temp == 'a')
      calibration_factor += 1;
    else if(temp == '-' || temp == 'z')
      calibration_factor -= 1;
  }
}
