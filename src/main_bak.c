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

//first read the int with the number of bytes the data will have
//then read that number of bytes
bool read_from_fifo(int fd, char* var)
{
    int n_bytes;
    if (read(fd, &n_bytes, sizeof(int)))
    {
        printf("going to read %d bytes\n", n_bytes);
        if (read(fd, var, n_bytes))
            printf("read var: %s\n", var);
        else {
            printf("error in read var. errno: %d\n", errno);
        }
    }

    return true;
}

void read_loop(int fFile, double wWait)
{
	if (fFile < 0) return;

	double max_time = wWait, total_time = 0;
	struct timespec cycle_time = { 0, 50 * 1000 * 1000 };
	double add_time = (double) cycle_time.tv_sec + (double) cycle_time.tv_nsec / 1000000000.;

	char next_line[1024];

	FILE *input_file = fdopen(fFile, "r");

	while (total_time < max_time)
	{
	while (fgets(next_line, 1024, input_file))
	 {
	write(STDOUT_FILENO, next_line, strlen(next_line));
	total_time = 0;
	 }

	nanosleep(&cycle_time, NULL);
	total_time += add_time;
	}

	fclose(input_file);
}

int readSystemInput(char fifoFilename[]) {
		  int fd;
		  fd = open(fifoFilename, O_RDONLY);
		  while(sentinel == 1) {
	    	read_from_fifo(fd, var);
	    	printf("var: %s\n", var);
	    }
	    close(fd);
	    exit(0);
	    return 0;
}

int readArduinoInput(int cport_nr, char shellFilename[], char bufTest[]) {
	int i, n;
	unsigned char buf[4096];
	
	
  while(sentinel == 1)
  {
    n = PollComport(cport_nr, buf, 4095);
    if(n > 0)
    {
      buf[n] = 0;   /* always put a "null" at the end of a string! */
      for(i=0; i < n; i++)
      {
        if(buf[i] < 32)  /* replace unreadable control-codes by dots */
        {
          buf[i] = '.';
        }
      }
      int comp = strncmp((char *)buf, (char *)bufTest, 4);
      if(comp == 0) {
      	execv(shellFilename, &shellFilename);
      	sentinel = 0;
      }
      printf("received %i bytes: %s\n", n, (char *)buf);
    }
#ifdef _WIN32
    Sleep(100);  /* it's ugly to use a sleeptimer, in a real program, change the while-loop into a (interrupt) timerroutine */
#else
    usleep(100000);  /* sleep for 100 milliSeconds */
#endif
  }
  return 0;
}

int main(int argc, char *argv[] )
{
  int cport_nr=16,        /* /dev/ttyS0 (COM1 on windows) */
      bdrate=9600;       /* 9600 baud */
	int i, n;
	unsigned char buf[4096];
//  unsigned char message[] = "Hell of a world";
  char bufTest[] = "HALT!!";
  char fifoFilename[] = "/tmp/trafficLight.fifo";
  char shellFilename[] = "/usr/bin/trafficLightShutdown.sh";
  pid_t pidSystem = fork();
	pid_t pidArduino = fork();

  if(OpenComport(cport_nr, bdrate)) {
    printf("Can not open comport\n");
    return(0);
  }

//  mkfifo(fifoFilename, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH| S_IWOTH);
	
	switch(pidSystem) {
		case -1:
			/* error occured */
			break;
		case 0:
			/* child process listen to the file */
	    // read the fifo file and see if anything was sent to it.
			readSystemInput(fifoFilename);
			break;
		default:
			/* parent stuff work on */
			break;
	}
/*	
	switch(pidArduino) {
		case -1:
			// error occured
			break;
		case 0:
			// child process listen to the file 
	    // read the fifo file and see if anything was sent to it.
//			readArduinoInput(cport_nr, shellFilename, bufTest);
			break;
		default:
			// parent stuff work on 
			break;
	}
*/
/*
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
      	execv(shellFilename, argv);
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
	*/

/*
  int len = sizeof(message);
  printf("Size is reported as: %d\n", len);
  int sendData = SendBuf(cport_nr, message, sizeof(message));
//  int sendData = cprintf(int comport_number, const char *text)
  if(sendData < 0) {
  	printf("Data was not sent properly\n");
  } else {
  	printf("Bytes reported as: %d\n", sendData);
  	printf("It SAYS we sent the data right\n");
  }
*/

  // delete the FIFO file
  /*
  int reResult = remove(fifoFilename);
  if(reResult == 0) {
  	printf("File removed\n");
  }
  */
  printf("Exiting file\n");
  return(0);
}
