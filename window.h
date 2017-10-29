#include <datatypes.h>

typedef struct SETPIXEL{
	// dimensions of the display
	UNS2 HEIGHT = 272;
	UNS2 WIDTH = 480;
	//command & data for writing to display
	UNS2 command; // 2 Byte command
	UNS2 data; // 2 Byte command
	// set courser
	UNS2 start_x;
	UNS2 start_y;
	UNS2 end_x;
	UNS2 end_y;
	UNS1 red;	// 1 Byte color
	UNS1 green; 
	UNS1 blue;
} window;
volatile window g_window;
