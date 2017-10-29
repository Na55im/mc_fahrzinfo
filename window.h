typedef unsigned int    BOOL __attribute__((__mode__(__QI__)));

typedef unsigned int    UNS1 __attribute__((__mode__(__QI__)));
typedef unsigned int    UNS2 __attribute__((__mode__(__HI__)));
//typedef unsigned int    UNS4_ __attribute__((__mode__(__SI__)));
typedef unsigned int    UNS8 __attribute__((__mode__(__DI__)));

typedef int             SGN1 __attribute__((__mode__(__QI__)));
typedef int             SGN2 __attribute__((__mode__(__HI__)));
//typedef int             SGN4_ __attribute__((__mode__(__SI__)));
typedef int             SGN8 __attribute__((__mode__(__DI__)));

#include <stdint.h>
typedef uint8_t         UNS1;   ///< 1 byte unsigned
typedef uint16_t        UNS2;   ///< 2 bytes unsigned
//typedef uint32_t        UNS4;   ///< 4 bytes unsigned
typedef uint64_t        UNS8;   ///< 8 bytes unsigned

typedef int8_t          SGN1;   ///< 1 byte signed
typedef int16_t         SGN2;   ///< 2 bytes signed
//typedef int32_t         SGN4;   ///< 4 bytes signed
typedef int64_t         SGN8;   ///< 8 bytes signed

typedef struct SETPIXEL{
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
