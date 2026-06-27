#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<assert.h>
#include<termios.h>
#include<string.h>
#include<sys/time.h>
#include<sys/types.h>
#include<errno.h>
#include "scheduler.h"

 #ifdef USART

#define DEV_NAME  "/dev/ttyS1"
#define UART_BUFSIZE   1024

extern const char HEADER[];

void set_baudrate (struct termios *opt, unsigned int baudrate);

void set_data_bit (struct termios *opt, unsigned int databit);

void set_parity (struct termios *opt, char parity);

void set_stopbit (struct termios *opt, const char *stopbit);

int  set_port_attr (
              int fd,
              int  baudrate,          // B1200 B2400 B4800 B9600 .. B115200
              int  databit,           // 5, 6, 7, 8
              const char *stopbit,    //  "1", "1.5", "2"
              char parity,            // N(o), O(dd), E(ven)
              int vtime,
              int vmin );

typedef struct uart_data
{
    uint8_t source_id[2];
    uint8_t next_id[2];
    uint8_t destination_id[2];
    uint8_t useful_message[16];
    uint8_t temperature;
    uint8_t hunmidity;
    uint8_t infrared;
    uint8_t illumination;
    uint8_t smoke;
}uart_data_to_gw;


extern uint8_t uart_buf[UART_BUFSIZE];

#endif
