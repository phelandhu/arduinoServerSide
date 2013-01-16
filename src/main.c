/**************************************************
 *
 * file: main.c
 * purpose: simple demo that receives characters from
 * the serial port and print them on the screen
 *
 * **************************************************/
// gcc main.c rs232.c -Wall -o2 -o test
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>
#include <sys/stat.h>
#include "readConfig.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "rs232.h"

typedef enum { false, true } bool;
char var[100];
int sentinel = 1;
int sentinelFile = 1;

int main(int argc, char *argv[] )
{
	int cport_nr=16,        /* /dev/ttyS0 (COM1 on windows) */
	    bdrate=9600;       /* 9600 baud */
	int i, n;
	unsigned char buf[4096];
	//  unsigned char message[] = "Hell of a world";
	char bufTest[] = "HALT!!";
	char shellFilename[] = "/usr/bin/trafficLightShutdown.sh";
	struct sample_parameters parms;

	printf ("Initializing parameters to default values...\n");
	init_parameters (&parms);

	printf ("Reading config file...\n");
	parse_config (&parms);

	cport_nr=(int)parms.port;
	bdrate=(int)parms.speed;
	if(OpenComport(cport_nr, bdrate)) {
		printf("Can not open comport\n");
		return(0);
	}

	while(sentinel == 1)
	{
		n = PollComport(cport_nr, buf, 4095);
		if(n > 0)
		{
			buf[n] = 0;   // always put a "null" at the end of a string! 
			for(i=0; i < n; i++)
			{
				if(buf[i] < 32)  // replace unreadable control-codes by dots
				{
					buf[i] = '.';
				}
			}
			int comp = strncmp((char *)buf, (char *)bufTest, 4);
			if(comp == 0) {
				// Need to comment this line out until I can recreate the file it is calling
				//      	execv(shellFilename, argv);
				sentinel = 0;
			}
			printf("received %i bytes: %s\n", n, (char *)buf);
		}
		#ifdef _WIN32
			Sleep(100);  // it's ugly to use a sleeptimer, in a real program, change the while-loop into a (interrupt) timerroutine 
		#else
			usleep(100000);  // sleep for 100 milliSeconds 
		#endif
	}

	printf("Exiting file\n");
	return(0);
}
