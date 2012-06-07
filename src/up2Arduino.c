/**************************************************
 *
 * file: main.c
 * purpose: simple demo that receives characters from
 * the serial port and print them on the screen
 *
 * **************************************************/
// gcc up2Arduino.c rs232.c -Wall -o2 -o display
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

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "rs232.h"

int main(int argc, char *argv[] )
{
  int cport_nr=16,        /* /dev/ttyS0 (COM1 on windows) */
      bdrate=9600;       /* 9600 baud */
	int inputDecision = 0;
	unsigned char message[7];

	if(argc > 1){
		inputDecision = atoi(argv[1]);
	  if(OpenComport(cport_nr, bdrate)) {
	    printf("Can not open comport\n");
	    return(0);
	  }
	  switch(inputDecision) {
	  	case 0:
	  		strcpy((char*)message, "#FF0000");
	  		break;
	  	case 1:
	  		strcpy((char*)message, "#00FF00");
	  		break;
	  	case 2:
	  		strcpy((char*)message, "#00FF00");
	  		break;
	  	case 3:
	  		strcpy((char*)message, "#0000FF");
	  		break;
	  	default:
	  		strcpy((char*)message, "#FF0000");
	  		break;
	  }
	  int sendData = SendBuf(cport_nr, message, sizeof(message));
	  if(sendData < 0) {
	  	printf("Data was not sent properly\n");
	  } else {
	  	printf("It SAYS we sent the data right\n");
	  }
	} else {
		printf("Usage: %s <status>\n", argv[0]);
	}
  return(0);
}
