#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */

#define PORT_NUM "/dev/ttyS0"

/*
 * 'open_port()' - Open serial port 1.
 *
 * Returns the file descriptor on success or -1 on error.
 */
int open_port(void)
{
  int fd; /* File descriptor for the port */
  int n;

  fd = open("/dev/ttyS3", O_RDWR | O_NOCTTY | O_NDELAY);
  if (fd == -1)
  {
   /* Could not open the port. */
    perror("open_port: Unable to open /dev/ttyS3 - ");
  }
  else
    fcntl(fd, F_SETFL, 0);

  n = write(fd, "ATZ\r", 4);
  if (n < 0)
    fputs("write() of 4 bytes failed!\n", stderr);

  return (fd);
}

int read_port(void)
{
    char port[] = "/dev/ttyS3";
	  printf("We're going to try to read\n");
    int fd = open(port, O_RDONLY | O_NOCTTY);
    if (fd == -1)
    {
        /* Could not open the port. */
        perror("open_port: Unable to open /dev/ttyS3 - ");
    } else {
    	printf("We're getting packets\n");
    }

    char buffer[32];
    int n = read(fd, buffer, sizeof(buffer));
    printf("Past the read\n");
    if (n < 0)
    {
        fputs("read failed!\n", stderr);
    } else {
    	printf("read success\n");
    }
    printf("out %s\n", buffer);
    return (fd);
}

int main() 
{
	printf("Hi, welcome to the program\n");
	read_port();
	return 0;
}