/*---------------------------------------------------------------------------------------
 * FahrzeugInfoDisplay: 
 	Read speed and direction from rotary encoder and print to LCD
 *                                                                     Agrebi, Stetter 2017
 *---------------------------------------------------------------------------------------
*/

#include "tm4c1294ncpdt.h"
#include "stdio.h"
#include "mpp1.h"
#include "window.h"


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

void write_command()
{
	GPIO_PORTL_DATA_R = 0x1F; //initial state
	/*GPIO_PORTL_DATA_R &= ~(PIN_CS); //CS=0
	GPIO_PORTL_DATA_R &= ~(PIN_RS); //RS=0
	GPIO_PORTL_DATA_R &= ~(PIN_WR); //WR=0
	*/
	GPIO_PORTL_DATA_R = 0x11;
	GPIO_PORTM_DATA_R = g_window.command; //write command
	/*GPIO_PORTL_DATA_R |= PIN_WR; //WR=1
	GPIO_PORTL_DATA_R |= PIN_CS; //CS=1
	*/
	GPIO_PORTL_DATA_R = 0x1B;
}

void write_data()
{
	GPIO_PORTL_DATA_R = 0x1F; //initial state
	/*GPIO_PORTL_DATA_R &= ~(PIN_CS); //CS=0
	GPIO_PORTL_DATA_R |= (PIN_RS); //RS=1
	GPIO_PORTL_DATA_R &= ~(PIN_WR); //WR=0*/
	GPIO_PORTL_DATA_R = 0x15;
	GPIO_PORTM_DATA_R = g_window.data; //write data
	/*GPIO_PORTL_DATA_R |= PIN_WR; //WR=1
	GPIO_PORTL_DATA_R |= PIN_CS; //CS=1*/
	GPIO_PORTL_DATA_R = 0x1B;

}

void window_set()
{
	//X
	g_window.command = 0x2A;
	write_command(); //set page address
	g_window.data = g_window.start_x >> 8;
	write_data(); //set start page address HB
	g_window.data = g_window.start_x;
	write_data(); //set start page address LB
	g_window.data = g_window.end_x >> 8;
	write_data(); //set end page address HB
	g_window.data = g_window.end_x;
	write_data(); //set end page address LB

	//Y
	g_window.command = 0x2B;
	write_command(); //set column address
	g_window.data = g_window.start_y>>8;
	write_data(); //set start column address HB
	g_window.data = g_window.start_y;
	write_data(); //set start column address LB
	g_window.data = g_window.end_y>>8;
	write_data(); //set end column address HB
	g_window.data = g_window.end_y;
	write_data(); //set end column address LB
}

void initialise_ssd1963(void)
{
	//GPIO_PORTL_DATA_R &= ~(PIN_RST); //RST=0
	GPIO_PORTL_DATA_R = 0x00; //RST=0
	wait(100); //wait > 100u
	////
	g_window.command = 0x01;
	write_command(); //software reset
	wait(5000); //wait >5m
	//
	g_window.command = 0xE2;
	write_command(); //sett PLL f=100Mhz
	g_window.data = 0x1D;
	write_data();
	g_window.data = 0x02;
	write_data();
	g_window.data = 0x04;
	write_data();
	////
	g_window.command = 0xE0;
	write_command(); //start PLL
	g_window.data =0x01 ;
	write_data();
	wait(100); //wait > 100u
	//////
	g_window.command = 0xE0;
	write_command(); //lock PLL
	g_window.data = 0x03 ;
	write_data();
	wait(100); //wait > 100us
	//////
	g_window.command = 0x01;
	write_command(); //software reset
	wait(5000); //wait >5m
	////
	g_window.command = 0xE6;
	write_command(); //set LCD pixel CLK = 9 MHz
	g_window.data = 0x01;
	write_data();
	g_window.data = 0x70;
	write_data();
	g_window.data =0xA3 ;
	write_data();
	g_window.command = 0xB0;
	write_command(); //set LCD panel mode
	g_window.data = 0x20;
	write_data(); //TFT panel 24bit
	g_window.data = 0x00;
	write_data(); //TFT mode
	g_window.data = 0x01;
	write_data(); //horizontal size 480-1 HB
	g_window.data = 0xDF;
	write_data(); //horizontal size 480-1 LB
	g_window.data = 0x01 ;
	write_data(); //vertical size 272-1 HB
	g_window.data = 0x0F ;
	write_data(); //vertical size 272-1 LB
	g_window.data = 0x00 ;
	write_data(); //even/odd line RGB
	///////////
	g_window.command = 0xB4;
	write_command(); //set horizontal period
	g_window.data =0x02 ;
	write_data(); //set HT total pxel = 531 HB
	g_window.data = 0x13;
	write_data(); //LB
	g_window.data = 0x00;
	write_data(); //set horiz sync start = 43 HB
	g_window.data =0x2B ;
	write_data(); //LB
	g_window.data = 0x0A;
	write_data(); //set horiz sync = 10
	g_window.data = 0x00;
	write_data(); //set horiz sync start pos = 8 HB
	g_window.data = 0x08;
	write_data(); //LB
	g_window.data = 0x00;
	write_data();
	///////
	g_window.command = 0xB6;
	write_command(); //set vertical period
	g_window.data = 0x01;
	write_data(); //set VT lines = 288 HB
	g_window.data = 0x20;
	write_data(); //LB
	g_window.data = 0x00;
	write_data(); //set VPS = 12 HB
	g_window.data = 0x0C;
	write_data(); //LB
	g_window.data = 0x0A;
	write_data(); //vertical sync pulse = 10
	g_window.data = 0x00 ;
	write_data(); //vert sync pulse start = 4 HB
	g_window.data = 0x04;
	write_data(); //LB
	//////////
	g_window.command = 0x36;
	write_command(); //flip display
	g_window.data = 0x03;
	write_data();
	//////////
	g_window.command = 0xF0;
	write_command(); //pixel data format = 8bit
	g_window.data = 0x00 ;
	write_data();
	//////////
	g_window.command = 0x29;
	write_command(); //display on

}

void clear_screen()
{
	int x, y;
	g_window.start_x = 0;
	g_window.end_x = 480;
    g_window.start_x = 0;
	g_window.end_x = 827;
    window_set();

    g_window.command = 0x2C;
	write_command();
	for(y = 0; y<= g_window.HEIGHT; y++)
		for(x=0; x<= g_window.WIDTH; x++){
			g_window.command = g_window.command;
			write_command();
			g_window.data = g_window.red;
			write_data();
			g_window.data = g_window.green;
			write_data();
			g_window.data = g_window.blue;
			write_data();
		}

}

int main(void)
{
	initialise_setup();
	initialise_ssd1963();

	g_window.red = 0x00;
	g_window.green = 0x44;
	g_window.blue = 0x55;
	g_window.command = 0x2C;
	clear_screen();


	while(1);
}
