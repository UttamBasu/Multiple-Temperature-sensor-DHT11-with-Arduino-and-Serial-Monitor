/*For OLED connection
   GND = GND
   VCC = +5V or +3.3V
   SCL = A5 (for Arduino Uno), 21 (For Arduino Mega), A5 (For Arduino Nano), 21 (For Arduino Leonardo)
   SDA = A4 (for Arduino Uno), 20 (For Arduino Mega), A4 (For Arduino Nano), 20 (For Arduino Leonardo)
*/

/*For 3 numbers of DHT11 Temperature Sensor
   VCC of all sensor connected with 5V
   GND of all sensor connected with GND
   A0 for motor temperature sensor DATA
   A1 for Controller temperature sensor DATA
   A2 for Battery temperature sensor DATA
   
   This Programm is made by Uttam Basu
*/


//include display file of the 1.3' OLED Display (128X64)
#include "U8glib.h"
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0);

//include dht sensor and define each pin
#include<dht.h>

//define input pin for all sensor
#define dhtpin_motor  A0
#define dhtpin_controller  A1
#define dhtpin_battery  A2

//define each temperature sensor name
dht dhtmot;
dht dhtcont;
dht dhtbat;

//Declare initial value for all temperature sensor
int mot_temp = 0;
int mot_hmdt = 0;
int cont_temp = 0;
int cont_hmdt = 0;
int bat_temp = 0;
int bat_hmdt = 0;

void u8g_prepare(void)
{
  u8g.setFont(u8g_font_6x10);
  u8g.setFontRefHeightExtendedText();
  u8g.setDefaultForegroundColor();
  u8g.setFontPosTop();
}

void intro(uint8_t a)
{
  u8g.setFont(u8g_font_unifont);
  u8g.drawStr(2, 19, "Welcome");
  u8g.drawStr(2, 38, "to");
  u8g.drawStr(2, 57, "Terra Motors :)");
}

void MotorTemp(uint8_t a)
{
  u8g.setFont(u8g_font_unifont);
  u8g.drawStr(1, 10, "Motor Temp");  //1 from left side and 10 from above position.
  u8g.setPrintPos(1, 25); //1 from left side and 25 from above position.
  u8g.print(mot_temp, 1);
  u8g.drawStr(20, 25, "C");  //20 from the left side and 25 from above position.

  u8g.drawStr(1, 40, "Motor Humidity");
  u8g.setPrintPos(1, 55); //1 from the left side and 55 from above position.
  u8g.print(mot_hmdt, 1);
  u8g.drawStr(20, 55, "%");  //20 from the left side and 55 from above position.
}

void ControllerTemp(uint8_t a)
{
  u8g.setFont(u8g_font_unifont);
  u8g.drawStr(1, 10, "Cont Temp");  //1 from left side and 10 from above position.
  u8g.setPrintPos(1, 25); //1 from left side and 25 from above position.
  u8g.print(cont_temp, 1);
  u8g.drawStr(20, 25, "C");  //20 from the left side and 25 from above position.

  u8g.drawStr(1, 40, "Cont Humidity");
  u8g.setPrintPos(1, 55); //1 from the left side and 55 from above position.
  u8g.print(cont_hmdt, 1);
  u8g.drawStr(20, 55, "%");  //20 from the left side and 55 from above position.
}

void BatteryTemp(uint8_t a)
{
  u8g.setFont(u8g_font_unifont);
  u8g.drawStr(1, 10, "Battery Temp");  //1 from left side and 10 from above position.
  u8g.setPrintPos(1, 25); //1 from left side and 25 from above position.
  u8g.print(bat_temp, 1);
  u8g.drawStr(20, 25, "C");  //20 from the left side and 25 from above position.

  u8g.drawStr(1, 40, "Battery Humidity");
  u8g.setPrintPos(1, 55); //1 from the left side and 55 from above position.
  u8g.print(bat_hmdt, 1);
  u8g.drawStr(20, 55, "%");  //20 from the left side and 55 from above position.
}

uint8_t draw_state = 4;

void draw(void)
{
  u8g_prepare();
  switch (draw_state >> 2) //Total number of case we want to print, out total case is 4
  {
    case 0: intro(draw_state & 7); break;
    case 1: MotorTemp(draw_state & 7); break;
    case 2: ControllerTemp(draw_state & 7); break;
    case 3: BatteryTemp(draw_state & 7); break;
  }
}

void setup(void)
{
  Serial.begin(9600);

  //Declare Columns Name of the Result
  Serial.println("LABEL,Date,Time,Motor Temperature,Controller Temperature,Battery Temperature, Motor Humidity, Controller Humidity, Battery Humidity");
}

void loop(void)
{
  //for Motor temperature read
  dhtmot.read11(dhtpin_motor);
  mot_temp = dhtmot.temperature;
  mot_hmdt = dhtmot.humidity;

  //for Controller temperature read
  dhtcont.read11(dhtpin_controller);
  cont_temp = dhtcont.temperature;
  cont_hmdt = dhtcont.humidity;

  //for Battery temperature read
  dhtbat.read11(dhtpin_battery);
  bat_temp = dhtbat.temperature;
  bat_hmdt = dhtbat.humidity;

  // picture loop (It is compulsory)
  u8g.firstPage();
  do
  {
    draw();
  }
  while (u8g.nextPage());

  //increase the state
  draw_state++;
  if ( draw_state >= 5 * 4)  //here put the value of number of case and multiple with case+1
    draw_state = 0;
  delay(500);
  Serial.println( (String) "DATA,DATE,TIME" + "," + mot_temp + "," + cont_temp + "," + bat_temp + "," + mot_hmdt+ "," + cont_hmdt+ "," + bat_hmdt);
}
