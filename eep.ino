void ee_read(){
 
 byte cs=EEPROM.read(0);
 lval=EEPROM.read(1);
 hval=EEPROM.read(2);
 l_val=EEPROM.read(3);
 h_val=EEPROM.read(4);
 v_min=EEPROM.read(5);
 v_min+=EEPROM.read(6)*256;

 v_max=EEPROM.read(7);
 v_max+=EEPROM.read(8)*256;
dac_trim=EEPROM.read(10)-127;

cs-=lval+hval+l_val+h_val+(v_min&255)+(v_max&255);

if ((cs!=11)||(EEPROM.read(9)!=22)) { // set default value 
 lval=150;
 hval=200;
 l_val=150;
 h_val=200;
 v_min=5100;
 v_max=5950;  
dac_trim=0;
  Serial.print(F("Default settings loaded ")); 
  ee_write();
}
}

void ee_write(){
byte cs=11+lval+hval+l_val+h_val+(v_min&255)+(v_max&255);
 Serial.println(F("Settings stored"));
 EEPROM.write(0,cs);
 EEPROM.write(1,lval);
 EEPROM.write(2,hval);
 EEPROM.write(3,l_val);
 EEPROM.write(4,h_val);
 EEPROM.write(5,v_min&255);
 EEPROM.write(6,(v_min&0xff00)>>8);
 EEPROM.write(7,v_max&255);
 EEPROM.write(8,(v_max&0xff00)>>8);
 EEPROM.write(9,22);
 EEPROM.write(10,127+dac_trim);
}

