#include<iostream>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <termios.h>
using namespace std;
int fd;
void settings(const char *abc)
{
      fd = open(abc,O_RDWR | O_NOCTTY); /* ttyUSB0 is the FT232 based USB2SERIAL Converter   */
      usleep(3500000);
                                    /* O_RDWR Read/Write access to serial port           */
                                    /* O_NOCTTY - No terminal will control the process   */
                                    /* O_NDELAY -Non Blocking Mode,Does not care about-  */
                                    /* -the status of DCD line,Open() returns immediatly */                                        
                                    
            if(fd == -1)                        /* Error Checking */
                   printf("\n  Error! in Opening ttyUSB0  ");
            else
                   printf("\n  ttyUSB0 Opened Successfully ");
       struct termios toptions;         /* get current serial port settings */
       tcgetattr(fd, &toptions);        /* set 9600 baud both ways */
       cfsetispeed(&toptions, B9600);
       cfsetospeed(&toptions, B9600);   /* 8 bits, no parity, no stop bits */
       toptions.c_cflag &= ~PARENB;
       toptions.c_cflag &= ~CSTOPB;
       toptions.c_cflag &= ~CSIZE;
       toptions.c_cflag |= CS8;         /* Canonical mode */
       toptions.c_lflag |= ICANON;       /* commit the serial port settings */
       tcsetattr(fd, TCSANOW, &toptions);
}
void sendCommand(const char *abc)
{
   write(fd, abc, 1);
}


int main()
{
	settings("/dev/ttyACM0");
	char c;
	for(int i=0;i<100;i++)
	{
		c='W';
		sendCommand(&c);
	}
	for(int i=0;i<100;i++)
	{
		c='A';
		sendCommand(&c);
	}
	for(int i=0;i<100;i++)
	{
		c='W';
		sendCommand(&c);
	}
	for(int i=0;i<100;i++)
	{
		c='D';
		sendCommand(&c);
	}
	return 0;
}
