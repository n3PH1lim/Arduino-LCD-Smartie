#include <avr/interrupt.h>
#include <avr/io.h>
//#include "LCD4Bit_mod.h"
#include <LiquidCrystal.h>

int RS = 12;
int RW = 10;
int CONTRAST = 11;
int ENABLE = 9;

LiquidCrystal lcd(RS,ENABLE, 10, 5, 4, 3, 2);




void setup() { 
  pinMode(13, OUTPUT);  //we'll use the debug LED to output a heartbeat
  digitalWrite(13,LOW);
  Serial.begin(19200);
   
  //lcd.init();
  lcd.clear();
  lcd.print("**LCD  SMARTIE**");
  lcd.setCursor(2,3);
  lcd.print("on Arduino");
}


byte serial_getch(){
  
  int incoming;  
  while (Serial.available()==0){}
	// read the incoming byte:
  incoming = Serial.read();

  return (byte) (incoming &0xff);
}
        


void loop(){
  
  byte rxbyte;
  byte temp;
  byte col;
  byte row;
  
  rxbyte = serial_getch();

  if (rxbyte == 254) //Matrix Orbital uses 254 prefix for commands
	{
		switch (serial_getch())
		{
			case 66: //backlight on (at previously set brightness)
                                // not implemented				

				break;
			case 70: //backlight off
				// not implemented				
                                break;
			case 71:  //set cursor position
				col = (serial_getch() - 1);  //get column byte
                                row = (serial_getch());
                                
				/*
                                switch (serial_getch())  //get row byte
				{
					//line 1 is already set up
					case 2:
						temp += 0x40;
						break;
					case 3:
						temp += 0x14;
						break;
					case 4:
						temp += 0x54;
						break;
					default:
						break;
				}
*/
				//lcd.commandWrite(0b10000000 + temp);
                                lcd.setCursor(col,row-1);
				break;
			case 72:  //cursor home (reset display position)
				//lcd.commandWrite(2);
                                lcd.setCursor(0,0);
				break;
			case 74:  //show underline cursor
				//lcd.commandWrite(0b00001110);
				break;
			case 75:  //underline cursor off
			case 84:  //block cursor off
				//lcd.commandWrite(0b00001100);
				break;
			case 76:  //move cursor left
				//lcd.commandWrite(16);
				break;
			case 77:  //move cursor right
				//lcd.commandWrite(20);
				break;
			case 78:  //define custom char
				//lcd.commandWrite(64 + (serial_getch() * 8));  //get+set char address
				for (temp = 7; temp != 0; temp--)
				{
					lcd.print(serial_getch()); //get each pattern byte
				}
				break;
			case 83:  //show blinking block cursor
				//lcd.commandWrite(0b00001111);
				break;
			case 86:  //GPO OFF
				//implement later
				break;
			case 87:  //GPO ON
				/*temp = serial_getch();
				if (temp == 1)
				{
					GPO1 = GPO_ON;
				}*/
				break;
			case 88:  //clear display, cursor home
				//lcd.commandWrite(1);
                                lcd.clear();
                                lcd.setCursor(0,0);
				break;
			case 152: //set and remember (doesn't save value, though)
			case 153: //set backlight brightness
				//not implemented
				break;

			//these commands ignored (no parameters)
			case 35: //read serial number
			case 36: //read version number
			case 55: //read module type
			case 59: //exit flow-control mode
			case 65: //auto transmit keypresses
			case 96: //auto-repeat mode off (keypad)
			case 67: //auto line-wrap on
			case 68: //auto line-wrap off
			case 81: //auto scroll on
			case 82: //auto scroll off
			case 104: //init horiz bar graph
			case 109: //init med size digits
			case 115: //init narrow vert bar graph
			case 118: //init wide vert bar graph
				break;
			default:
				//all other commands ignored and parameter byte discarded
				temp = serial_getch();  //dump the command code
				break;
		}
		return;
	} //END OF COMMAND HANDLER

	//change accented char to plain, detect and change descenders
	//NB descenders only work on 5x10 displays. This lookup table works
	//  with my DEM-20845 (Display Elektronik GmbH) LCD using KS0066 chip.
	switch (rxbyte)
	{
		//chars that have direct equivalent in LCD charmap
/*		case 0x67: //g
			rxbyte = 0xE7;
			break;
		case 0x6A: //j
			rxbyte = 0xEA;
			break;
		case 0x70: //p
			rxbyte = 0xF0;
			break;
		case 0x71: //q
			rxbyte = 0xF1;
			break;
		case 0x79: //y
			rxbyte = 0xF9;
			break;
*/		case 0xE4: //ASCII "a" umlaut
			rxbyte = 0xE1;
			break;
		case 0xF1: //ASCII "n" tilde
			rxbyte = 0xEE;
			break;
		case 0xF6: //ASCII "o" umlaut
			rxbyte = 0xEF; //was wrong in v0.86
			break;
		case 0xFC: //ASCII "u" umlaut
			rxbyte = 0xF5;
			break;

		//accented -> plain equivalent
		//and misc symbol translation
		case 0xA3: //sterling (pounds)
			rxbyte = 0xED;
			break;
/*		case 0xB0: //degrees symbol
			rxbyte = 0xDF;
			break;
*/		case 0xB5: //mu
			rxbyte = 0xE4;
			break;
		case 0xC0: //"A" variants
		case 0xC1:
		case 0xC2:
		case 0xC3:
		case 0xC4:
		case 0xC5:
			rxbyte = 0x41;
			break;
		case 0xC8: //"E" variants
		case 0xC9:
		case 0xCA:
		case 0xCB:
			rxbyte = 0x45;
			break;
		case 0xCC: //"I" variants
		case 0xCD:
		case 0xCE:
		case 0xCF:
			rxbyte = 0x49;
			break;
		case 0xD1: //"N" tilde -> plain "N"
			rxbyte = 0x43;
			break;
		case 0xD2: //"O" variants
		case 0xD3:
		case 0xD4:
		case 0xD5:
		case 0xD6:
		case 0xD8:
			rxbyte = 0x4F;
			break;
		case 0xD9: //"U" variants
		case 0xDA:
		case 0xDB:
		case 0xDC:
			rxbyte = 0x55;
			break;
		case 0xDD: //"Y" acute -> "Y"
			rxbyte = 0x59;
			break;
/*		case 0xDF: //beta  //mucks up LCDSmartie's degree symbol??
			rxbyte = 0xE2;
			break;
*/		case 0xE0: //"a" variants except umlaut
		case 0xE1:
		case 0xE2:
		case 0xE3:
		case 0xE5:
			rxbyte = 0x61;
			break;
		case 0xE7: //"c" cedilla -> "c"
			rxbyte = 0x63;
			break;
		case 0xE8: //"e" variants
		case 0xE9:
		case 0xEA:
		case 0xEB:
			rxbyte = 0x65;
			break;
		case 0xEC: //"i" variants
		case 0xED:
		case 0xEE:
		case 0xEF:
			rxbyte = 0x69;
			break;
		case 0xF2: //"o" variants except umlaut
		case 0xF3:
		case 0xF4:
		case 0xF5:
		case 0xF8:
			rxbyte = 0x6F;
			break;
		case 0xF7: //division symbol
			rxbyte = 0xFD;
			break;
		case 0xF9: //"u" variants except umlaut
		case 0xFA:
		case 0xFB:
			rxbyte = 0x75;
			break;
		default:
			break;
	}

	lcd.print(rxbyte);  //otherwise a plain char so we print it to lcd
	return;


}


