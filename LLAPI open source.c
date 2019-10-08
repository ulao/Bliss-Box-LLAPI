/*
This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

/* The following is the work of Bliss-Box LLC and to be share with the above GLP License. 
	
	this code handles the sending and received of the data in to and out of the Bliss-Box products. 
	This protocol was developed to be used and shared in anyway. 
	Author Bliss-Box LLC
	contributors Jamie Dickson, Sean Green, and Cray Elliott.
	Honorable mention to sorg for his MisSter project (what would we be without you)
*/
//the following is based on a 16MHz clock on a ATmega32U4 


#include <util/delay.h> 
#include <avr/eeprom.h>
#include <avr/wdt.h> 
static unsigned int setParm=0;
static unsigned char parmType=0;
#define ERROR_UNKNOWN_COMMAND 0xf1;
#define ERROR_NO_ERROR 0xf0;
#define ERROR_NO_COMMAND 0xff;
#define PARM_RUMBLE 01
#define PARM_MODES 02
#define PARM_MAPPER 03
#define PARM_PLAYER 04
#define _DATA 0x01
#define _LATCH 0x04
#define debug 1


#define END_SIG do {PORTD |= _DATA;_delay_us(.8);PORTD &= ~_DATA;} while(0)
#define END_TALK do { DDRD |= 0x05;} while(0)
static unsigned char outData[15];
static unsigned char rumbleLevel=255;
static unsigned char rumbleLoop=255;

char sendData(char size ) 
{
	

	//reply command needs a buffer and time to reply. 
	_delay_us(5);//leave some us in-between. 
	PORTD |= 0x05; _delay_us(1);//let bridge sync in (any amount of time is ok here)
	PORTD &= ~0x05;_delay_us(.5); //Low for 1.5 us before the first rising  bit.
	for (char data=0;data<size;data++)
	{
		//_delay_us(.2);
		char bit=0;
		for (;bit<8;bit++)
		{

			PORTD |= _DATA;//set bit hi
			_delay_us(1.6);
			if ( ~outData[data] & 0x01 ) {_delay_us(1.4); asm("nop\n");} else {PORTD &= ~_DATA;_delay_us(1.31); asm("nop\n");}
			outData[data] >>=1;
		}
		END_SIG; //end byte signature. leave latch low.
	}
	END_TALK;//both high
 
}
char getBridgeCommand()
{
 DDRB |= debug;
 PORTB |= debug;
 PORTB &= ~debug;
 
 
	unsigned char command=0xff;
	//send the ok to talk signal	
	DDRD &= ~0x05;//set both sending or not. 
	PORTD |= 0x05;//pull up/in, update is finished.
	_delay_us(1);

	while ( (PIND & 0x05) == 0x05) {wdt_reset(); }//wait for talk. loop while high
 
	_delay_us(3.4);//sync
	for (char i = 0; i < 8; i++) { command >>=1; if ( (PIND & _DATA) != _DATA) command |= 0x80; _delay_us(4); }//read command.
	
	DDRD |= 0x05;
	PORTD &= ~0x05;//set both sending or not. Remember in all cases leave out and low returning 1 or 0. Once it loops it will set hi and in.

	// set setParm gets data for the length of the parm
	// i.e setParm=3 means 3 more bytes to come. 
	if (setParm)
	{
		if ( parmType == PARM_MODES) 
		{
			/***************************
			* Bliss box related flags (options)
			* uncomment if you wish to use this for your own intentions. 
			*/
			//if (command & 0x08) _HOTSWAP_DISABLED=1; else _HOTSWAP_DISABLED=0;
			//if (command & 0x10) _UDLR=1; else _UDLR=0;
			//if (command & 0x40) _AUTO_PAUSE_DIABLED=1;else _AUTO_PAUSE_DIABLED=0;
		}
		else if ( parmType == PARM_PLAYER && setParm == 1)  eeprom_update_byte(EE_PLAYER_NUMBER,command);
		else if ( parmType == PARM_RUMBLE )
		{
		  	  if (setParm == 1) rumbleLevel=command;  
				else /* == 2 */  rumbleLoop=command;
		}
		else if (parmType == PARM_MAPPER ) 
		{//mapper[24]={1,2,3,4,5, 6, 7, 8, 9,10,11 ,12 ,13 ,14 ,15,16,17,18,19,20,21,22,23,24}; 
			 mapper[23 - setParm]=command;
			 for (char i=0;i<24;++i) mapper[i]=i+1; 
		}
		setParm --;
		return 1; //no need for replies and this could repeat a command so we best exit here. 
	} else  { parmType = 0 ;} //clear parm type
	
		//carry out command. 
	if ( command==0) { return 0;}//return with poll
	
	else if (command==1) //get payload.
	{
		//reportBuffer is the button data as seen in Rhysh'a implementation. 
		outData[0]=_cntrl;
		for (char i =1;i<14;i++) { outData[i]=reportBuffer[i];}
		sendData(13);
		return 1; //for no error
	}
	else if (command==2) //get pressure.
	//psx_pressure_data is the array from the psx controller, use your own implementation. 
	{
		outData[0]=reportBuffer[0];
		for (char i =0;i<13;i++) { outData[i+1]= psx_pressure_data[i] ;}
		sendData(13);
	    return 1; //for no error
	}
	
	 
	//1X commands Rumble
	//curGamepad is a pointer to the Bliss-Box code, below it is use to rumble the controller. uncomment if desired
	//else if (command==0x11) { curGamepad->rumbleConst(rumbleLevel,rumbleLoop);}//start parm
	//else if (command==0x12) { curGamepad->rumbleConst(0,0);	}//end	
	//else if (command==0x14) { curGamepad->rumbleSine(rumbleLevel,rumbleLoop);}//start parm 
	//else if (command==0x18) { curGamepad->rumbleSine(0,0);	  }//end	
	//else if (command==0x1A) { curGamepad->rumbleConst(255,10); }//jolt
	//else if (command==0x1B) { curGamepad->rumbleSine(255,10);  }//jolt
	else if (command==0x1C) { setParm = 2;parmType=PARM_RUMBLE;}//set Const parms. (level, loop)
	
	//sets  gets  2x commands
	else if (command==0x20) {setParm = 1; parmType=PARM_MODES;}
	else if (command==0x21)
	{
		//this code is to get Bliss-Box data and features. User where needed. 
		/*
		outData[0]= _autoPause;
		//1
		//free outBuffer[1] |= (_TURBO_MODE << 2);
		outData[0] |= (_HOTSWAP_DISABLED << 3);
		outData[0] |= (_UDLR << 4);
		//outData[0] |= (_ANALOG_TO_DPAD<< 5);
		outData[0] |= (_AUTO_PAUSE_DIABLED<< 6);
		outData[0] |= (_DPAD_ONLY << 7);
		*/
		sendData(1);
	}
	else if (command==0x22) {setParm = 23; parmType=PARM_MAPPER;}
	else if (command==0x23) {setParm = 1; parmType=PARM_PLAYER;}
	else if (command==0xff){outData[0] = ERROR_NO_COMMAND; sendData(1); } 
	else  {outData[0] = ERROR_UNKNOWN_COMMAND; sendData(1); }

	
	return 1; //return with no poll and show return response
}
	
	