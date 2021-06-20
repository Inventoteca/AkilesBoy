#include <NintendoExtensionCtrl.h>
#include <DRV8833.h>
#include "LedControl.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

DRV8833 driver = DRV8833();
LedControl lc=LedControl(4,8,7,1);
const int inputA1 = 6, inputA2 = 9, inputB1 = 10, inputB2 = 11;
ClassicController classic;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

byte feliz[8]= {
  B00111100,
  B01000010,
  B10100101,
  B10000001,
  B10100101,
  B10011001,
  B01000010,
  B00111100
};
byte triste[8]= {
  B00111100,
  B01000010,
  B10100101,
  B10000001,
  B10011001,
  B10100101,
  B01000010,
  B00111100
};
byte serio[8]= {
  B00111100,
  B01000010,
  B10100101,
  B10000001,
  B10000001,
  B10111101,
  B01000010,
  B00111100
};
byte corazon[8]= {
  B00000000,
  B01100110,
  B11111111,
  B11111111,
  B01111110,
  B00111100,
  B00011000,
  B00000000
};

void setup() 
{
  Serial.begin(115200);
  classic.begin();
  lc.shutdown(0,false);
  lc.setIntensity(0,4);
  lc.clearDisplay(0);
  driver.attachMotorA(inputA1, inputA2);
  driver.attachMotorB(inputB1, inputB2);
  for (int i = 0; i < 8; i++)
  {
    lc.setRow(0,i,serio[i]);
  }
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) 
  {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  //display.display();
  //delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  // Draw a single pixel in white
  display.drawPixel(0, 0, SSD1306_WHITE);

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
  delay(2000);
  
  while (!classic.connect()) {
    Serial.println("Classic Controller not detected!");
    delay(1000);
  }
  Serial.println("Piloto listo. Arrancando Sumobot.");
   
}

void loop() 
{
  boolean success = classic.update();
  boolean padUp = classic.dpadUp();
  boolean padDown = classic.dpadDown();
  boolean padRight = classic.dpadRight();
  boolean padLeft = classic.dpadLeft();
  
  boolean aButton = classic.buttonA();
  boolean bButton = classic.buttonB();
  if(padUp==false && padDown==false && padRight==false && padLeft==false) //No hay movimiento
  {
    sinmovi();
  }

  if(padUp==true) //Para adelante
  {
    adelante();
  }
  else if(padDown==true) //Para atrás
  {
    atras();
  }
  else if(padRight==true) //Para derecha
  {
    derecha();
  }
  else if(padLeft==true) //Para izquierda
  {
    izquierda();
  }
  else if(aButton==true) //A
  {
    a();
  }
  else if(bButton==true) //B
  {
    b();
  }
  
}


void sinmovi(){
  driver.motorAStop();
  driver.motorBStop();
  for (int i = 0; i < 8; i++)
  {
    lc.setRow(0,i,corazon[i]);
  }
  
}

void adelante(){
  driver.motorAReverse();
  driver.motorBReverse();
  for (int i = 0; i < 8; i++)
  {
    lc.setRow(0,i,feliz[i]);
  }
}
void atras(){
  driver.motorAForward();
  driver.motorBForward();
  for (int i = 0; i < 8; i++)
  {
    lc.setRow(0,i,triste[i]);
  }
}
void derecha(){
  driver.motorBStop();
  driver.motorAReverse();
  for (int i = 0; i < 8; i++)
  {
    lc.setRow(0,i,serio[i]);
  }
}
void izquierda(){
  driver.motorBReverse();
  driver.motorAStop();
  for (int i = 0; i < 8; i++)
  {
    lc.setRow(0,i,serio[i]);
  }
}
void a(){
  for (int i = 0; i < 8; i++)
  {
    lc.setRow(0,i,corazon[i]);
  }
  delay(1000);
}
void b(){
  for (int i = 0; i < 8; i++)
  {
    lc.setRow(0,i,triste[i]);
  }
  delay(1000);
}
