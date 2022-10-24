#include "serialport.h"


int rs485SerialInit()
{
    int fd;
    int i;
    struct termios opt;

    fd = open(RS481_FILE, O_RDWR);
    if (fd < 0)
    {
        printf("error to open %s\n",RS481_FILE);
        return -1;
    }
    tcgetattr(fd, &opt);

    opt.c_iflag = IGNBRK | IGNPAR ;
    opt.c_oflag = 0 ;
    opt.c_cflag = CS8 | CREAD | CLOCAL ;
    opt.c_lflag = 0 ;

    for ( i=0 ; i<NCCS ; i++ ) opt.c_cc[i] = 0 ;

    opt.c_cc[VTIME] = 1;
    opt.c_cc[VMIN] = 0;

    cfsetispeed(&opt, B9600);
    cfsetospeed(&opt, B9600);

    if(tcsetattr(fd, TCSANOW, &opt) != 0)
    {
        printf("serial error");
        close(fd);
        return -1;
    }

    if (tcflush(fd, TCIOFLUSH) == 0)
    {
        printf("The input and output queues have been flushed.n\n");
    }
    else
    {
        printf("tcflush error");
    }

    return fd;

}

int reset485Serial(int iFd)
{
    printf("Reset RS485 \t\n");
    close(iFd);
    return rs485SerialInit();
}

void Modbus_MakeMessage(unsigned char *pModbusMsg,unsigned char ucLength)
{
    unsigned short usCrc;

    usCrc = (unsigned short)calcrc16(pModbusMsg,ucLength); // six bytes for preset single register

    pModbusMsg[ucLength] = (usCrc >> 8) & 0xff;
    pModbusMsg[ucLength+1] = (usCrc >> 0) & 0xff;
}

unsigned short calcrc16( unsigned char * buf, int len)
{
    unsigned short crc = 0;
    int i,j;
    for ( j = 0; j < len; j++)
    {
        unsigned char b = buf[j];
        for (i = 0; i < 8; i++)
        {
            crc = ((b ^ (unsigned char)crc) & 1) ? ((crc >> 1) ^ 0xA001) : (crc >> 1);
            b >>= 1;
        }
    }
    return crc;
}
