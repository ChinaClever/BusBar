/* Keybad Driver Test/Example Program
 *
 * Compile with:
 *  gcc -s -Wall -Wstrict-prototypes keybad.c -o keybad_demo
 *
 *
 * Note : 
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <string.h>

//控制凤鸣器 -- 专用

int gpio_fd;

int gpio_init(void)
{
   gpio_fd = open("/dev/inout", O_RDONLY | O_NDELAY);
   return gpio_fd;
}

int gpio_on(int dat)
{
    int ret;
    if(gpio_fd<0)
       return -1;
    ret=ioctl(gpio_fd, 1, dat);
    return ret;
}

int gpio_off(int dat)
{
   int ret;
   if(gpio_fd<0)
      return -1;
   ret=ioctl(gpio_fd, 0, dat);
   return ret;
}




