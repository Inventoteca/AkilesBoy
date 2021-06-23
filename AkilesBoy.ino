
// Codigo para sumobot

// -------------------------------------------------------------------------- Librerías
#include <NintendoExtensionCtrl.h>
#include <DRV8833.h>
#include "LedControl.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Servo.h> 


// -------------------------------------------------------------------------- definiciones
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
#define pinBrazo        13


// -------------------------------------------------------------------------- Objetos/variables
DRV8833 driver = DRV8833();
LedControl lc=LedControl(4,8,7,1);
const int inputA1 = 9, inputA2 = 6, inputB1 = 10, inputB2 = 11;
ClassicController classic;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Servo brazo;
int contador;

// botones del control
boolean success = classic.update();
boolean padUp = classic.dpadUp();
boolean padDown = classic.dpadDown();
boolean padRight = classic.dpadRight();
boolean padLeft = classic.dpadLeft();
  
boolean aButton = classic.buttonA();
boolean bButton = classic.buttonB();

byte feliz[8]= 
{
  B00111100,
  B01000010,
  B10100101,
  B10000001,
  B10100101,
  B10011001,
  B01000010,
  B00111100
};
byte triste[8]= 
{
  B00111100,
  B01000010,
  B10100101,
  B10000001,
  B10011001,
  B10100101,
  B01000010,
  B00111100
};
byte serio[8]= 
{
  B00111100,
  B01000010,
  B10100101,
  B10000001,
  B10000001,
  B10111101,
  B01000010,
  B00111100
};
byte corazon[8]= 
{
  B00000000,
  B01100110,
  B11111111,
  B11111111,
  B01111110,
  B00111100,
  B00011000,
  B00000000
};

byte boca1[8]= 
{
  B00000000,
  B00000000,
  B01111110,
  B11111111,
  B11111111,
  B01111110,
  B00000000,
  B00000000
};

byte boca2[8]= 
{
  B00000000,
  B00000000,
  B00000000,
  B01111110,
  B01111110,
  B00000000,
  B00000000,
  B00000000
};

byte boca3[8]= 
{
  B00000000,
  B00000000,
  B00000000,
  B01010101,
  B10101010,
  B00000000,
  B00000000,
  B00000000
};

byte boca4[8]= 
{
  B00000000,
  B00000000,
  B00000000,
  B11111111,
  B00100001,
  B00010010,
  B00001100,
  B00000000
};

byte boca5[8]= 
{
  B00000000,
  B00000000,
  B00000000,
  B11111111,
  B10000100,
  B01001000,
  B00110000,
  B00000000
};

// -------------------------------------------------------------------------- Setup
void setup() 
{
  Serial.begin(115200);
  
  classic.begin();
  lc.shutdown(0,false);
  lc.setIntensity(0,4);
  lc.clearDisplay(0);

  brazo.attach(pinBrazo);
  brazo.write(90);
  
  driver.attachMotorA(inputA1, inputA2);
  driver.attachMotorB(inputB1, inputB2);
  
  for (int i = 0; i < 8; i++)
  {
    lc.setRow(0,i,corazon[i]);
  }
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) 
  {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.setRotation(2);  // Se invierte de cabeza el display oled
  display.clearDisplay ();
  display.display ();
  

  // muestra un pixel en cada esquina de la pantalla
  //display.drawPixel (0, 0, WHITE);
  //display.drawPixel (127, 0, WHITE);
  display.drawPixel (127, 31, WHITE);
  //display.drawPixel (0, 31, WHITE);
 
  // muestra una línea de texto
  display.setTextSize (1);
  display.setTextColor (WHITE);
  display.setCursor (30,24); // x,y
  display.println("Akiles Boy");
  display.display();
  //delay(2000);

  // pregunta solo al inicio pero sigue de largo si no lo encuentra
  while(!classic.connect()) 
  {
    Serial.println("Classic Controller not detected!");
    delay(1000);
  }
  Serial.println("Piloto listo. Arrancando Sumobot.");
   
}


// -------------------------------------------------------------------------- loop
void loop() 
{

 radioControl();

  
}


// -------------------------------------------------------------------------- auto
void autonomo()
{
  
}


// ------------------------------------------------------------------------ RF
void radioControl(void)
{
  //display.setTextColor (WHITE);
  //display.setCursor (0,24); // x,y
  display.println("Radio control");
  display.display();
    
  success = classic.update();
  padUp = classic.dpadUp();
  padDown = classic.dpadDown();
  padRight = classic.dpadRight();
  padLeft = classic.dpadLeft();
  
  aButton = classic.buttonA();
  bButton = classic.buttonB(); 

    if(padUp==true || padDown==true || padRight==true || padLeft==true || aButton == true || bButton == true) //No hay movimiento
  {
    if(padUp==true) //Para adelante
  {
    adelante();
    display.setCursor (0,10); 
    display.print("adelante");
    display.display();
  }
  if(padDown==true) //Para atrás
  {
    atras();
    display.setCursor (0,10); 
    display.print("atras");
    display.display();
  }
  if(padRight==true) //Para derecha
  {
    derecha();
    display.setCursor (0,10); 
    display.print("derecha");
    display.display();
  }
  if(padLeft==true) //Para izquierda
  {
    izquierda();
    display.setCursor (0,10); 
    display.print("izquierda");
    display.display();
  }
  
  if(aButton==true) //A
  {
    a();
    display.setCursor (0,10); 
    display.print("A");
    display.display();
  }
  
  if(bButton==true) //B
  {
    b();
    display.setCursor (0,10); 
    display.print("B");
    display.display();
  }
  }
  
  //else
  {
    sinmovi();
    //display.println("stop");
    //display.display();
  }
}

// -------------------------------------------------------------------------- sinmovi
void sinmovi()
{
  driver.motorAStop();
  driver.motorBStop();
  if(contador > 40)
  {
    for (int i = 0; i < 8; i++)
    {
      lc.setRow(0,i,boca1[i]);
    }
  }

  if(contador > 60 && contador <80)
  {
    for (int i = 0; i < 8; i++)
    {
      lc.setRow(0,i,boca2[i]);
    }
  }

  else
  {
    contador=0;
  }
  
  contador++;
}

// -------------------------------------------------------------------------- adelante
void adelante()
{
  driver.motorAForward();
  driver.motorBForward();
  for (int i = 0; i < 8; i++)
  {
    lc.setRow(0,i,boca2[i]);
  }
}

// -------------------------------------------------------------------------- atras
void atras()
{
  driver.motorAReverse();
  driver.motorBReverse();
  for (int i = 0; i < 8; i++)
  {
    lc.setRow(0,i,boca4[i]);
  }
}

// -------------------------------------------------------------------------- derecha
void derecha()
{
  driver.motorAReverse();
  driver.motorBForward();
  for (int i = 0; i < 8; i++)
  {
    lc.setRow(0,i,boca3[i]);
  }
}

// -------------------------------------------------------------------------- izquiereda
void izquierda()
{
  driver.motorAForward();
  driver.motorBReverse();
  for (int i = 0; i < 8; i++)
  {
    lc.setRow(0,i,boca5[i]);
  }
}

// -------------------------------------------------------------------------- a
void a()
{
  brazo.write(0);
  for (int i = 0; i < 8; i++)
  {
    lc.setRow(0,i,corazon[i]);
  }
  //delay(1000);
}

// -------------------------------------------------------------------------- b
void b()
{
  brazo.write(180);
  for (int i = 0; i < 8; i++)
  {
    lc.setRow(0,i,boca2[i]);
  }
  //delay(1000);
}
