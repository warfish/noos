//
// Serial I/O
//

#ifndef NOOS_SERIO_H
#define NOOS_SERIO_H

#define SIO_COM1        0x3f8
#define SIO_COM2        0x2f8
#define SIO_COM3        0x3e8
#define SIO_COM4        0x2e8

#define SIO_B115200     1
#define SIO_B57600      2
#define SIO_B38400      3
#define SIO_B28800      4
#define SIO_B19200      6

#define SIO_DATA_OFF    0
#define SIO_INTR_OFF    1
#define SIO_FCR_OFF     2
#define SIO_LCR_OFF     3
#define SIO_MCR_OFF     4
#define SIO_LSR_OFF     5
#define SIO_MSR_OFF     6
#define SIO_SCR_OFF     7

#define SIO_BAUD_LOW_OFF    SIO_DATA_OFF
#define SIO_BAUD_HI_OFF     SIO_INTR_OFF

#define SIO_MSR_DATA_READY  (1 << 0)
#define SIO_MSR_TR_READY    (1 << 5)

void serio_init(uint16_t iobase, int ratediv);
void serio_write(uint16_t iobase, uint8_t byte);
uint8_t serio_read(uint16_t iobase);
int serio_poll(uint16_t iobase);

#endif

