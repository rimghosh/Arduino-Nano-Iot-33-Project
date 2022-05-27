//Creator:Rimi Ghosh, refered Internet to create this script
#include "HX711.h"
float analog_pot=0;
#define DOUT 2
#define CLK 3
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

HX711 scale;

float calibration_factor =57400; 
//56400 worked for my 440lb max scale set


void setup() {
Serial.begin(9600);
scale.begin(DOUT, CLK);
Serial.println("HX711 calibration sketch");
Serial.println("Remove all weight from scale");
Serial.println("After readings begin, place known weight on scale");
Serial.println("Press + or a to increase calibration factor");
Serial.println("Press - or z to decrease calibration factor");

scale.set_scale();
scale.tare(); //Reset the scale to 0

long zero_factor = scale.read_average(); //Get a baseline reading
Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
Serial.println(zero_factor);

}

void loop() {
analog_pot=analogRead(A0);
scale.set_scale(calibration_factor); //Adjust to this calibration factor

Serial.print("Reading: ");
Serial.print(scale.get_units()*0.453592, 3);
Serial.print(" kg"); //Change this to kg and re-adjust the calibration factor if you follow SI units like a sane person
Serial.print(" calibration_factor: ");
Serial.print(calibration_factor);
Serial.println();


delay(100);

if(Serial.available())
{
char temp = Serial.read();
if(temp == '+' || temp == 'a')
calibration_factor += 10;
else if(temp == '-' || temp == 'z')
calibration_factor -= 10;

if(temp == 'q' || temp == 'w')
calibration_factor += 100;
else if(temp == 'e' || temp == 'r')
calibration_factor -= 100;


if(temp == 't' || temp == 'y')
calibration_factor += 10000;
else if(temp == 'u' || temp == 'i')
calibration_factor -= 10000;

}
}
