#include <EEPROM.h>
int lval,hval,l_val,h_val;
 int v_min,v_max,cval;
short int dac_trim;
boolean debugMessage;
String inputString;
int f_width,raw1,raw2;
#define hall1_pin 2
#define hall2_pin 3
#define runout_pin 7
#define runout_buzzer_pin 6
#define hall_arduino_powered
#include <U8g2lib.h>

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);
unsigned long mil;
void setup() {
 
TCCR1A = TCCR1A & 0xe0 | 3;     // set for pin 10
TCCR1B = TCCR1B & 0xe0 | 0x09;  // 10 bit PWM 15625 Hz 0.005 V resolution 

Serial.begin(115200);
ee_read();
helpMessage();
inputString="";
  //u8g2_SetI2CAddress(u8g2.getU8g2(), 0X3A);
  u8g2.begin();
  u8g2.setPowerSave(0);
  u8g2.setFont(u8g2_font_fur42_t_symbol );

#ifdef hall_arduino_powered 
pinMode(14, OUTPUT);  
pinMode(15, OUTPUT);  
digitalWrite(15, HIGH); // a1 +5v
digitalWrite(14, LOW); // a0 -5v

#endif
pinMode(12, OUTPUT); // For ground capacitor
pinMode(10, OUTPUT);
pinMode(11, INPUT); // using for DAC output. Software disconected from atmega
pinMode(LED_BUILTIN, OUTPUT);
pinMode(runout_pin, OUTPUT);
digitalWrite(runout_pin, HIGH);
pinMode(runout_buzzer_pin, OUTPUT);
digitalWrite(runout_buzzer_pin, HIGH);

digitalWrite(12, LOW); // For ground capacitor
}

void get_width(){
unsigned int a=0;

for (byte t=0;t<30;t++){
  
a+=analogRead(hall1_pin)+analogRead(hall2_pin);
delay(1);}
a=a/6;

//a=4500;
cval=a;
float k=(float)(h_val-l_val)/(float)(v_max-v_min);
f_width= (float) (cval-v_min)*k +l_val;

int pw=(float) 1024/495*f_width;
analogWrite(10,pw+dac_trim);
}

void loop() {

for (byte t=0;t<67;t++) { // ~2s cycle
       get_width();   serial_check();  
      if ((f_width>hval)||(f_width<lval)) { 
        pinMode(runout_pin, LOW);   digitalWrite(LED_BUILTIN, HIGH); 
        if (t<45) pinMode(runout_buzzer_pin, LOW); else pinMode(runout_buzzer_pin, HIGH);  } 
        else 
            {pinMode(runout_buzzer_pin, HIGH); pinMode(runout_pin, HIGH);   digitalWrite(LED_BUILTIN, LOW); } // check limits
}
if (debugMessage) 
debug_Message();
u8g2.clearBuffer();
u8g2.setCursor(5, 60);
u8g2.print((float)f_width/100);
u8g2.sendBuffer(); 


  
 
}
