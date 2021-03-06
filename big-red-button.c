/*
,* Copyright © 2013, Malcolm Sparks <malcolm@congreve.com>. All Rights Reserved.
,*
,* A program to convert USB firing events from the Dream Cheeky 'Big Red Button' to MQTT events.
,*/

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LID_CLOSED 21
#define BUTTON_PRESSED 22
#define LID_OPEN 23

#define CMD "/home/schnable/big-red-button/doit\n"

int main(int argc, char **argv)
{
  int fd;
  int i, res, desc_size = 0;
  char buf[256];

  /* 
     Rather than run with sudo and hardcode the device /dev/hidraw0, 
     use a udev rule to make this device - for example:

     $ cat /etc/udev/rules.d/50-big-red-button.rules
     KERNEL=="hidraw*", SUBSYSTEM=="hidraw", ATTRS{product}=="DL100B Dream Cheeky Generic Controller", MODE="0664", GROUP="users", SYMLINK+="big-red-button"
  */

  fd = open("/dev/big-red-button", O_RDWR|O_NONBLOCK);

  if (fd < 0) {
    perror("Unable to open device /dev/big-red-button - check your udev rules.");
    return 1;
  }

  int prior = LID_CLOSED;

  while (1) {

    memset(buf, 0x0, sizeof(buf));
    buf[0] = 0x08;
    buf[7] = 0x02;

    res = write(fd, buf, 8);
    if (res < 0) {
      perror("write");
      exit(1);
    }

    memset(buf, 0x0, sizeof(buf));
    res = read(fd, buf, 8);

    if (res >= 0) {
      if (prior == LID_CLOSED && buf[0] == LID_OPEN) {
         // do something here...
         printf("Ready to fire!\n");
         fflush(stdout);
      } else if (prior != BUTTON_PRESSED && buf[0] == BUTTON_PRESSED) {
         // do something here...
         // printf("Fire!\n");
         system(CMD);
         fflush(stdout);
      } else if (prior != LID_CLOSED && buf[0] == LID_CLOSED) {
         // do something here...
         printf("Stand down!\n");
         fflush(stdout);
      }
      prior = buf[0];
    }
    usleep(20000); /* Sleep for 20ms*/
  }
}
