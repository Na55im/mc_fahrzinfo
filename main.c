/*---------------------------------------------------------------------------------------
 * FahrzeugInfoDisplay: 
 	Read speed and direction from rotary encoder and print to LCD
 *                                                                     Agrebi, Stetter 2017
 *---------------------------------------------------------------------------------------
*/

#include "tm4c1294ncpdt.h"
#include "stdio.h"
#include "mpp1.h"


//MACROS
#define PIN_RD 0x01
#define PIN_WR 0x02
#define PIN_RS 0x04
#define PIN_CS 0x08
#define PIN_RST 0x10





/*void wait(unsigned int time){
 //1/16Mhz = 62,5nS, 62,5*3= 187,5nS. 5,3*time= waittime, SysCtlDelay iterates 3 times
//enable the clock
SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); //on PIN F
time = time * 5.333333; // time in us
SysCtlDelay(time);
}
*/

void wait(int clicks){
	volatile int tmp;
	for(tmp=0;tmp<clicks;tmp++);
}


void initialise_setup(void)
{
	//SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

			 //Port  Clock Gating Control
			  	 SYSCTL_RCGCGPIO_R  |= 0x0C08; //clock enable Port M + L
		 	 	 while((SYSCTL_PRGPIO_R & 0x0C08) == 0); //wait for clock enable
		 	 	//SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOM);
		 	 //SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOL);
			//Enable Ports
			 	 GPIO_PORTL_DEN_R = 0x1F; //enable Prt L 0-4
			 	 GPIO_PORTM_DEN_R = 0xFF; //enable Port M 0-7
			 //Set direction
			 	 GPIO_PORTL_DIR_R = 0x1F; //Port L 0-4 = Output
			 	 GPIO_PORTM_DIR_R = 0xFF; //Port M 0-7 = Output

}

void write_command(unsigned char command)
{
	GPIO_PORTL_DATA_R = 0x1F; //initial state
	/*GPIO_PORTL_DATA_R &= ~(PIN_CS); //CS=0
	GPIO_PORTL_DATA_R &= ~(PIN_RS); //RS=0
	GPIO_PORTL_DATA_R &= ~(PIN_WR); //WR=0
	*/
	GPIO_PORTL_DATA_R = 0x11;
	GPIO_PORTM_DATA_R = command; //write command
	/*GPIO_PORTL_DATA_R |= PIN_WR; //WR=1
	GPIO_PORTL_DATA_R |= PIN_CS; //CS=1
	*/
	GPIO_PORTL_DATA_R = 0x1B;
}


void write_data (unsigned char data)
{
	GPIO_PORTL_DATA_R = 0x1F; //initial state
	/*GPIO_PORTL_DATA_R &= ~(PIN_CS); //CS=0
	GPIO_PORTL_DATA_R |= (PIN_RS); //RS=1
	GPIO_PORTL_DATA_R &= ~(PIN_WR); //WR=0*/
	GPIO_PORTL_DATA_R = 0x15;
	GPIO_PORTM_DATA_R = data; //write command
	/*GPIO_PORTL_DATA_R |= PIN_WR; //WR=1
	GPIO_PORTL_DATA_R |= PIN_CS; //CS=1*/
	GPIO_PORTL_DATA_R = 0x1B;

}

void window_set(unsigned int start_x, unsigned int end_x, unsigned int start_y, unsigned int end_y)
{
	//X
	write_command(0x2A); //set page address
	write_data(start_x>>8); //set start page address HB
	write_data(start_x); //set start page address LB
	write_data(end_x>>8); //set end page address HB
	write_data(end_x); //set end page address LB

	//Y
	write_command(0x2B); //set column address
	write_data(start_y>>8); //set start column address HB
	write_data(start_y); //set start column address LB
	write_data(end_y>>8); //set end column address HB
	write_data(end_y); //set end column address LB
}


void initialise_ssd1963(void)
{
	//GPIO_PORTL_DATA_R &= ~(PIN_RST); //RST=0
	GPIO_PORTL_DATA_R = 0x00; //RST=0
	wait(100); //wait > 100u
	////
	write_command(0x01); //software reset
	wait(5000); //wait >5m
	//
	write_command(0xE2); //sett PLL f=100Mhz
	write_data(0x1D);
	write_data(0x02);
	write_data(0x04);
	////
	write_command(0xE0); //start PLL
	write_data(0x01);
	wait(100); //wait > 100u
	//////
	write_command(0xE0); //lock PLL
	write_data(0x03);
	wait(100); //wait > 100us
	//////
	write_command(0x01); //software reset
	wait(5000); //wait >5m
	////
	write_command(0xE6); //set LCD pixel CLK = 9 MHz
	write_data(0x01);
	write_data(0x70);
	write_data(0xA3);
	write_command(0xB0); //set LCD panel mode
	write_data(0x20); //TFT panel 24bit
	write_data(0x00); //TFT mode
	write_data(0x01); //horizontal size 480-1 HB
	write_data(0xDF); //horizontal size 480-1 LB
	write_data(0x01); //vertical size 272-1 HB
	write_data(0x0F); //vertical size 272-1 LB
	write_data(0x00); //even/odd line RGB
	///////////
	write_command(0xB4); //set horizontal period
	write_data(0x02); //set HT total pxel = 531 HB
	write_data(0x13); //LB
	write_data(0x00); //set horiz sync start = 43 HB
	write_data(0x2B); //LB
	write_data(0x0A); //set horiz sync = 10
	write_data(0x00); //set horiz sync start pos = 8 HB
	write_data(0x08); //LB
	write_data(0x00);
	///////
	write_command(0xB6); //set vertical period
	write_data(0x01); //set VT lines = 288 HB
	write_data(0x20); //LB
	write_data(0x00); //set VPS = 12 HB
	write_data(0x0C); //LB
	write_data(0x0A); //vertical sync pulse = 10
	write_data(0x00); //vert sync pulse start = 4 HB
	write_data(0x04); //LB
	//////////
	write_command(0x36); //flip display
	write_data(0x03);
	//////////
	write_command(0xF0); //pixel data format = 8bit
	write_data(0x00);
	//////////
	write_command(0x29); //display on

}


void clear_screen(unsigned char r, unsigned char g, unsigned char b)
{
	int x, y;
	window_set(0,480,0,827);
	write_command(0x2C);
	for(y = 0; y<= 272; y++)
		for(x=0; x<=480; x++){
			write_command(0x2C);
			write_data(r);
			write_data(g);
			write_data(b);
		}

}

int main(void)
{
	initialise_setup();
	initialise_ssd1963();


	clear_screen(0x00, 0x44, 0x55);


	while(1);
}
