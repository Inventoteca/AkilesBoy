
// Codigo para sumobot

// -------------------------------------------------------------------------- Librerías
#include <NintendoExtensionCtrl.h>
#include <DRV8833.h>
#include "LedControl.h"
#include <SPI.h>
//#include <Wire.h>
//#include <Adafruit_GFX.h>
//#include <Adafruit_SSD1306.h>
#include <Servo.h> 
#include "caras.h"


// -------------------------------------------------------------------------- definiciones
//#define SCREEN_WIDTH 128 // OLED display width, in pixels
//#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
//#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
//#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32


// ----------------------------Defino pines a usar
#define pinBrazo        13
//#define pinTouch1       12
//#define pinTouch2       5
#define pinPisoD        A1
#define pinPisoI        A0


// -------------------------------------------------------------------------- Objetos/variables
DRV8833 driver = DRV8833();
LedControl lc=LedControl(4,8,7,1);
const int inputA1 = 9, inputA2 = 6, inputB1 = 10, inputB2 = 11;
ClassicController classic;
//Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
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

boolean sinMov = true;

// -------------------------------------------------------------------------- Setup
void setup() 
{
  Serial.begin(115200);
  
  classic.begin();
  lc.shutdown(0,false);
  lc.setIntensity(0,4);
  lc.clearDisplay(0);

  // Configuro el servo
  brazo.attach(pinBrazo);
  brazo.write(160);     //abajo

  // Configuro los pines touch
  //pinMode(pinTouch1,INPUT);
  //pinMode(pinTouch2,INPUT);
  
  driver.attachMotorA(inputA1, inputA2);
  driver.attachMotorB(inputB1, inputB2);
  
  for (int i = 0; i < 8; i++)
  {
    lc.setRow(0,i,corazon[i]);
  }
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  
  //if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) 
  //{
  //  Serial.println(F("SSD1306 allocation failed"));
  //  for(;;); // Don't proceed, loop forever
  //}

  //display.setRotation(2);  // Se invierte de cabeza el display oled
  //display.clearDisplay ();
  //display.display ();
  

  // muestra un pixel en cada esquina de la pantalla
  //display.drawPixel (0, 0, WHITE);
  //display.drawPixel (127, 0, WHITE);
  //display.drawPixel (127, 31, WHITE);
  //display.drawPixel (0, 31, WHITE);
 
  // muestra una línea de texto
  //display.setTextSize (1);
  //display.setTextColor (WHITE);
  //display.setCursor (30,24); // x,y
  //display.println("Akiles Boy");
  //display.display();
  //delay(2000);

  // pregunta solo al inicio pero sigue de largo si no lo encuentra
  while(!classic.connect()) 
  {
    Serial.println("Classic Controller not detected!");
    delay(100);
  }
  Serial.println("Piloto listo. Arrancando Sumobot.");
   
}


// -------------------------------------------------------------------------- loop
void loop() 
{

 radioControl();
 //autonomo();
 //display.display();
  
}


// -------------------------------------------------------------------------- auto
void autonomo()
{
  //display.setCursor (0,0); // x,y
  //display.println("             ");
  //display.setCursor (0,0); // x,y
  //display.println("Autonomo");
  //display.drawCircle(64, 10, 10, WHITE);
  

  //if((analogRead(pinPisoI)> 75) || (analogRead(pinPisoD)> 75))
  //  display.fillCircle(64, 10, 10, WHITE);
  
  //else
  //{
  //  display.fillCircle(64, 10, 10, BLACK);
  //  display.drawCircle(64, 10, 10, WHITE);
  //}

  // Una lectura analógica de borde negro da arriba de 30 en el ADC
  //display.setCursor (10,20); // x,y
  //display.fillRect(10, 20, 20, 10, BLACK);
  //display.println(analogRead(pinPisoI));

  //display.setCursor (100,20); // x,y
  //display.fillRect(100, 20, 20, 10, BLACK);
  //display.println(analogRead(pinPisoD));

  Serial.print(analogRead(pinPisoI));
  Serial.print("\t");
  Serial.println(analogRead(pinPisoD));
}


// ------------------------------------------------------------------------ RF
void radioControl(void)
{
  //display.setTextColor (WHITE);
  //display.setCursor (0,0); // x,y
  //display.println("Radio control:");
    
  success = classic.update();
  padUp = classic.dpadUp();
  padDown = classic.dpadDown();
  padRight = classic.dpadRight();
  padLeft = classic.dpadLeft();
  
  aButton = classic.buttonA();
  bButton = classic.buttonB();

  
  //display.setTextColor(WHITE); 
  //display.setCursor (0,10);  

    if(padUp==true || padDown==true || padRight==true || padLeft==true || aButton == true || bButton == true) //No hay movimiento
  {
     sinMov = false;
    
     if(padUp==true) //Para adelante
      {
        adelante();
        //display.setCursor (0,10);
        //display.print("W");
      }
     
     if(padDown==true) //Para atrás
      {
        atras();
        //display.setCursor (0,10);
        //display.print("S");
      }
    
    if(padRight==true) //Para derecha
      {
        derecha();
        //display.setCursor (0,10);
        //display.print(">");
      }
    
    if(padLeft==true) //Para izquierda
    {
      izquierda();
      //display.setCursor (0,10);
      //display.print("<");
    }
    
    }
    
    else
    {
      sinmovi(); 
      sinMov = true;
      //display.setCursor (0,10);
      //display.setTextColor(BLACK); 
      //display.write(127); // Borrar
      //display.setTextColor(WHITE); 
    }

    if(aButton==true) //A
    {
      a();
      //display.setCursor (0,10);
      //display.print("A");
    }
    
    if(bButton==true) //B
    {
      b();
      //display.setCursor (0,10);
      //display.print("B");
    }

    // ------- Cambio de cara
    if(contador < 100 && contador >0 && (sinMov == true))
    {
      for (int i = 0; i < 8; i++)
      {
        lc.setRow(0,i,boca1[i]);
      }
    }

    if(contador > 100 && contador <200 && sinMov== true)
    {
      for (int i = 0; i < 8; i++)
      {
        lc.setRow(0,i,boca2[i]);
      }
    }

    else if (contador >= 200 || sinMov == false)
    {
      contador=0;
    }
  
    contador++;
    Serial.println(contador);
}

// -------------------------------------------------------------------------- sinmovi
void sinmovi()
{
  driver.motorAStop();
  driver.motorBStop();
  
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
    lc.setRow(0,i,boca3[i]);
  }
}

// -------------------------------------------------------------------------- derecha
void derecha()
{
  driver.motorAReverse();
  driver.motorBForward();
  for (int i = 0; i < 8; i++)
  {
    lc.setRow(0,i,boca4[i]);
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
  brazo.write(60);
  for (int i = 0; i < 8; i++)
  {
    lc.setRow(0,i,corazon[i]);
  }
  //delay(1000);
}

// -------------------------------------------------------------------------- b
void b()
{
  brazo.write(160);
  for (int i = 0; i < 8; i++)
  {
    lc.setRow(0,i,boca2[i]);
  }
  //delay(1000);
}
