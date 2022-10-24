#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <dirent.h>
#include <libgen.h>
#include <signal.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>
#include <sys/time.h>
#include <termios.h>

#ifdef __cplusplus
 extern "C"
 {
#endif

#define RS481_FILE  "/dev/ttyO4"

int rs485SerialInit();  //初始化串口
int reset485Serial(int iFd); //重启485
void Modbus_MakeMessage(unsigned char *pModbusMsg,unsigned char ucLength); 
unsigned short calcrc16( unsigned char * buf, int len);

#ifdef __cplusplus
}
#endif
#endif // SERIALPORT_H
